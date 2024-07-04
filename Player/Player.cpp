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
//moveCntUpdate_の初期値
const float moveCntUpdateInit = 0.03f;
//moveCount_の移動が終了する値
const float moveCountEnd = 1.0f;
//ジャンプ移動の通常の移動との距離の倍率
const int normalMoveVectoMult = 2;
//落ちる速度の初期値
const float fallSpeedInit = 0.0f;
//カメラの振動の種類
SHAKETYPE camShakeType;
//カメラの振動の強さ
const float camShakePower = 2.0f;
//制限時間減らすマスの減る時間
const float decTime = 5.0f;
//混乱マス踏んだ時のアニメーションの長さ(秒)
const float confAnimTime = 2;
//同時押しの許可フレーム
const int simulPressFrame = 5;

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

	//操作説明について
	hOpeExplanBackGround_(-1),

	//影について
	hShadow_(-1),

	//画像に関する変数の初期化
	hFrame_(-1),
	hFrameOutline_(-1),
	hGage_(-1),
	hTime_(-1),
	gameTime_(30),

	//ヒットストップに関する変数の初期化
	hitStopTime_(0.2f),
	isHitStop_(false),
	pHitStopTimer_(nullptr),
	isCamShake_(true),

	//アニメーションに関する変数
	//移動にかかるフレーム数
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

	//操作説明の背景ロード
	hOpeExplanBackGround_ = Image::Load("Assets\\opeBackGround.png");
	assert(hOpeExplanBackGround_ >= 0);
	
	pTimer_ = new Timer(gameTime_);

	ShadowInit();
	//音楽初期化
	AudioInit();
	//ステート初期化
	StateInit();

	//カメラの振動　初期化
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

	//時間ゲージ
	TimeGageManagement();

	//影表示
	ShadowDraw();

	Image::SetTransform(hGage_, tGage_);
	Image::SetTransform(hFrame_, tFrame_);
	Image::SetTransform(hFrameOutline_, tFrameOutline_);
	Image::Draw(hGage_);
	Image::Draw(hFrame_);
	Image::Draw(hFrameOutline_);

	//時間ゲージとの距離
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

	pStateManager_->Update();
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
	if (!isHitStop_)
	{
		Camera::SetPosition(camPos_);
		Camera::SetTarget(camTarget_);
	}
	//カメラ振動
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
	//ステートマネージャーにステート追加
	pStateManager_ = new StateManager(this);

	pStateManager_->AddState("STATE_IDLE", new PlayerIdleState(pStateManager_));
	pStateManager_->AddState("STATE_WALK", new PlayerWalkState(pStateManager_));
	pStateManager_->AddState("STATE_JUMP", new PlayerJumpState(pStateManager_));
	pStateManager_->AddState("STATE_FALL", new PlayerFallState(pStateManager_));
	pStateManager_->AddState("STATE_CONVMOVE", new PlayerConvMoveState(pStateManager_));
	pStateManager_->AddState("STATE_CONF", new PlayerConfState(pStateManager_));
	pStateManager_->AddState("STATE_DEAD", new PlayerDeadState(pStateManager_));

	//最初のステート決める
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
	//操作説明を表示する基準の位置
	const XMFLOAT2 opePos = XMFLOAT2(50, 200);

	//一行の縦の長さ
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

	//移動方向表示
	//Wを押したときの移動方向
	moveDirStr_[DIR_W] = "W - " + moveDirStr_[DIR_W];
	//Sを押したときの移動方向
	moveDirStr_[DIR_S] = "S - " + moveDirStr_[DIR_S];
	//Aを押したときの移動方向
	moveDirStr_[DIR_A] = "A - " + moveDirStr_[DIR_A];
	//Wを押したときの移動方向
	moveDirStr_[DIR_D] = "D - " + moveDirStr_[DIR_D];

	for (int i = 0; i < DIR_MAX; i++)
	{
		pText_->Draw(opePos.x, opePos.y + lineHeight * i, moveDirStr_[i].c_str());
	}
}

