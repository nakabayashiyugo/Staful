#include "Player.h"

#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Engine/Model.h"
#include "Engine/Image.h"
#include "Engine/Fbx.h"
#include "Engine/SphereCollider.h"
#include "Engine/Easing.h"

#include "PlayScene.h"
#include "SceneTransition.h"
#include "Stage.h"
#include "Timer.h"
#include "StageOrigin.h"

const int MODELSIZE = 0.8f;
//�ړ����Ȃ��Ƃ�
const XMFLOAT3 moveInit(0, 0, 0);
//�O�̈ړ�����
const XMFLOAT3 moveFront(0, 0, 1);
//���̈ړ�����
const XMFLOAT3 moveBack(0, 0, -1);
//�E�̈ړ�����
const XMFLOAT3 moveRight(1, 0, 0);
//���̈ړ�����
const XMFLOAT3 moveLeft(-1, 0, 0);
//moveCount_�̏����l
const float moveCountInit = 1.0f;

Player::Player(GameObject* parent)
	: GameObject(parent, "Player"),
	hModel_(-1),
	//�v���C���[�̑���ɂ���
	moveFinished_(false),
	moveCount_(moveCountInit),
	upVecPlus_(0),
	moveDir_(0, 0, 0), 
	destPos_(0, 0, 0), 
	prevPos_(0, 0, 0),
	gravity_(0, -0.0007f, 0), 
	gravityAcce_(gravity_.y),
	velocity_(XMVectorSet(0, 0, 0, 0)),
	eyeDirection_(XMVectorSet(0, 0, 1, 0)),

	playerState_(STATE_IDLE), 
	prevPlayerState_(STATE_DEAD), 
	stageState_(STATE_START),

	//�摜�Ɋւ���ϐ��̏�����
	hFrame_(-1), 
	hFrameOutline_(-1), 
	hGage_(-1), 
	hTime_(-1),

	//�q�b�g�X�g�b�v�Ɋւ���ϐ��̏�����
	hitStopTime_(0.2f), 
	isHitStop_(false), 

	
	hurdle_Limit_(0),
	tableHitPoint_(XMFLOAT3(0, 0, 0)), isTableHit_(false)
{
	pTrans_ = (SceneTransition*)FindObject("SceneTransition");
	XSIZE = (int)pTrans_->GetMathSize_x();
	ZSIZE = (int)pTrans_->GetMathSize_z();

	Math_Resize(XSIZE, ZSIZE, &math_);

	pPlayScene_ = (PlayScene*)FindObject("PlayScene");

	SetTableMath(pPlayScene_->GetTableMath());

	for (int x = 0; x < XSIZE; x++)
	{
		for (int z = 0; z < ZSIZE; z++)
		{
			if (math_[x][z].mathType_ == MATH_START)
			{
				startPos_ = XMFLOAT3((float)x, 1.0f, (float)z);
			}
			if (math_[x][z].mathType_ == MATH_GOAL)
			{
				goalPos_ = XMFLOAT3((float)x, 1.0f, (float)z);
			}
		}
	}
}

void Player::Initialize()
{
	std::string fileName = "Assets\\Player";
	fileName += std::to_string(pPlayScene_->GetPlayerNum() + 1) + ".fbx";
	hModel_ = Model::Load(fileName);
	assert(hModel_ >= 0);

	hFrame_ = Image::Load("Assets\\Timer_Frame.png");
	assert(hFrame_ >= 0);
	hFrameOutline_ = Image::Load("Assets\\Timer_FrameOutline.png");                                                                          
	assert(hFrameOutline_ >= 0);
	hGage_ = Image::Load("Assets\\Timer_Gage.png");
	assert(hGage_ >= 0);
	hTime_ = Image::Load("Assets\\Logo_TIME.png");
	assert(hTime_ >= 0);
	
	pTimer_ = new Timer(30);
}

void Player::Update()
{
	Stage* pStage = (Stage*)FindObject("Stage");
	
	switch (stageState_)
	{
	case STATE_START:
		transform_.position_ = startPos_;
		prevPos_ = transform_.position_;
		destPos_ = transform_.position_;
		stageState_ = STATE_PLAY;
		playerState_ = STATE_IDLE;
		break;
	case STATE_PLAY:
		PlayUpdate();
		break;
	case STATE_GOAL:
		break;
	}

	if (pPlayScene_->GetTableChange())
	{
		SetTableMath(pPlayScene_->GetTableMath());
		pPlayScene_->SetTableChange(false);
	}
}

