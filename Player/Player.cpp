#include "Player.h"

#include <map>

#include "../Engine/Input.h"
#include "../Engine/Camera.h"
#include "../Engine/Model.h"
#include "../Engine/Image.h"
#include "../Engine/Fbx.h"
#include "../Engine/SphereCollider.h"
#include "../Engine/Easing.h"
#include "../Engine/Shaker.h"
#include "../Engine/Audio.h"
#include "../Engine/Text.h"

#include "../Scene/PlayScene.h"
#include "../Scene/SceneTransition.h"

#include "../Stage.h"
#include "../Timer.h"
#include "../StageOrigin.h"
#include "State/StateManager.h"
#include "State/PlayerState.h"

const int MODELSIZE = 0.8f;
//�W�����v���̍ō����B�_
const float JAMPMAXHEIGHT = 2;
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
const float moveCountInit = 0.0f;
//moveCntUpdate_�̏����l
const float moveCntUpdateInit = 0.03f;
//moveCount_�̈ړ����I������l
const float moveCountEnd = 1.0f;
//�W�����v�ړ��̒ʏ�̈ړ��Ƃ̋����̔{��
const int normalMoveVectoMult = 2;
//�����鑬�x�̏����l
const float fallSpeedInit = 0.0f;
//�J�����̐U���̎��
SHAKETYPE camShakeType;
//�J�����̐U���̋���
const float camShakePower = 2.0f;
//�������Ԍ��炷�}�X�̌��鎞��
const float decTime = 5.0f;
//�����}�X���񂾎��̃A�j���[�V�����̒���(�b)
const float confAnimTime = 2;
//���������̋��t���[��
const int simulPressFrame = 5;

Player::Player(GameObject* parent)
	: GameObject(parent, "Player"),
	hModel_(-1),
	//�J�����ɂ���
	camPos_(XMFLOAT3(0, 0, 0)),
	camTarget_(XMFLOAT3(0, 0, 0)),
	dirCamToPlayer_(XMFLOAT3(0, 0, 0)),
	//�v���C���[�̑���ɂ���
	moveFinished_(false),
	moveCount_(moveCountInit),
	moveCntUpdate_(moveCntUpdateInit),
	pushButton_(DIR_MAX),
	moveDir_(0, 0, 0),
	destPos_(0, 0, 0),
	prevPos_(0, 0, 0),
	gravity_(0, -0.0007f, 0),
	gravityAcce_(gravity_.y),
	velocity_(XMVectorSet(0, 0, 0, 0)),
	eyeDirection_(XMVectorSet(0, 0, 1, 0)),
	deadHeight_(-1.0f),
	isReturn_(false),
	operableFrames_(10),
	onConvFrames_(0),

	playerState_(STATE_IDLE),
	prevPlayerState_(STATE_DEAD),
	stageState_(STATE_START),

	isConfAnim_(false),

	//��������ɂ���
	hOpeExplanBackGround_(-1),

	//�e�ɂ���
	hShadow_(-1),

	//�摜�Ɋւ���ϐ��̏�����
	hFrame_(-1),
	hFrameOutline_(-1),
	hGage_(-1),
	hTime_(-1),
	gameTime_(30),

	//�q�b�g�X�g�b�v�Ɋւ���ϐ��̏�����
	hitStopTime_(0.2f),
	isHitStop_(false),
	pHitStopTimer_(nullptr),
	isCamShake_(true),

	//�A�j���[�V�����Ɋւ���ϐ�
	//�ړ��ɂ�����t���[����
	moveFrameNum_(moveCountEnd / moveCntUpdateInit)

