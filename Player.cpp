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
//前の移動距離
const XMFLOAT3 moveFront(0, 0, 1);
//後ろの移動距離
const XMFLOAT3 moveBack(0, 0, -1);
//右の移動距離
const XMFLOAT3 moveRight(1, 0, 0);
//左の移動距離
const XMFLOAT3 moveLeft(-1, 0, 0);

Player::Player(GameObject* parent)
	: GameObject(parent, "Player"),
	hModel_(-1),
	moveVec_(0, 0, 0), destPos_(0, 0, 0), prevPos_(0, 0, 0),
	velocity_(XMVectorSet(0, 0, 0, 0)),
	eyeDirection_(XMVectorSet(0, 0, 1, 0)),
	camRot_(0, 0, 0), gravity_(0, 0, 0), 
	playerState_(STATE_IDLE), prevPlayerState_(STATE_DEAD), stageState(STATE_START),
	hurdle_Limit_(0),
	tableHitPoint_(XMFLOAT3(0, 0, 0)), isTableHit_(false),
	hFrame_(-1), hFrameOutline_(-1), hGage_(-1), hTime_(-1),
	isGoal_(false),
	hitStopTime_(0.2f), isHitStop_(false)
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
	
	switch (stageState)
	{
	case STATE_START:
		transform_.position_ = startPos_;
		prevPos_ = transform_.position_;
		destPos_ = transform_.position_;
		stageState = STATE_PLAY;
		playerState_ = STATE_IDLE;
		break;
	case STATE_PLAY:
		PlayUpdate();
		break;
	case STATE_GOAL:
		isGoal_ = true;
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
	//カメラ
	//プレイヤーとカメラとの距離
	const XMFLOAT3 dirCamToPlayer = XMFLOAT3(0, 5, -5);
	//カメラの位置
	const XMFLOAT3 camPos = XMFLOAT3(transform_.position_.x + dirCamToPlayer.x,
									 transform_.position_.y + dirCamToPlayer.y,
									 transform_.position_.z + dirCamToPlayer.z);
	Camera::SetPosition(camPos);
	Camera::SetTarget(transform_.position_);
	//コライダー
	SphereCollider* pSC = new SphereCollider(MODELSIZE / 2);
	this->AddCollider(pSC);

	//時間切れ
	pTimer_->Update();
	if (pTimer_->isTimeUpped())
	{
		stageState = STATE_FAILURE;
	}

	SetAnimFramerate();

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

	if (playerState_ != STATE_DEAD)
	{
		//コンベアによって移動する方向
		XMVECTOR converyor_velocity = XMVectorSet(-1.0f, 0, 0, 0);
		standMath_ = GetMathType(transform_.position_);
		switch (standMath_.mathType_)
		{
		case MATH_CONVEYOR:
			XMMATRIX yrot = XMMatrixRotationY(XMConvertToRadians(-standMath_.mathPos_.rotate_.z));
			converyor_velocity = XMVector3Transform(converyor_velocity, yrot);	//その回転でベクトルの向きを変える
			converyor_velocity = converyor_velocity * 0.04f;
			if (playerState_ == STATE_WALK || playerState_ == STATE_IDLE)		velocity_ += converyor_velocity;
			break;
		case MATH_GOAL:
			stageState = STATE_GOAL;
			break;
		case MATH_HOLE:
			playerState_ = STATE_FALL;
			break;
		default:break;
		}
	}

	transform_.position_ = prevPos_ + velocity_;
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
		//前後左右移動
		if (Input::IsKey(DIK_W))
		{
			PlayerMoveFront();
		}
		if (Input::IsKey(DIK_S))
		{
			PlayerMoveBack();
		}
		if (Input::IsKey(DIK_A))
		{
			PlayerMoveLeft();
		}
		if (Input::IsKey(DIK_D))
		{
			PlayerMoveRight();
		}
	}
}

