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

	//移動距離
	XMFLOAT3 moveVec_;
	//移動先の位置
	XMFLOAT3 destPos_;
	//移動前の位置
	XMFLOAT3 prevPos_;
	//移動ベクトル
	XMVECTOR velocity_;
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
	STAGE_STATE stageState;
	//アニメーションのフレーム
	int nowFrame_, startFrame_, endFrame_;

	//立っているマスの情報
	MATHDEDAIL standMath_;

	//重力
	XMFLOAT3 gravity_;

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
	//プレイヤーの操作に関する関数
	void PlayerOperation();
	//前移動
	void PlayerMoveFront();
	//後ろ移動
	void PlayerMoveBack();
	//右移動
	void PlayerMoveRight();
	//左移動
	void PlayerMoveLeft();

	void IdleUpdate();
	void WalkUpdate();
	void JampUpdate();
	void FallUpdate();
	void DeadUpdate();
	void ReturnToStartMath();

	float GetHitStopTime() { return hitStopTime_; }
	bool GetHitStop() { return isHitStop_; }
	void SetHitStop(bool _ishit) { isHitStop_ = _ishit; }

	void SetAnimFramerate();

	bool Is_InSide_Table(XMFLOAT3 _pos);

	

	bool Is_Goal() { return isGoal_; }

	XMFLOAT3 GetPosition() { return centerPos_; }

	MATHDEDAIL GetMathType(XMFLOAT3 _pos);

	bool GetFailed() {
		if (stageState == STATE_FAILURE)
		{
			return true;
		}
		return false;
	}
	PLAYER_STATE GetPlayerState() { return playerState_; }

	void OnCollision(GameObject* pTarget) override;

	//時間ゲージ
	void TimeGageManagement();
};