{
	MathVolumeRead();
	Math_Resize(mathVolume_.x, mathVolume_.z, &math_);
	Read();

	pPlayScene_ = (PlayScene*)FindObject("PlayScene");
	playerHeight_ = pPlayScene_->GetPlayerHeight();

	SetTableMath(pPlayScene_->GetTableMath());

	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int z = 0; z < mathVolume_.z; z++)
		{
			if (math_[x][z].mathType_ == MATH_START)
			{
				startPos_ = XMFLOAT3((float)x, playerHeight_, (float)z);
			}
			if (math_[x][z].mathType_ == MATH_GOAL)
			{
				goalPos_ = XMFLOAT3((float)x, playerHeight_, (float)z);
			}
		}
	}
	PossiMoveDirInit();

	pText_ = new Text();
	pText_->Initialize("Assets\\opeChar.png", 16, 32, 16);
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

	//��������̔w�i���[�h
	hOpeExplanBackGround_ = Image::Load("Assets\\opeBackGround.png");
	assert(hOpeExplanBackGround_ >= 0);
	
	pTimer_ = new Timer(gameTime_);

	ShadowInit();
	//���y������
	AudioInit();
	//�X�e�[�g������
	StateInit();

	//�J�����̐U���@������
	pCamShaker_ = new Shaker();
}

void Player::Update()
{
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

	//���ԃQ�[�W
	TimeGageManagement();

	//�e�\��
	ShadowDraw();

	Image::SetTransform(hGage_, tGage_);
	Image::SetTransform(hFrame_, tFrame_);
	Image::SetTransform(hFrameOutline_, tFrameOutline_);
	Image::Draw(hGage_);
	Image::Draw(hFrame_);
	Image::Draw(hFrameOutline_);

	//���ԃQ�[�W�Ƃ̋���
	const XMFLOAT3 tDis = XMFLOAT3(-0.1f, 0.1f, 0);
	const XMFLOAT3 tScale = XMFLOAT3(0.25f, 0.25f, 1);
	tTime_.position_ = XMFLOAT3(tFrame_.position_.x + tDis.x, tFrame_.position_.y + tDis.y, tDis.z);
	tTime_.scale_ = tScale;
	Image::SetTransform(hTime_, tTime_);
	Image::Draw(hTime_);

	pPlayScene_->PlayerNumDraw();

	OperateExplanDraw();
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

	ShadowManagement();

	pStateManager_->Update();
	//�A�j���[�V�������X�V����
	SetAnimFramerate();
}

void Player::CameraPosSet()
{
	const XMFLOAT3 dirCamToPlayerInit = XMFLOAT3(0, 7, -5);
	//�J����
	//�v���C���[�ƃJ�����Ƃ̋���
	dirCamToPlayer_ = dirCamToPlayerInit;
	//�J���������Ă���I�u�W�F�N�g
	camTarget_ = XMFLOAT3(transform_.position_.x, 0, transform_.position_.z);
	//�J�����̈ʒu
	camPos_ = XMFLOAT3(camTarget_.x + dirCamToPlayer_.x,
						camTarget_.y + dirCamToPlayer_.y,
						camTarget_.z + dirCamToPlayer_.z);
	if (!isHitStop_)
	{
		Camera::SetPosition(camPos_);
		Camera::SetTarget(camTarget_);
	}
	//�J�����U��
	CameraShake();
}

void Player::CameraShakeInit()
{
	Camera::ShakeInit(camShakeType, hitStopTime_, camShakePower);
	
}

void Player::CameraShake()
{
	Camera::CameraShake();
}

bool Player::Is_InSide_Table(XMFLOAT3 _pos)
{
	return _pos.x >= 0 && _pos.x < mathVolume_.x &&
		_pos.z  >= 0 && _pos.z < mathVolume_.z;
}