void Player::PlayerOperation()
{
	if (playerState_ != STATE_DEAD){
		//ボタンが押されてからのフレーム数
		static int pushButtonFrame = 0;
		//前後左右移動
		if (Input::IsKeyDown(DIK_W)){
			//移動距離
			pushButton_ = DIR_W;
			pushButtonFrame = 0;
		}
		if (Input::IsKeyDown(DIK_S)){
			//移動距離
			pushButton_ = DIR_S;
			pushButtonFrame = 0;
		}
		if (Input::IsKeyDown(DIK_D)) {
			//移動距離
			pushButton_ = DIR_D;
			pushButtonFrame = 0;
		}
		if (Input::IsKeyDown(DIK_A)){
			//移動距離
			pushButton_ = DIR_A;
			pushButtonFrame = 0;
		}	
		if (Input::IsKey(DIK_SPACE)) {
			isPushSpace_ = true;
		}

		//同時押しされたか判定
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
	//移動先が壁だったら
	if (WallCheck())
	{
		//移動終了
		destPos_ = prevPos_;
		moveFinished_ = true;
		return;
	}

	//moveCountの毎秒増えていく値
	const float cntUpdate = 0.03f;
	moveCount_ += cntUpdate;
	if (moveCount_ > moveCountEnd) moveCount_ = moveCountEnd;

	Easing* pEasing = new Easing();
	//velocity_に入れるためのXMFLOAT3型の変数
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
	//進む方向に視線方向を合わせる
	if (XMVectorGetX(XMVector3Length(_vec)) != 0)
	{
		//上方向が0の移動ベクトル
		XMVECTOR normalVelo = XMVectorSet(XMVectorGetX(_vec), 0, XMVectorGetZ(_vec), 0);
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
	//立っているマスの効果
	MathTypeEffect();
	//playerの操作
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
	//0～移動可能方向の数までの乱数取得
	int random1 = (rand() % (int)DIR_MAX);
	int random2 = (rand() % (int)DIR_MAX);
	//入れ替え
	tmp = pushButtonMoveDir_[random1];
	pushButtonMoveDir_[random1] = pushButtonMoveDir_[random2];
	pushButtonMoveDir_[random2] = tmp;

	//混乱のアニメーション
	//混乱のアニメーションをしていなければ
	//何回転するか
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
	//スタートマスに戻るときのプレイヤーの高さ
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
	//移動終了
	moveFinished_ = true;
	isConfAnim_ = false;
}

void Player::MathTypeEffect()
{
	Stage* pStage = pPlayScene_->GetStagePointer();
	//コンベアによって移動する方向
	const XMVECTOR converyor_velocity = XMVectorSet(-1.0f, 0, 0, 0);

	//自分の立っているマスの情報
	standMath_ = GetMathType(transform_.position_);
	switch (standMath_.mathType_)
	{
	case MATH_CONVEYOR:
		//コンベアに連続して乗っているフレーム数が
		//コンベアマスに乗った時操作できるフレーム数を
		//上回らない限りコンベアに乗ってる判定にしない
		onConvFrames_++;
		if (onConvFrames_ < operableFrames_) break;
		onConvFrames_ = 0;

		//立っているコンベアの向いている方向を入れる回転行列
		XMMATRIX yrot = XMMatrixRotationY(XMConvertToRadians(-standMath_.mathPos_.rotate_.z));
		//上の回転行列でconveryor_velocityの変える
		XMVECTOR rotConvVec = XMVector3Transform(converyor_velocity, yrot);
		//移動方向を上のベクトルにする
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
		//制限時間減らす
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

	//影のスケールの最大値
	const float shadowScaleMax = 1.0f;
	//影のスケールが最大値になるときのプレイヤーの高さ
	const float scaleMaxPlayerHeight = playerHeight_;
	//影のスケールの最小値
	const float shadowScaleMin = 0.0f;
	//影のスケールが最小値になるときのプレイヤーの高さ
	const float scaleMinPlayerHeight = 10.0f;

	
	tShadow_.scale_.x = shadowScaleMax - ((transform_.position_.y - scaleMaxPlayerHeight) / (scaleMinPlayerHeight - scaleMaxPlayerHeight));
	tShadow_.scale_.z = tShadow_.scale_.x;
}

void Player::SetAnimFramerate()
{
	nowFrame_ = Model::GetAnimFrame(hModel_);

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
	float animSpeed = 1.0f;
	//移動のアニメーションのスピード
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
	//とげとげと当たったら
	if (pTarget->GetObjectName() == "Togetoge" && playerState_ != STATE_DEAD)
	{
		DamageDir(pTarget->GetTransform().position_);
		CameraShakeInit();
		//エフェクト
		EmitterDataAssign(pTarget->GetTransform().position_);
		//ヒットストップ
		HitStopInit();
		MoveFinished();
		prevPos_ = transform_.position_;
		playerState_ = STATE_DEAD;
		pStateManager_->ChangeState("STATE_DEAD");
	}
}

void Player::DamageDir(XMFLOAT3 _hitTgtgPos)
{
	//当たったとげとげとの距離
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
	//時間ゲージ
	const XMFLOAT3 timerPos = XMFLOAT3(-0.8f, 0.8f, 0);
	const XMFLOAT3 timerScale = XMFLOAT3(1.0f, 0.25f, 1);

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
	//時間ゲージのxサイズの基準値
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
	//エフェクトのファイルが入ってるフォルダー
	const std::string folderName = "Assets\\Effect\\";
	//使うエフェクトのファイル名
	std::string fileName = "Effect_Dead.png";
	fileName = folderName + fileName;
	// 自分の位置を 0
	// 当たったとげとげの位置を 1 としたときの
	// エフェクトを出す位置
	const float effectPos = 0.5f;
	//上方向に足される値
	const float upDirPlus = 0.5f;
	//エフェクトを出す位置
	const XMFLOAT3 deadEffectPos = 
		XMFLOAT3(transform_.position_.x + ((_hitTgtgPos.x - transform_.position_.x) * effectPos),
			transform_.position_.y + ((_hitTgtgPos.y - transform_.position_.y) * effectPos + upDirPlus),
			transform_.position_.z + ((_hitTgtgPos.z - transform_.position_.z) * effectPos));
	//エフェクトの方向
	const XMFLOAT3 deadEffectDir =
		XMFLOAT3(_hitTgtgPos.x - transform_.position_.x,
			_hitTgtgPos.y - transform_.position_.y,
			_hitTgtgPos.z - transform_.position_.z);
	//エフェクトの方向の誤差
	const XMFLOAT3 dirRnd = XMFLOAT3(0, 100, 100);
	//パーティクルが発射されるフレーム数
	const int particleDelay = 1;
	//パーティクルのスピード
	const float particleSpeed = 0.5f;
	//パーティクルの生存時間(フレーム数)
	const int particleLifeTime = 2;

	//それぞれ代入
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
	//音楽が入ってるフォルダ名
	const std::string folderName = "Assets\\Audio\\";
	//SEのフォルダ
	const std::string SEFolder = "SE\\";
	//SEロード
	std::string se = folderName + SEFolder + "SE_Damage.wav";
	hSE_Damage_ = Audio::Load(se, false);
}
