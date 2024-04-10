#pragma once
#include "Engine/GameObject.h"
#include "StageOrigin.h"

class PlayScene;
class Timer;

enum PLAYER_STATE
{
	STATE_IDLE = 0,
	STATE_WALK,
	STATE_JAMP,
	STATE_FALL,
	STATE_DEAD,
};

enum STAGE_STATE
{
	STATE_START = 0,
	STATE_PLAY,
	STATE_GOAL,
	STATE_FAILURE,
};


class Player
	:public GameObject, StageOrigin
{
	int hModel_;
	//時間ゲージの画像
	int hFrame_, hGage_, hFrameOutline_;
	int hTime_;
	//時間ゲージの画像のトランスフォーム
	Transform tFrame_, tGage_, tFrameOutline_;
	Transform tTime_;

	//クリアしたか
	bool isGoal_;
	//ヒットストップの時間
	float hitStopTime_;
	bool isHitStop_;

	PlayScene* pPlayScene_;

	//移動が終了したか
	bool moveFinished_;
	float moveCount_;
	//上方向の毎フレーム足される値
	float upVecPlus_;
	//移動方向
	XMFLOAT3 moveDir_;
	//移動先の位置
	XMFLOAT3 destPos_;
	//移動前の位置
	XMFLOAT3 prevPos_;
	//移動ベクトル
	XMVECTOR velocity_;
	//重力
	XMFLOAT3 gravity_;
	//重力加速度
	float gravityAcce_;
	//視線ベクトル
	XMVECTOR eyeDirection_;


	XMFLOAT3 tableHitPoint_;
	bool isTableHit_;

	XMFLOAT3 startPos_, goalPos_;
	XMFLOAT3 centerPos_;
	XMFLOAT3 camRot_;

	//プレイヤーの状態
	PLAYER_STATE playerState_;
	//プレイヤーの直前の状態
	PLAYER_STATE prevPlayerState_;
	//ゲームの進行度の状態
	STAGE_STATE stageState_;
	//アニメーションのフレーム
	int nowFrame_, startFrame_, endFrame_;

	//立っているマスの情報
	MATHDEDAIL standMath_;

	

	//障害物の置ける数
	int hurdle_Limit_;

	//タイマー
	Timer* pTimer_;

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

	void PlayUpdate();
	void CameraPosSet();
	//プレイヤーの操作に関する関数
	void PlayerOperation();
	//移動
	void PlayerMove();
	//移動先のマスの種類
	MATHTYPE DestPosMathType();

	//それぞれの状態のUpdate
	void IdleUpdate();
	void WalkUpdate();
	void JampUpdate();
	void FallUpdate();
	void DeadUpdate();

	//スタートマスに戻す
	void ReturnToStartMath();

	//自分の立っているマスの効果
	void MathTypeEffect();

	//ヒットストップ
	float GetHitStopTime() { return hitStopTime_; }
	bool GetHitStop() { return isHitStop_; }
	void SetHitStop(bool _ishit) { isHitStop_ = _ishit; }

	//アニメーションのフレームレート
	void SetAnimFramerate();

	//playerがステージ内に入っているか
	bool Is_InSide_Table(XMFLOAT3 _pos);

	XMFLOAT3 GetPosition() { return centerPos_; }

	//戻り値 : 引数の位置のマスの種類
	MATHDEDAIL GetMathType(XMFLOAT3 _pos);

	PLAYER_STATE GetPlayerState() { return playerState_; }
	STAGE_STATE GetStageState() { return stageState_; }

	//当たり判定
	void OnCollision(GameObject* pTarget) override;

	//時間ゲージ
	void TimeGageManagement();
};