void Player::StateInit()
{
	//�X�e�[�g�}�l�[�W���[�ɃX�e�[�g�ǉ�
	pStateManager_ = new StateManager(this);

	pStateManager_->AddState("STATE_IDLE", new PlayerIdleState(pStateManager_));
	pStateManager_->AddState("STATE_WALK", new PlayerWalkState(pStateManager_));
	pStateManager_->AddState("STATE_JUMP", new PlayerJumpState(pStateManager_));
	pStateManager_->AddState("STATE_FALL", new PlayerFallState(pStateManager_));
	pStateManager_->AddState("STATE_CONVMOVE", new PlayerConvMoveState(pStateManager_));
	pStateManager_->AddState("STATE_CONF", new PlayerConfState(pStateManager_));
	pStateManager_->AddState("STATE_DEAD", new PlayerDeadState(pStateManager_));

	//�ŏ��̃X�e�[�g���߂�
	pStateManager_->ChangeState("STATE_IDLE");
}

void Player::PossiMoveDirInit()
{
	for (int i = 0; i < DIR_MAX; i++)
	{
		pushButtonMoveDir_[i] = (PLAYER_MOVEDIR)i;
	}

	possiMoveDir_[pushButtonMoveDir_[DIR_W]] = moveFront;
	possiMoveDir_[pushButtonMoveDir_[DIR_S]] = moveBack;
	possiMoveDir_[pushButtonMoveDir_[DIR_D]] = moveRight;
	possiMoveDir_[pushButtonMoveDir_[DIR_A]] = moveLeft;
}

void Player::OperateExplanDraw()
{
	//���������\�������̈ʒu
	const XMFLOAT2 opePos = XMFLOAT2(50, 200);

	//��s�̏c�̒���
	const float lineHeight = 30;

	if (playerState_ == STATE_CONF)
	{
		for (int i = 0; i < DIR_MAX; i++)
		{
			moveDirStr_[i] = "?";
		}
	}
	else
	{
		for (int i = 0; i < DIR_MAX; i++)
		{
			switch (pushButtonMoveDir_[i])
			{
			case DIR_W:
				moveDirStr_[i] = "FRONT";
				break;
			case DIR_S:
				moveDirStr_[i] = "BACK";
				break;
			case DIR_D:
				moveDirStr_[i] = "RIGHT";
				break;
			case DIR_A:
				moveDirStr_[i] = "LEFT";
			}
		}
	}

	//�ړ������\��
	//W���������Ƃ��̈ړ�����
	moveDirStr_[DIR_W] = "W - " + moveDirStr_[DIR_W];
	//S���������Ƃ��̈ړ�����
	moveDirStr_[DIR_S] = "S - " + moveDirStr_[DIR_S];
	//A���������Ƃ��̈ړ�����
	moveDirStr_[DIR_A] = "A - " + moveDirStr_[DIR_A];
	//W���������Ƃ��̈ړ�����
	moveDirStr_[DIR_D] = "D - " + moveDirStr_[DIR_D];

	for (int i = 0; i < DIR_MAX; i++)
	{
		pText_->Draw(opePos.x, opePos.y + lineHeight * i, moveDirStr_[i].c_str());
	}
}

void Player::PlayerOperation()
{
	if (playerState_ != STATE_DEAD){
		//�{�^����������Ă���̃t���[����
		static int pushButtonFrame = 0;
		//�O�㍶�E�ړ�
		if (Input::IsKeyDown(DIK_W)){
			//�ړ�����
			pushButton_ = DIR_W;
			pushButtonFrame = 0;
		}
		if (Input::IsKeyDown(DIK_S)){
			//�ړ�����
			pushButton_ = DIR_S;
			pushButtonFrame = 0;
		}
		if (Input::IsKeyDown(DIK_D)) {
			//�ړ�����
			pushButton_ = DIR_D;
			pushButtonFrame = 0;
		}
		if (Input::IsKeyDown(DIK_A)){
			//�ړ�����
			pushButton_ = DIR_A;
			pushButtonFrame = 0;
		}	
		if (Input::IsKey(DIK_SPACE)) {
			isPushSpace_ = true;
		}

		//�����������ꂽ������
		if (pushButton_ != DIR_MAX)
		{
			if (isPushSpace_)
			{
				playerState_ = STATE_JUMP;
				isPushSpace_ = false;
				pushButtonFrame = 0;
				moveDir_ = possiMoveDir_[pushButtonMoveDir_[pushButton_]];
				pushButton_ = DIR_MAX;
			}
			pushButtonFrame++;
			if (pushButtonFrame > simulPressFrame)
			{
				playerState_ = STATE_WALK;
				isPushSpace_ = false;
				pushButtonFrame = 0;
				moveDir_ = possiMoveDir_[pushButtonMoveDir_[pushButton_]];
				pushButton_ = DIR_MAX;
			}
		}
	}
}