void Player::Draw()
{ 
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);

	TimeGageManagement();

	Image::SetTransform(hGage_, tGage_);
	Image::SetTransform(hFrame_, tFrame_);
	Image::SetTransform(hFrameOutline_, tFrameOutline_);
	Image::Draw(hGage_);
	Image::Draw(hFrame_);
	Image::Draw(hFrameOutline_);

	const XMFLOAT3 tPos = XMFLOAT3(-0.1f, 0.8f, 0);
	const XMFLOAT3 tScale = XMFLOAT3(0.5f, 0.5f, 1);
	tTime_.position_ = tPos;
	tTime_.scale_ = tScale;
	Image::SetTransform(hTime_, tTime_);
	Image::Draw(hTime_);
}

void Player::Release()
{
}

void Player::PlayUpdate()
{
	//�J����
	CameraPosSet();
	//�R���C�_�[
	SphereCollider* pSC = new SphereCollider(MODELSIZE / 2);
	this->AddCollider(pSC);

	//���Ԑ؂�
	pTimer_->Update();
	if (pTimer_->isTimeUpped())
	{
		stageState_ = STATE_FAILURE;
	}

	switch (playerState_)
	{
	case STATE_IDLE:
		IdleUpdate();
		break;
	case STATE_WALK:
		WalkUpdate();
		break;
	case STATE_JAMP:
		JampUpdate();
		break;
	case STATE_FALL:
		FallUpdate();
		break;
	case STATE_DEAD:
		DeadUpdate();
		break;
	}
	if (playerState_ != prevPlayerState_)
	{
		SetAnimFramerate();
		prevPlayerState_ = playerState_;
	}
}

void Player::CameraPosSet()
{
	//�J����
	//�v���C���[�ƃJ�����Ƃ̋���
	const XMFLOAT3 dirCamToPlayer = XMFLOAT3(0, 7, -5);
	//�J�����̈ʒu
	const XMFLOAT3 camPos = XMFLOAT3(transform_.position_.x + dirCamToPlayer.x,
		dirCamToPlayer.y,
		transform_.position_.z + dirCamToPlayer.z);
	const XMFLOAT3 camTarget = XMFLOAT3(transform_.position_.x, 0, transform_.position_.z);
	Camera::SetPosition(camPos);
	Camera::SetTarget(camTarget);
}

bool Player::Is_InSide_Table(XMFLOAT3 _pos)
{
	return _pos.x >= 0 && _pos.x < XSIZE &&
		_pos.z  >= 0 && _pos.z < ZSIZE;
}

void Player::PlayerOperation()
{
	if (playerState_ != STATE_DEAD)
	{
		//�O�㍶�E�ړ�
		if (Input::IsKey(DIK_W))
		{
			//�ړ�����
			moveDir_ = moveFront;
			playerState_ = STATE_WALK;
			if (Input::IsKey(DIK_SPACE))
			{
				playerState_ = STATE_JAMP;
			}
		}
		if (Input::IsKey(DIK_S))
		{
			//�ړ�����
			moveDir_ = moveBack;
			playerState_ = STATE_WALK;
			if (Input::IsKey(DIK_SPACE))
			{
				playerState_ = STATE_JAMP;
			}
		}
		if (Input::IsKey(DIK_A))
		{
			//�ړ�����
			moveDir_ = moveLeft;
			playerState_ = STATE_WALK;
			if (Input::IsKey(DIK_SPACE))
			{
				playerState_ = STATE_JAMP;
			}
		}
		if (Input::IsKey(DIK_D))
		{
			//�ړ�����
			moveDir_ = moveRight;
			playerState_ = STATE_WALK;
			if (Input::IsKey(DIK_SPACE))
			{
				playerState_ = STATE_JAMP;
			}
		}
	}
}

