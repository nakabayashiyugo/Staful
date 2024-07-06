#pragma once
#include "../Engine/GameObject.h"

#include "../StageOrigin.h"

#include "../Engine/VFX.h"

class PlayScene;
class Timer;
class Shaker;
class Text;
class StateManager;

enum PLAYER_STATE
{
	STATE_IDLE = 0,	//止まってるとき
	STATE_WALK,		//歩いているとき
	STATE_JUMP,		//ジャンプしたとき
	STATE_FALL,		//落下しているとき
	STATE_CONVMOVE,	//コンベアによって移動しているとき
	STATE_CONF,		//混乱マスに乗ってくるくるするとき
	STATE_DEAD,		//死んだとき
};

enum STAGE_STATE
{
	STATE_START = 0,
	STATE_PLAY,
	STATE_GOAL,
	STATE_FAILURE,
};

enum PLAYER_MOVEDIR
{
	DIR_W = 0,
	DIR_S,
	DIR_D,
	DIR_A,
	DIR_MAX,
};

class Player
	:public GameObject, StageOrigin
{
	int hModel_;

	PlayScene* pPlayScene_;
public:
	Player(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//当たり判定
	void OnCollision(GameObject* pTarget) override;

//カメラについて
private:
	//カメラの位置
	XMFLOAT3 camPos_;
	//カメラの見るオブジェクト
	XMFLOAT3 camTarget_;
	//カメラの見るオブジェクトとカメラの距離
	XMFLOAT3 dirCamToPlayer_;

	Shaker* pCamShaker_;
public:
	//ダメージを受けた方向
	void DamageDir(XMFLOAT3 _hitTgtgPos);
	//カメラの位置
	void CameraPosSet();
	//カメラの振動の初期化
	void CameraShakeInit();
	//カメラの振動
	void CameraShake();


//プレイヤーの操作について
private:
	//スタート位置、ゴール位置
	XMFLOAT3 startPos_, goalPos_;
	//基準の高さ
	float playerHeight_;
	//移動が終了したか
	bool moveFinished_;
	float moveCount_;
	//moveCount_の毎フレームたされる値
	float moveCntUpdate_;
	//落ちる速度
	float fallSpeed_;
	//スペースボタンが押されたか
	bool isPushSpace_;
	//wasdを押したときの移動方向
	PLAYER_MOVEDIR pushButtonMoveDir_[DIR_MAX];
	//移動が可能な方向
	XMFLOAT3 possiMoveDir_[DIR_MAX]; 
	//何ボタン押されたか
	PLAYER_MOVEDIR pushButton_;
	//移動方向
	XMFLOAT3 moveDir_;
	//移動先の位置
	XMFLOAT3 destPos_;
	//移動前の位置
	XMFLOAT3 prevPos_;
	//重力
	XMFLOAT3 gravity_;
	//重力加速度
	float gravityAcce_;
	//移動ベクトル
	XMVECTOR velocity_;
	//視線ベクトル
	XMVECTOR eyeDirection_;
	//落ちた時の死ぬ高さ
	float deadHeight_;
	//スタートマスに戻っているか
	bool isReturn_;
	//コンベアマスに乗った時操作できるフレーム数
	int operableFrames_;
	//コンベアマスに何フレーム連続して乗っているか
	int onConvFrames_;

	//立っているマスの情報
	MATHDEDAIL standMath_;
	//直前までたっていたマスの情報
	MATHDEDAIL prevStandMath_;

	StateManager* pStateManager_;
	//プレイヤーの状態
	PLAYER_STATE playerState_;
	//プレイヤーの直前の状態
	PLAYER_STATE prevPlayerState_;

	//ゲームの進行度の状態
	STAGE_STATE stageState_;

	//今立ってるマスで混乱のアニメーションを終えているか
	bool isConfAnim_;
	//混乱マスに乗った時の最初のrotateの値
	XMFLOAT3 prevRot_;


public:
	void PlayUpdate();
	//プレイヤーの操作に関する関数
	void PlayerOperation();
	//移動
	void PlayerMove();
	//プレイヤーを引数の方向に向かせる
	void ChangePlayerDir(XMVECTOR _vec);
	void ChangePlayerDir(XMFLOAT3 _vec);
	//戻り値 : 引数の位置のマスの種類
	MATHDEDAIL GetMathType(XMFLOAT3 _pos);
	//移動先のマスの種類
	MATHTYPE DestPosMathType();
	//移動先との間に壁があるか
	bool WallCheck();

	//それぞれの状態のUpdate
	void IdleUpdate();
	void WalkUpdate();
	void JumpUpdate();
	void FallUpdate();
	void ConvMoveUpdate();
	void ConfUpdate();
	void DeadUpdate();
	//スタートマスに戻す
	void ReturnToStartMath();

	//移動が終了したときの処理
	void MoveFinished();

	//自分の立っているマスの効果
	void MathTypeEffect();
	//playerがステージ内に入っているか
	bool Is_InSide_Table(XMFLOAT3 _pos);
	//ステート初期化
	void StateInit();

	//プレイヤーのポジションのゲッター
	XMFLOAT3 GetPosition() { return transform_.position_; }
	//standMath_のゲッター
	MATHDEDAIL GetStandMath() { return standMath_; }
	//プレイヤーのステートのゲッター・セッター
	PLAYER_STATE GetPlayerState() { return playerState_; }
	void SetPlayerState(PLAYER_STATE _state) { playerState_ = _state; }
	//ゲームの進行度のステートのゲッター
	STAGE_STATE GetStageState() { return stageState_; }

	void PossiMoveDirInit();
	

//操作説明について
private:
	//移動方向を文字として入れた配列
	std::string moveDirStr_[DIR_MAX];

	//操作説明を表示されている場所の背景
	int hOpeExplanBackGround_;

	Text* pText_;

public:
	//操作説明の描画
	void OperateExplanDraw();


//影について
private:
	//影のモデル番号
	int hShadow_;
	//影のトランスフォーム
	Transform tShadow_;

public:
	void ShadowInit();
	void ShadowDraw();
	//影の動きの制御
	void ShadowManagement();

//プレイヤーのアニメーションについて
private:
	//アニメーションのフレーム
	int nowFrame_, startFrame_, endFrame_;
	//移動にかかるフレーム数
	int moveFrameNum_;
public:
	//アニメーションのフレームレート
	void SetAnimFramerate();


//ヒットストップについて
private:
	//ヒットストップの時間
	float hitStopTime_;
	bool isHitStop_;

	Timer* pHitStopTimer_;

	//ヒットストップ時カメラを振動させるか
	bool isCamShake_;
public:
	//ヒットストップの初期化
	void HitStopInit();
	//ヒットストップ中の処理
	void HitStopUpdate();


//時間ゲージについて
private:
	//制限時間
	int gameTime_;
	//時間ゲージの画像
	int hFrame_, hGage_, hFrameOutline_;
	int hTime_;
	//時間ゲージの画像のトランスフォーム
	Transform tFrame_, tGage_, tFrameOutline_;
	Transform tTime_;
	//タイマー
	Timer* pTimer_;
public:
	//時間ゲージ
	void TimeGageManagement();


//エフェクトについて
private:
	//とげとげと当たった時に出るエフェクトに関するデータ
	EmitterData deadEmitData_;
	//とげとげと当たった時に出るエフェクトに関するデータのハンドル
	int deadEmitHandle_;

public:
	//とげとげと当たった時に出るエフェクトの代入
	//引数 : 当たったとげとげの位置
	void EmitterDataAssign(XMFLOAT3 _hitTgtgPos);


//音楽について
private:
	//とげとげと当たった時のSEのハンドル
	int hSE_Damage_;
public:
	//音の初期化
	void AudioInit();
};