void Player::PlayerMoveFront()
{
	//移動距離
	moveVec_ = moveFront;
	//進んだ先の位置
	destPos_ = XMFLOAT3(transform_.position_.x + moveVec_.x,
						transform_.position_.y + moveVec_.y,
						transform_.position_.z + moveVec_.z);
	standMath_ = GetMathType(destPos_);
	prevPos_ = transform_.position_;
	if (standMath_.mathType_ == MATH_WALL)
	{
		return;
	}
	playerState_ = STATE_WALK;
}

void Player::PlayerMoveBack()
{
}

void Player::PlayerMoveRight()
{
}

void Player::PlayerMoveLeft()
{
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
	velocity_ = XMVectorSet(0, 0, 0, 0);
	PlayerOperation();
}

void Player::WalkUpdate()
{
	//moveCountの初期値
	const float cntInit = 0;
	//moveCountの毎秒増えていく値
	const float cntUpdate = 0.03f;
	static float moveCount = cntInit;
	moveCount += cntUpdate;
	
	Easing* pEasing = new Easing();
	//velocity_に入れるためのXMFLOAT3型の変数
	XMFLOAT3 vec;
	vec.x = moveVec_.x * pEasing->EaseInSine(moveCount);
	vec.y = moveVec_.y * pEasing->EaseInSine(moveCount);
	vec.z = moveVec_.z * pEasing->EaseInSine(moveCount);

	velocity_ = XMLoadFloat3(&vec);
	//進む方向に視線方向を合わせる
	if (XMVectorGetX(XMVector3Length(velocity_)) != 0)
	{
		XMVECTOR v = XMVector3Dot(eyeDirection_, XMVector3Normalize(velocity_));

		float angle = XMConvertToDegrees(acos(XMVectorGetX(v)));

		// XMVector3Cross : vFront と vMove の外積求める
		XMVECTOR vCross = XMVector3Cross(eyeDirection_, XMVector3Normalize(velocity_));

		// vFront と vMove の外積が下向きだったら angle に-1をかけて、向きを反対にする
		if (XMVectorGetY(vCross) < 0)
		{
			angle *= -1;
		}
		transform_.rotate_.y = angle;
	}
	float a = pEasing->EaseInSine(moveCount);
	if ((int)a)
	{
		moveCount = 0;
		transform_.position_ = destPos_;
		playerState_ = STATE_IDLE;
	}

}

void Player::JampUpdate()
{
	gravity_.y = 0.2f;
	if (transform_.position_.y >= 1.5f)
	{
		playerState_ = STATE_FALL;
	}
}

void Player::FallUpdate()
{
	gravity_.y += -0.01f;
	if (transform_.position_.y < 1.0f && !isTableHit_)
	{
		isTableHit_ = true;
		tableHitPoint_ = transform_.position_;
	}
	if (isTableHit_)
	{
		if (GetMathType(tableHitPoint_).mathType_ != (int)MATH_HOLE)
		{
			playerState_ = STATE_WALK;
			return;
		}
	}
	if (transform_.position_.y < -1.0f)
	{
		ReturnToStartMath();
		return;
	}
}

void Player::DeadUpdate()
{

	ReturnToStartMath();
}

void Player::ReturnToStartMath()
{
	playerState_ = STATE_DEAD;
	if (abs(startPos_.x - transform_.position_.x) <= 0.01f &&
		abs(startPos_.z - transform_.position_.z) <= 0.01f)
	{
		stageState = STATE_START;
	}
	transform_.position_.x = transform_.position_.x + (startPos_.x - transform_.position_.x) / 10;
	transform_.position_.z = transform_.position_.z + (startPos_.z - transform_.position_.z) / 10;
	transform_.position_.y = 10;
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
	if (prevPlayerState_ != playerState_)
	{
		Model::SetAnimFrame(hModel_, startFrame_, endFrame_, 1);
	}
	prevPlayerState_ = playerState_;
}

void Player::OnCollision(GameObject* pTarget)
{
	if (pTarget->GetObjectName() == "Togetoge")
	{
		playerState_ = STATE_DEAD;
		isHitStop_ = true;
	}
}

void Player::TimeGageManagement()
{
	//時間ゲージ
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