void Player::PlayerMove()
{
	isConfAnim_ = false;
	prevRot_ = transform_.rotate_;
	//�ړ��悪�ǂ�������
	if (WallCheck())
	{
		//�ړ��I��
		destPos_ = prevPos_;
		moveFinished_ = true;
		return;
	}

	//moveCount�̖��b�����Ă����l
	const float cntUpdate = 0.03f;
	moveCount_ += cntUpdate;
	if (moveCount_ > moveCountEnd) moveCount_ = moveCountEnd;

	Easing* pEasing = new Easing();
	//velocity_�ɓ���邽�߂�XMFLOAT3�^�̕ϐ�
	XMFLOAT3 vec = XMFLOAT3(0, 0, 0);
	vec.x = moveDir_.x * pEasing->EaseOutQuad(moveCount_);
	vec.y = moveDir_.y * pEasing->EaseOutCirc(moveCount_ * normalMoveVectoMult);
	vec.z = moveDir_.z * pEasing->EaseOutQuad(moveCount_);

	delete pEasing;

	velocity_ = XMLoadFloat3(&vec);

	if (moveCount_ >= moveCountEnd)
	{
		MoveFinished();
	}
	transform_.position_ = prevPos_ + velocity_;
}

void Player::ChangePlayerDir(XMVECTOR _vec)
{
	//�i�ޕ����Ɏ������������킹��
	if (XMVectorGetX(XMVector3Length(_vec)) != 0)
	{
		//�������0�̈ړ��x�N�g��
		XMVECTOR normalVelo = XMVectorSet(XMVectorGetX(_vec), 0, XMVectorGetZ(_vec), 0);
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
}

void Player::ChangePlayerDir(XMFLOAT3 _vec)
{
	//������XMVECTOR�^�ɕϊ�����
	XMVECTOR velo;
	velo = XMLoadFloat3(&_vec);
	ChangePlayerDir(velo);
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

bool Player::WallCheck()
{
	bool ret = false;
	//��������Ȃ�moveDir_
	XMFLOAT2 vec2MoveDir;
	vec2MoveDir.x = moveDir_.x;
	vec2MoveDir.y = moveDir_.z;
	//moveDir_�̒���
	float moveDirSize = XMVectorGetX(XMVector2Length(XMLoadFloat2(&vec2MoveDir)));

	for (int i = 1; i <= moveDirSize; i++)
	{
		//moveDir_�𐳋K������ϐ�
		XMFLOAT2 normalMoveDir;
		XMStoreFloat2(&normalMoveDir, XMVector2Normalize(XMLoadFloat2(&vec2MoveDir)));
		destPos_.x = prevPos_.x + normalMoveDir.x * i;
		destPos_.z = prevPos_.z + normalMoveDir.y * i;
		if (GetMathType(destPos_).mathType_ == MATH_WALL)
		{
			ret = true;
		}
	}
	if (DestPosMathType() == MATH_WALL)
	{
		ret = true;
	}
	return ret;
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
	velocity_ = XMVectorSet(0, 0, 0, 0);
	transform_.position_ = destPos_;
	prevPos_ = transform_.position_;
	moveFinished_ = false;
	moveDir_ = moveInit;
	//�����Ă���}�X�̌���
	MathTypeEffect();
	//player�̑���
	PlayerOperation();
}

void Player::WalkUpdate()
{
	ChangePlayerDir(moveDir_);
	PlayerMove();
	if (moveFinished_)
	{
		playerState_ = STATE_IDLE;
	}
}

void Player::JumpUpdate()
{
	ChangePlayerDir(moveDir_);
	
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

	moveDir_.y = JAMPMAXHEIGHT;
	PlayerMove();

	if (moveFinished_)
	{
		playerState_ = STATE_IDLE;
	}
}

void Player::FallUpdate()
{
	fallSpeed_ += gravity_.y;
	gravity_.y += gravityAcce_;
	XMFLOAT3 vec;
	vec.x = XMVectorGetX(velocity_);
	vec.y = fallSpeed_;
	vec.z = XMVectorGetZ(velocity_);
	velocity_ = XMLoadFloat3(&vec);
	transform_.position_ += velocity_;

	if (transform_.position_.y <= deadHeight_)
	{
		fallSpeed_ = fallSpeedInit;
		gravity_.y = gravityAcce_;
		playerState_ = STATE_DEAD;
		moveFinished_ = false;
	}
}

void Player::ConvMoveUpdate()
{
	PlayerMove();
	if (moveFinished_)
	{
		playerState_ = STATE_IDLE;
	}
}

void Player::ConfUpdate()
{
	PLAYER_MOVEDIR tmp;
	srand(time(NULL));
	//0�`�ړ��\�����̐��܂ł̗����擾
	int random1 = (rand() % (int)DIR_MAX);
	int random2 = (rand() % (int)DIR_MAX);
	//����ւ�
	tmp = pushButtonMoveDir_[random1];
	pushButtonMoveDir_[random1] = pushButtonMoveDir_[random2];
	pushButtonMoveDir_[random2] = tmp;

	//�����̃A�j���[�V����
	//�����̃A�j���[�V���������Ă��Ȃ����
	//����]���邩
	const int confRotNum = 2;

	const float confCntInit = 0.0f;
	const float confCntUpdate = 1 / (confAnimTime * FPS);
	static float confCnt = confCntInit;
	confCnt += confCntUpdate;

	Easing* pEasing = new Easing();
	float ease = pEasing->EaseInOutQuint(confCnt);

	transform_.rotate_.y = prevRot_.y + 360 * confRotNum * ease;

	delete pEasing;

	if (ease >= 1)
	{
		transform_.rotate_ = prevRot_;
		isConfAnim_ = true;
		confCnt = confCntInit;
		playerState_ = STATE_IDLE;
	}
}

void Player::DeadUpdate()
{
	Audio::Play(hSE_Damage_, 1);
	HitStopUpdate();
	if (!isHitStop_)
	{
		Audio::Stop(hSE_Damage_);
		VFX::End(deadEmitHandle_);
		ReturnToStartMath();
	}
}

void Player::ReturnToStartMath()
{
	moveFinished_ = false;
	isReturn_ = true;
	moveDir_.x = startPos_.x - prevPos_.x;
	moveDir_.y = 0;
	moveDir_.z = startPos_.z - prevPos_.z;
	PlayerMove();
	//�X�^�[�g�}�X�ɖ߂�Ƃ��̃v���C���[�̍���
	const float returnHeight = 10;
	transform_.position_.y = returnHeight;
	if (moveFinished_)
	{
		isReturn_ = false;
		stageState_ = STATE_START;
	}
}

void Player::MoveFinished()
{
	moveCount_ = moveCountInit;
	//�ړ��I��
	moveFinished_ = true;
	isConfAnim_ = false;
}

void Player::MathTypeEffect()
{
	Stage* pStage = pPlayScene_->GetStagePointer();
	//�R���x�A�ɂ���Ĉړ��������
	const XMVECTOR converyor_velocity = XMVectorSet(-1.0f, 0, 0, 0);

	//�����̗����Ă���}�X�̏��
	standMath_ = GetMathType(transform_.position_);
	switch (standMath_.mathType_)
	{
	case MATH_CONVEYOR:
		//�R���x�A�ɘA�����ď���Ă���t���[������
		//�R���x�A�}�X�ɏ����������ł���t���[������
		//����Ȃ�����R���x�A�ɏ���Ă锻��ɂ��Ȃ�
		onConvFrames_++;
		if (onConvFrames_ < operableFrames_) break;
		onConvFrames_ = 0;

		//�����Ă���R���x�A�̌����Ă�������������]�s��
		XMMATRIX yrot = XMMatrixRotationY(XMConvertToRadians(-standMath_.mathPos_.rotate_.z));
		//��̉�]�s���converyor_velocity�̕ς���
		XMVECTOR rotConvVec = XMVector3Transform(converyor_velocity, yrot);
		//�ړ���������̃x�N�g���ɂ���
		XMStoreFloat3(&moveDir_, rotConvVec);
		if (DestPosMathType() != MATH_WALL)
		{
			playerState_ = STATE_CONVMOVE;
		}
		else
		{
			destPos_ = prevPos_;
		}
		break;
	case MATH_GOAL:
		stageState_ = STATE_GOAL;
		break;
	case MATH_HOLE:
		playerState_ = STATE_FALL;
		break;
	case MATH_TIMELIMITDEC:
		//�������Ԍ��炷
		if (!pStage->IsTimeLimitDecCheck(transform_.position_.x, transform_.position_.z))
		{
			pTimer_->SetCurTime(pTimer_->GetCurTime() + decTime);
			pStage->SetIsStand(transform_.position_.x, transform_.position_.z, true);
		}
		break;
	case MATH_CONFUSION:
		if(!isConfAnim_)		playerState_ = STATE_CONF;
		break;
	}

	prevStandMath_ = standMath_;
}

void Player::ShadowInit()
{
	//�e�̃��f���̃t�@�C���l�[��
	std::string modelName = "Player_Shadow.fbx";
	//���f���̓����Ă�t�H���_��
	std::string folderName = "Assets\\";
	modelName = folderName + modelName;

	//���f�����[�h
	hShadow_ = Model::Load(modelName);
	assert(hShadow_ >= 0);
}

void Player::ShadowDraw()
{
	Model::SetTransform(hShadow_, tShadow_);
	if (GetMathType(tShadow_.position_).mathType_ != MATH_HOLE &&
		playerState_ != STATE_DEAD)
	{
		Model::Draw(hShadow_);
	}
}

void Player::ShadowManagement()
{
	tShadow_.position_ = transform_.position_;
	tShadow_.position_.y = playerHeight_;

	//�e�̃X�P�[���̍ő�l
	const float shadowScaleMax = 1.0f;
	//�e�̃X�P�[�����ő�l�ɂȂ�Ƃ��̃v���C���[�̍���
	const float scaleMaxPlayerHeight = playerHeight_;
	//�e�̃X�P�[���̍ŏ��l
	const float shadowScaleMin = 0.0f;
	//�e�̃X�P�[�����ŏ��l�ɂȂ�Ƃ��̃v���C���[�̍���
	const float scaleMinPlayerHeight = 10.0f;

	
	tShadow_.scale_.x = shadowScaleMax - ((transform_.position_.y - scaleMaxPlayerHeight) / (scaleMinPlayerHeight - scaleMaxPlayerHeight));
	tShadow_.scale_.z = tShadow_.scale_.x;
}

void Player::SetAnimFramerate()
{
	nowFrame_ = Model::GetAnimFrame(hModel_);

	//���ꂼ��̃t���[�����[�g
	const int IDLE_FIRST = 1;
	const int IDLE_END = 60;
	const int WALK_FIRST = 61;
	const int WALK_END = 120;
	const int JAMP_FIRST = 121;
	const int JAMP_END = 150;
	const int FALL_FIRST = 150;
	const int FALL_END = 150;
	//�A�j���[�V�����̃X�s�[�h
	float animSpeed = 1.0f;
	//�ړ��̃A�j���[�V�����̃X�s�[�h
	float moveAnimSpeed = (float)(WALK_END - WALK_FIRST) / moveFrameNum_;
	std::string resStr = std::to_string(moveAnimSpeed) + "\n";
	OutputDebugString(resStr.c_str());
	if (prevPlayerState_ != playerState_)
	{
		switch (playerState_)
		{
		case STATE_IDLE:
			startFrame_ = IDLE_FIRST;
			endFrame_ = IDLE_END;
			break;
		case STATE_WALK:
			startFrame_ = WALK_FIRST;
			endFrame_ = WALK_END;
			animSpeed = moveAnimSpeed;
			break;
		case STATE_JUMP:
			startFrame_ = JAMP_FIRST;
			endFrame_ = JAMP_END;
			break;
		case STATE_FALL:
			startFrame_ = FALL_FIRST;
			endFrame_ = FALL_END;
			break;
		case STATE_DEAD:
			endFrame_ = startFrame_ = nowFrame_;
			break;
		}
		Model::SetAnimFrame(hModel_, startFrame_, endFrame_, animSpeed);
	}

	if (isReturn_)
	{
		startFrame_ = IDLE_FIRST;
		endFrame_ = IDLE_END;
		Model::SetAnimFrame(hModel_, startFrame_, endFrame_, animSpeed);
	}

	prevPlayerState_ = playerState_;
}

void Player::HitStopInit()
{
	pHitStopTimer_ = new Timer(hitStopTime_);
	isHitStop_ = true;
}

void Player::HitStopUpdate()
{
	if (isHitStop_)
	{
		pHitStopTimer_->Update();
		if (pHitStopTimer_->isTimeUpped())
		{
			isHitStop_ = false;
		}
	}
}

void Player::OnCollision(GameObject* pTarget)
{
	//�Ƃ��Ƃ��Ɠ���������
	if (pTarget->GetObjectName() == "Togetoge" && playerState_ != STATE_DEAD)
	{
		DamageDir(pTarget->GetTransform().position_);
		CameraShakeInit();
		//�G�t�F�N�g
		EmitterDataAssign(pTarget->GetTransform().position_);
		//�q�b�g�X�g�b�v
		HitStopInit();
		MoveFinished();
		prevPos_ = transform_.position_;
		playerState_ = STATE_DEAD;
		pStateManager_->ChangeState("STATE_DEAD");
	}
}

void Player::DamageDir(XMFLOAT3 _hitTgtgPos)
{
	//���������Ƃ��Ƃ��Ƃ̋���
	XMFLOAT3 hitDir = XMFLOAT3(abs(transform_.position_.x - _hitTgtgPos.x),
		abs(transform_.position_.y - _hitTgtgPos.y),
		abs(transform_.position_.z - _hitTgtgPos.z));

	if (hitDir.x > hitDir.y)
	{
		camShakeType = TYPE_BESIDE;
	}
	else
	{
		camShakeType = TYPE_VERTICAL;
	}
}

void Player::TimeGageManagement()
{
	//���ԃQ�[�W
	const XMFLOAT3 timerPos = XMFLOAT3(-0.8f, 0.8f, 0);
	const XMFLOAT3 timerScale = XMFLOAT3(1.0f, 0.25f, 1);

	//���ԃQ�[�W�̔����Ƃ���̃g�����X�t�H�[��
	tFrame_.position_ =
		XMFLOAT3(timerPos.x, timerPos.y, timerPos.z);
	tFrame_.scale_ = timerScale;

	//���ԃQ�[�W�̘g�̃g�����X�t�H�[��
	//���ԃQ�[�W�̘g�̑���
	float outlineThick = 0.05f;
	tFrameOutline_.position_ =
		XMFLOAT3(timerPos.x, timerPos.y, timerPos.z);
	tFrameOutline_.scale_ = XMFLOAT3(timerScale.x + outlineThick, timerScale.y + outlineThick, timerScale.z);

	//���ԃQ�[�W�̗΂̂Ƃ���̃g�����X�t�H�[��
	//���ԃQ�[�W��x�T�C�Y�̊�l
	const float scaleBasex = 4.0f;
	const float posBasex = -0.6f;
	tGage_.position_ =
		XMFLOAT3((((posBasex / (scaleBasex / timerScale.x)) / pTimer_->GetLimitTime()) * pTimer_->GetCurTime()) + timerPos.x,
			timerPos.y, timerPos.z);
	tGage_.scale_ = XMFLOAT3(float(pTimer_->GetLimitTime() - pTimer_->GetCurTime())
		 * (timerScale.x / (float(pTimer_->GetLimitTime())))
		, timerScale.y, timerScale.z);
}

void Player::EmitterDataAssign(XMFLOAT3 _hitTgtgPos)
{
	//�G�t�F�N�g�̃t�@�C���������Ă�t�H���_�[
	const std::string folderName = "Assets\\Effect\\";
	//�g���G�t�F�N�g�̃t�@�C����
	std::string fileName = "Effect_Dead.png";
	fileName = folderName + fileName;
	// �����̈ʒu�� 0
	// ���������Ƃ��Ƃ��̈ʒu�� 1 �Ƃ����Ƃ���
	// �G�t�F�N�g���o���ʒu
	const float effectPos = 0.5f;
	//������ɑ������l
	const float upDirPlus = 0.5f;
	//�G�t�F�N�g���o���ʒu
	const XMFLOAT3 deadEffectPos = 
		XMFLOAT3(transform_.position_.x + ((_hitTgtgPos.x - transform_.position_.x) * effectPos),
			transform_.position_.y + ((_hitTgtgPos.y - transform_.position_.y) * effectPos + upDirPlus),
			transform_.position_.z + ((_hitTgtgPos.z - transform_.position_.z) * effectPos));
	//�G�t�F�N�g�̕���
	const XMFLOAT3 deadEffectDir =
		XMFLOAT3(_hitTgtgPos.x - transform_.position_.x,
			_hitTgtgPos.y - transform_.position_.y,
			_hitTgtgPos.z - transform_.position_.z);
	//�G�t�F�N�g�̕����̌덷
	const XMFLOAT3 dirRnd = XMFLOAT3(0, 100, 100);
	//�p�[�e�B�N�������˂����t���[����
	const int particleDelay = 1;
	//�p�[�e�B�N���̃X�s�[�h
	const float particleSpeed = 0.5f;
	//�p�[�e�B�N���̐�������(�t���[����)
	const int particleLifeTime = 2;

	//���ꂼ����
	deadEmitData_.textureFileName = fileName;
	XMStoreFloat3(&deadEmitData_.position, XMLoadFloat3(&deadEffectPos));
	deadEmitData_.position.y += upDirPlus;
	deadEmitData_.directionRnd = dirRnd;
	XMStoreFloat3(&deadEmitData_.direction, XMVector3Normalize(XMLoadFloat3(&deadEffectDir)));
	deadEmitData_.delay = particleDelay;
	deadEmitData_.speed = particleSpeed;
	deadEmitData_.lifeTime = particleLifeTime;


	deadEmitHandle_ = VFX::Start(deadEmitData_);
}

void Player::AudioInit()
{
	//���y�������Ă�t�H���_��
	const std::string folderName = "Assets\\Audio\\";
	//SE�̃t�H���_
	const std::string SEFolder = "SE\\";
	//SE���[�h
	std::string se = folderName + SEFolder + "SE_Damage.wav";
	hSE_Damage_ = Audio::Load(se, false);
}
