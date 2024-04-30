#include "Player.h"

#include "Engine/Input.h"
#include "Engine/Camera.h"
#include "Engine/Model.h"
#include "Engine/Image.h"
#include "Engine/Fbx.h"
#include "Engine/SphereCollider.h"
#include "Engine/Easing.h"
#include "Engine/Shaker.h"

#include "Scene/PlayScene.h"
#include "Scene/SceneTransition.h"
#include "Stage.h"
#include "Timer.h"
#include "StageOrigin.h"

const int MODELSIZE = 0.8f;
//ジャンプ時の最高到達点
const float JAMPMAXHEIGHT = 2;
//移動しないとき
const XMFLOAT3 moveInit(0, 0, 0);
//前の移動距離
const XMFLOAT3 moveFront(0, 0, 1);
//後ろの移動距離
const XMFLOAT3 moveBack(0, 0, -1);
//右の移動距離
const XMFLOAT3 moveRight(1, 0, 0);
//左の移動距離
const XMFLOAT3 moveLeft(-1, 0, 0);
//moveCount_の初期値
const float moveCountInit = 0.0f;
//ジャンプ移動の通常の移動との距離の倍率
const int normalMoveVectoMult = 2;
//落ちる速度の初期値
const float fallSpeedInit = 0.0f;

Player::Player(GameObject* parent)
	: GameObject(parent, "Player"),
	hModel_(-1),
	//カメラについて
	camPos_(XMFLOAT3(0, 0, 0)),
	camTarget_(XMFLOAT3(0, 0, 0)),
	dirCamToPlayer_(XMFLOAT3(0, 0, 0)),
	//プレイヤーの操作について
	moveFinished_(false),
	moveCount_(moveCountInit),
	moveDir_(0, 0, 0), 
	destPos_(0, 0, 0), 
	prevPos_(0, 0, 0),
	gravity_(0, -0.0007f, 0), 
	gravityAcce_(gravity_.y),
	velocity_(XMVectorSet(0, 0, 0, 0)),
	eyeDirection_(XMVectorSet(0, 0, 1, 0)),
	deadHeight_(-1.0f),
	isReturn_(false),

	playerState_(STATE_IDLE), 
	prevPlayerState_(STATE_DEAD), 
	stageState_(STATE_START),

	//影について
	hShadow_(-1),

	//画像に関する変数の初期化
	hFrame_(-1), 
	hFrameOutline_(-1), 
	hGage_(-1), 
	hTime_(-1),

	//ヒットストップに関する変数の初期化
	hitStopTime_(0.2f), 
	isHitStop_(false), 

	
	hurdle_Limit_(0)
{
	pST = (SceneTransition*)FindObject("SceneTransition");
	XSIZE = (int)pST->GetMathSize_x();
	ZSIZE = (int)pST->GetMathSize_z();

	Math_Resize(XSIZE, ZSIZE, &math_);

	pPlayScene_ = (PlayScene*)FindObject("PlayScene");
	playerHeight_ = pPlayScene_->GetPlayerHeight();

	SetTableMath(pPlayScene_->GetTableMath());

	for (int x = 0; x < XSIZE; x++)
	{
		for (int z = 0; z < ZSIZE; z++)
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

	ShadowInit();

	//カメラの振動　初期化
	pCamShaker_ = new Shaker();
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

	//時間ゲージ
	TimeGageManagement();

	//影表示
	ShadowDraw();

	//プレイヤー番号表示
	pST->PlayerNumDraw();

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
	CameraPosSet();
	//コライダー
	SphereCollider* pSC = new SphereCollider(MODELSIZE / 2);
	this->AddCollider(pSC);

	//時間切れ
	pTimer_->Update();
	if (pTimer_->isTimeUpped())
	{
		stageState_ = STATE_FAILURE;
	}

	ShadowManagement();

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
	case STATE_CONVMOVE:
		ConvMoveUpdate();
		break;
	case STATE_DEAD:
		DeadUpdate();
		break;
	}
	//アニメーションを更新する
	SetAnimFramerate();
}

void Player::CameraPosSet()
{
	const XMFLOAT3 dirCamToPlayerInit = XMFLOAT3(0, 7, -5);
	//カメラ
	//プレイヤーとカメラとの距離
	dirCamToPlayer_ = dirCamToPlayerInit;
	//カメラが見ているオブジェクト
	camTarget_ = XMFLOAT3(transform_.position_.x, 0, transform_.position_.z);
	//カメラの位置
	camPos_ = XMFLOAT3(camTarget_.x + dirCamToPlayer_.x,
						camTarget_.y + dirCamToPlayer_.y,
						camTarget_.z + dirCamToPlayer_.z);
	//カメラ振動
	CameraShake();
	Camera::SetPosition(camPos_);
	Camera::SetTarget(camTarget_);
}

void Player::CameraShakeInit()
{
	//カメラの振動時間
	const float camShakeTime = 0.2f;
	//カメラの振動の強さ
	const float camShakePower = 0.5f;
	pCamShaker_->ShakeInit(&camPos_, camShakeTime, camShakePower);
	pCamShaker_->SetIsShake(true);
}