void Player::PlayerMove()
{
	//�ړ��悪�ǂ�������
	if (DestPosMathType() == MATH_WALL)
	{
		destPos_ = prevPos_;
		//�ړ��I��
		moveFinished_ = true;
		return;
	}
	//moveCount�̖��b�����Ă����l
	const float cntUpdate = -0.03f;
	moveCount_ += cntUpdate;

	Easing* pEasing = new Easing();
	//velocity_�ɓ���邽�߂�XMFLOAT3�^�̕ϐ�
	XMFLOAT3 vec = XMFLOAT3(0, 0, 0);
	vec.x = moveDir_.x * (moveCountInit - pEasing->EaseInSine(moveCount_));
	vec.y = moveDir_.y;
	vec.z = moveDir_.z * (moveCountInit - pEasing->EaseInSine(moveCount_));

	velocity_ = XMLoadFloat3(&vec);
	//�i�ޕ����Ɏ������������킹��
	if (XMVectorGetX(XMVector3Length(velocity_)) != 0)
	{
		//�������0�̈ړ��x�N�g��
		XMVECTOR normalVelo = XMVectorSet(XMVectorGetX(velocity_), 0,
										  XMVectorGetZ(velocity_), 0);
		XMVECTOR v = XMVector3Dot(eyeDirection_, XMVector3Normalize(normalVelo));

		float angle = XMConvertToDegrees(acos(XMVectorGetX(v)));

		// XMVector3Cross : vFront �� vMove �̊O�ϋ��߂�
		XMVECTOR vCross = XMVector3Cross(eyeDirection_, XMVector3Normalize(normalVelo));

		// vFront �� vMove �̊O�ς��������������� angle ��-1�������āA�����𔽑΂ɂ���
		if (XMVectorGetY(vCross) < 0)
		{
			angle *= -1;
		}
		transform_.rotate_.y = angle;
	}
	if (moveCount_ <= 0)
	{
		moveCount_ = moveCountInit;
		//�ړ��I��
		moveFinished_ = true;
	}
	transform_.position_ = prevPos_ + velocity_;
}

MATHTYPE Player::DestPosMathType()
{
	MATHTYPE retType;
	//�i�񂾐�̈ʒu
	destPos_.x = prevPos_.x + moveDir_.x;
	destPos_.z = prevPos_.z + moveDir_.z;
	retType = GetMathType(destPos_).mathType_;

	return retType;
}

MATHDEDAIL Player::GetMathType(XMFLOAT3 _pos)
{
	if (!Is_InSide_Table(_pos))
	{
		return MATHDEDAIL{ MATH_HOLE, transform_};
	}
	return math_[_pos.x][_pos.z];
}

void Player::IdleUpdate()
{
	transform_.position_ = destPos_;
	prevPos_ = transform_.position_;
	moveFinished_ = false;
	moveDir_ = moveInit;
	//�d�͏�����
	gravity_.y = gravityAcce_;
	//player�̑���
	PlayerOperation();
	//�����Ă���}�X�̌���
	MathTypeEffect();
}

void Player::WalkUpdate()
{
	PlayerMove();
	if (moveFinished_)
	{
		playerState_ = STATE_IDLE;
	}
}

void Player::JampUpdate()
{
	//�W�����v�ړ��̒ʏ�̈ړ��Ƃ̋����̔{��
	const int normalMoveVectoMult = 2;
	//moveDir_�𐳋K�����邽�߂̕ϐ�
	XMFLOAT3 normalMoveDir = XMFLOAT3(moveDir_.x, 0, moveDir_.z);
	XMVECTOR dir;
	//moveDir_��XMVECTOR�^�ɕϊ�
	dir = XMLoadFloat3(&normalMoveDir);
	//���K��
	dir = XMVector3Normalize(dir);
	//dir��XMFLOAT3�^�ɕϊ�
	XMStoreFloat3(&normalMoveDir, dir);
	moveDir_.x = normalMoveDir.x * normalMoveVectoMult;
	moveDir_.z = normalMoveDir.z * normalMoveVectoMult;

	//������̃x�N�g���̏�����
	const float upVecPlusInit = 0.1f;
	upVecPlus_ = upVecPlusInit;
	upVecPlus_ += gravity_.y;
	gravity_.y += gravityAcce_;
	moveDir_.y += upVecPlus_;
	PlayerMove();

	//player�̏�Ԃ�JAMP����FALL�ɐ؂�ւ�
	//�ړ��O�̈ʒu��0,
	//�ړ���̈ʒu��1�Ƃ������́A
	//JAMP����FALL�ɐ؂�ւ���l
	const float switchValue = 0.5f;
	//���݂̈ʒu����ړ��O�̈ʒu���������x�N�g��
	XMFLOAT3 distVec = XMFLOAT3(0, 0, 0);
	distVec.x = transform_.position_.x - prevPos_.x;
	distVec.z = transform_.position_.z - prevPos_.z;
	//���݂̈ʒu�ƈړ��O�̈ʒu�̒�������
	float dist = XMVectorGetX(XMVector3Length(XMLoadFloat3(&distVec)));
	//��������Ȃ��ړ�����
	XMFLOAT2 vec2MoveDir = XMFLOAT2(moveDir_.x, moveDir_.z);
	//�ړ������̒���
	float moveDist = XMVectorGetX(XMVector2Length(XMLoadFloat2(&vec2MoveDir)));

	//���݂̈ʒu�ƈړ��O�̈ʒu�̒����������A
	//JAMP��FALL��؂�ւ���l�܂łɒB���Ă�����
	if (dist >= moveDist * switchValue)
	{
		playerState_ = STATE_FALL;
	}
	if (moveFinished_)
	{
		playerState_ = STATE_IDLE;
	}
}

