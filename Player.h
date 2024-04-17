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

	PlayScene* pPlayScene_;

	XMFLOAT3 tableHitPoint_;
	bool isTableHit_;

	XMFLOAT3 centerPos_;

	//障害物の置ける数
	int hurdle_Limit_;
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


//プレイヤーの操作について
private:
	//スタート位置、ゴール位置
	XMFLOAT3 startPos_, goalPos_;
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
	//重力
	XMFLOAT3 gravity_;
	//重力加速度
	float gravityAcce_;
	//移動ベクトル
	XMVECTOR velocity_;
	//視線ベクトル
	XMVECTOR eyeDirection_;

	//立っているマスの情報
	MATHDEDAIL standMath_;
	//プレイヤーの状態
	PLAYER_STATE playerState_;
	//プレイヤーの直前の状態
	PLAYER_STATE prevPlayerState_;
	//ゲームの進行度の状態
	STAGE_STATE stageState_;
public:
	void PlayUpdate();
	//カメラの位置
	void CameraPosSet();
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
	//playerがステージ内に入っているか
	bool Is_InSide_Table(XMFLOAT3 _pos);

	//プレイヤーのポジションのゲッター
	XMFLOAT3 GetPosition() { return centerPos_; }
	//プレイヤーのステートのゲッター
	PLAYER_STATE GetPlayerState() { return playerState_; }
	//ゲームの進行度のステートのゲッター
	STAGE_STATE GetStageState() { return stageState_; }


//プレイヤーのアニメーションについて
private:
	//アニメーションのフレーム
	int nowFrame_, startFrame_, endFrame_;
public:
	//アニメーションのフレームレート
	void SetAnimFramerate();


//ヒットストップについて
private:
	//ヒットストップの時間
	float hitStopTime_;
	bool isHitStop_;
public:
	//ヒットストップ
	float GetHitStopTime() { return hitStopTime_; }
	bool GetHitStop() { return isHitStop_; }
	void SetHitStop(bool _ishit) { isHitStop_ = _ishit; }


//時間ゲージについて
private:
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
};