void Player::CameraShake()
{
	if (pCamShaker_->GetIsShake())
	{
		pCamShaker_->ShakeUpdate();
		camTarget_ = XMFLOAT3(camPos_.x - dirCamToPlayer_.x,
			camPos_.y - dirCamToPlayer_.y,
			camPos_.z - dirCamToPlayer_.z);
	}
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
			//移動距離
			moveDir_ = moveFront;
			playerState_ = STATE_WALK;
			if (Input::IsKey(DIK_SPACE))
			{
				playerState_ = STATE_JAMP;
			}
		}
		if (Input::IsKey(DIK_S))
		{
			//移動距離
			moveDir_ = moveBack;
			playerState_ = STATE_WALK;
			if (Input::IsKey(DIK_SPACE))
			{
				playerState_ = STATE_JAMP;
			}
		}
		if (Input::IsKey(DIK_A))
		{
			//移動距離
			moveDir_ = moveLeft;
			playerState_ = STATE_WALK;
			if (Input::IsKey(DIK_SPACE))
			{
				playerState_ = STATE_JAMP;
			}
		}
		if (Input::IsKey(DIK_D))
		{
			//移動距離
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
	//移動先が壁だったら
	if (WallCheck())
	{
		destPos_ = prevPos_;
		//移動終了
		moveFinished_ = true;
		prevPlayerState_ = STATE_IDLE;
		return;
	}
	DestPosMathType();
	//moveCountの毎秒増えていく値
	const float cntUpdate = 0.03f;
	moveCount_ += cntUpdate;
	if (moveCount_ > 1) moveCount_ = 1;

	Easing* pEasing = new Easing();
	//velocity_に入れるためのXMFLOAT3型の変数
	XMFLOAT3 vec = XMFLOAT3(0, 0, 0);
	vec.x = moveDir_.x * pEasing->EaseOutQuad(moveCount_);
	vec.y = moveDir_.y * pEasing->EaseOutCirc(moveCount_ * normalMoveVectoMult);
	vec.z = moveDir_.z * pEasing->EaseOutQuad(moveCount_);

	velocity_ = XMLoadFloat3(&vec);

	if (moveCount_ >= 1)
	{
		moveCount_ = moveCountInit;
		//移動終了
		moveFinished_ = true;
	}
	transform_.position_ = prevPos_ + velocity_;
}

void Player::ChangePlayerDir(XMVECTOR _vec)
{
	//進む方向に視線方向を合わせる
	if (XMVectorGetX(XMVector3Length(_vec)) != 0)
	{
		//上方向が0の移動ベクトル
		XMVECTOR normalVelo = XMVectorSet(XMVectorGetX(_vec), 0,
			XMVectorGetZ(_vec), 0);
		XMVECTOR v = XMVector3Dot(eyeDirection_, XMVector3Normalize(normalVelo));

		float angle = XMConvertToDegrees(acos(XMVectorGetX(v)));

		// XMVector3Cross : vFront と vMove の外積求める
		XMVECTOR vCross = XMVector3Cross(eyeDirection_, XMVector3Normalize(normalVelo));

		// vFront と vMove の外積が下向きだったら angle に-1をかけて、向きを反対にする
		if (XMVectorGetY(vCross) < 0)
		{
			angle *= -1;
		}
		transform_.rotate_.y = angle;
	}
}

void Player::ChangePlayerDir(XMFLOAT3 _vec)
{
	//引数をXMVECTOR型に変換する
	XMVECTOR velo;
	velo = XMLoadFloat3(&_vec);
	ChangePlayerDir(velo);
}

MATHTYPE Player::DestPosMathType()
{
	MATHTYPE retType;
	//進んだ先の位置
	destPos_.x = prevPos_.x + moveDir_.x;
	destPos_.z = prevPos_.z + moveDir_.z;
	retType = GetMathType(destPos_).mathType_;

	return retType;
}