void Player::FallUpdate()
{
	upVecPlus_ += gravity_.y;
	gravity_.y += gravityAcce_;
	moveDir_.y += upVecPlus_;
	PlayerMove();

	if (moveFinished_)
	{
		playerState_ = STATE_IDLE;
		if (DestPosMathType() == MATH_HOLE)
		{
			playerState_ = STATE_DEAD;
		}
		moveFinished_ = false;
	}
}

void Player::DeadUpdate()
{
	ReturnToStartMath();
}

void Player::ReturnToStartMath()
{
	moveDir_.x = startPos_.x - prevPos_.x;
	moveDir_.y = 0;
	moveDir_.z = startPos_.z - prevPos_.z;
	PlayerMove();
	transform_.position_.y = 10;
	if (moveFinished_)
	{
		stageState_ = STATE_START;
	}
}

void Player::MathTypeEffect()
{
	//�R���x�A�ɂ���Ĉړ��������
	const XMVECTOR converyor_velocity = XMVectorSet(-1.0f, 0, 0, 0);
	//�����̗����Ă���}�X�̏��
	standMath_ = GetMathType(transform_.position_);
	switch (standMath_.mathType_)
	{
	case MATH_CONVEYOR:
		//�����Ă���R���x�A�̌����Ă�������������]�s��
		XMMATRIX yrot = XMMatrixRotationY(XMConvertToRadians(-standMath_.mathPos_.rotate_.z));
		//��̉�]�s���converyor_velocity�̕ς���
		XMVECTOR rotConvVec = XMVector3Transform(converyor_velocity, yrot);
		//�ړ���������̃x�N�g���ɂ���
		XMStoreFloat3(&moveDir_, rotConvVec);
		playerState_ = STATE_WALK;
		break;
	case MATH_GOAL:
		stageState_ = STATE_GOAL;
		break;
	case MATH_HOLE:
		playerState_ = STATE_FALL;
		break;
	default:break;
	}
}

void Player::SetAnimFramerate()
{
	switch (playerState_)
	{
	case STATE_IDLE:
		startFrame_ = 1;
		endFrame_ = 60;
		break;
	case STATE_WALK:
		startFrame_ = 61;
		endFrame_ = 120;
		break;
	case STATE_JAMP:
		startFrame_ = 121;
		endFrame_ = 150;
		break;
	case STATE_FALL:
		startFrame_ = 150;
		endFrame_ = 150;
		break;
	}
	Model::SetAnimFrame(hModel_, startFrame_, endFrame_, 1);
}

void Player::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Togetoge")
	{
		moveCount_ = moveCountInit;
		prevPos_ = transform_.position_;
		playerState_ = STATE_DEAD;
		isHitStop_ = true;
	}
}

void Player::TimeGageManagement()
{
	//���ԃQ�[�W
	const XMFLOAT3 timerPos = XMFLOAT3(-0.6f, 0.8f, 0);
	const XMFLOAT3 timerScale = XMFLOAT3(2.0f, 0.5f, 1);

	tFrame_.position_ =
		XMFLOAT3(timerPos.x, timerPos.y, timerPos.z);
	tFrame_.scale_ = timerScale;

	tFrameOutline_.position_ =
		XMFLOAT3(timerPos.x, timerPos.y, timerPos.z);
	tFrameOutline_.scale_ = XMFLOAT3(timerScale.x + 0.05f, timerScale.y + 0.05f, timerScale.z);

	tGage_.position_ =
		XMFLOAT3((((timerPos.x / 2) / pTimer_->GetLimitTime()) * pTimer_->GetCurTime()) + timerPos.x,
			timerPos.y, timerPos.z);
	tGage_.scale_ = XMFLOAT3(float(pTimer_->GetLimitTime() - pTimer_->GetCurTime()) /
		100 * (timerScale.x / (float(pTimer_->GetLimitTime()) / 100))
		, timerScale.y, timerScale.z);
}