bool Player::WallCheck()
{
	bool ret = false;
	//上方向がないmoveDir_
	XMFLOAT2 vec2MoveDir;
	vec2MoveDir.x = moveDir_.x;
	vec2MoveDir.y = moveDir_.z;
	//moveDir_の長さ
	float moveDirSize = XMVectorGetX(XMVector2Length(XMLoadFloat2(&vec2MoveDir)));

	for (int i = 1; i <= moveDirSize; i++)
	{
		//moveDir_を正規化する変数
		XMFLOAT2 normalMoveDir;
		XMStoreFloat2(&normalMoveDir, XMVector2Normalize(XMLoadFloat2(&vec2MoveDir)));
		destPos_.x = prevPos_.x + normalMoveDir.x * i;
		destPos_.z = prevPos_.z + normalMoveDir.y * i;
		if (GetMathType(destPos_).mathType_ == MATH_WALL)
		{
			ret = true;
		}
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
	//playerの操作
	PlayerOperation();
	//立っているマスの効果
	MathTypeEffect();
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

void Player::JampUpdate()
{
	ChangePlayerDir(moveDir_);
	
	//moveDir_を正規化するための変数
	XMFLOAT3 normalMoveDir = XMFLOAT3(moveDir_.x, 0, moveDir_.z);
	XMVECTOR dir;
	//moveDir_をXMVECTOR型に変換
	dir = XMLoadFloat3(&normalMoveDir);
	//正規化
	dir = XMVector3Normalize(dir);
	//dirをXMFLOAT3型に変換
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
		playerState_ = STATE_IDLE;
		if (DestPosMathType() == MATH_HOLE && transform_.position_.y <= deadHeight_)
		{
			playerState_ = STATE_DEAD;
		}
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

void Player::DeadUpdate()
{
	if (!pCamShaker_->GetIsShake())
	{
		ReturnToStartMath();
	}
}

void Player::ReturnToStartMath()
{
	isReturn_ = true;
	moveDir_.x = startPos_.x - prevPos_.x;
	moveDir_.y = 0;
	moveDir_.z = startPos_.z - prevPos_.z;
	PlayerMove();
	//スタートマスに戻るときのプレイヤーの高さ
	const float returnHeight = 10;
	transform_.position_.y = returnHeight;
	if (moveFinished_)
	{
		isReturn_ = false;
		stageState_ = STATE_START;
	}
}

void Player::MathTypeEffect()
{
	//コンベアによって移動する方向
	const XMVECTOR converyor_velocity = XMVectorSet(-1.0f, 0, 0, 0);
	//自分の立っているマスの情報
	standMath_ = GetMathType(transform_.position_);
	switch (standMath_.mathType_)
	{
	case MATH_CONVEYOR:
		//立っているコンベアの向いている方向を入れる回転行列
		XMMATRIX yrot = XMMatrixRotationY(XMConvertToRadians(-standMath_.mathPos_.rotate_.z));
		//上の回転行列でconveryor_velocityの変える
		XMVECTOR rotConvVec = XMVector3Transform(converyor_velocity, yrot);
		//移動方向を上のベクトルにする
		XMStoreFloat3(&moveDir_, rotConvVec);
		playerState_ = STATE_CONVMOVE;
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

void Player::ShadowInit()
{
	//影のモデルのファイルネーム
	std::string modelName = "Player_Shadow.fbx";
	//モデルの入ってるフォルダ名
	std::string folderName = "Assets\\";
	modelName = folderName + modelName;

	//モデルロード
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
}

void Player::SetAnimFramerate()
{
	nowFrame_ = Model::GetAnimFrame(hModel_);

	//移動量がゼロだったら
	if (XMVectorGetX(XMVector3Length(velocity_)) <= 0)
	{
		return;
	}
	//それぞれのフレームレート
	const int IDLE_FIRST = 1;
	const int IDLE_END = 60;
	const int WALK_FIRST = 61;
	const int WALK_END = 120;
	const int JAMP_FIRST = 121;
	const int JAMP_END = 150;
	const int FALL_FIRST = 150;
	const int FALL_END = 150;
	//アニメーションのスピード
	const int animSpeed = 1;
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
			break;
		case STATE_JAMP:
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

void Player::OnCollision(GameObject* pTarget)
{
	//とげとげと当たったら
	if (pTarget->GetObjectName() == "Togetoge" && playerState_ != STATE_DEAD)
	{
		CameraShakeInit();
		moveCount_ = moveCountInit;
		prevPos_ = transform_.position_;
		playerState_ = STATE_DEAD;
		isHitStop_ = true;
	}
}

void Player::TimeGageManagement()
{
	//時間ゲージ
	const XMFLOAT3 timerPos = XMFLOAT3(-0.6f, 0.8f, 0);
	const XMFLOAT3 timerScale = XMFLOAT3(2.0f, 0.5f, 1);

	//時間ゲージの白いところのトランスフォーム
	tFrame_.position_ =
		XMFLOAT3(timerPos.x, timerPos.y, timerPos.z);
	tFrame_.scale_ = timerScale;

	//時間ゲージの枠のトランスフォーム
	//時間ゲージの枠の太さ
	float outlineThick = 0.05f;
	tFrameOutline_.position_ =
		XMFLOAT3(timerPos.x, timerPos.y, timerPos.z);
	tFrameOutline_.scale_ = XMFLOAT3(timerScale.x + outlineThick, timerScale.y + outlineThick, timerScale.z);

	//時間ゲージの緑のところのトランスフォーム
	tGage_.position_ =
		XMFLOAT3((((timerPos.x / 2) / pTimer_->GetLimitTime()) * pTimer_->GetCurTime()) + timerPos.x,
			timerPos.y, timerPos.z);
	tGage_.scale_ = XMFLOAT3(float(pTimer_->GetLimitTime() - pTimer_->GetCurTime()) /
		100 * (timerScale.x / (float(pTimer_->GetLimitTime()) / 100))
		, timerScale.y, timerScale.z);
}
