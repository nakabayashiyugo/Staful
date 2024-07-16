#pragma once
#include "../Engine/GameObject.h"
#include "../StageOrigin.h"

class PlayScene;
class GamePlayer;

enum SCENESTATE
{
	SCENE_MAPEDIT1 = 0,
	SCENE_MAPEDIT2,
	SCENE_STAGE1,
	SCENE_STAGE2,
	SCENE_RESULT,
	SCENE_TURNEND,
};

//一人プレイか二人プレイか
enum GAMESTATE
{
	STATE_SOLO = 0,
	STATE_TWO,
};

//プレイヤーの人数
const int PLAYERNUMMAX = 2;

class SceneTransition
	:public GameObject
{
	//現在のターン数
	int turnNum_;
	//現在プレイ中のプレイヤーの番号
	int playerNum_;
	
	GamePlayer* gPlayer_[PLAYERNUMMAX];

	SCENESTATE sceneState_, prevSceneState_;

	//一人プレイか二人プレイか
	GAMESTATE gameState_;

	//ロゴの画像番号
	int hPlayer1_, hPlayer2_;
public:
	SceneTransition(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	int GetSceneState() { return (int)sceneState_; };
	void SetSceneState(int _sceneState) { sceneState_ = (SCENESTATE)_sceneState; };

	//現在プレイ中のプレイヤーの番号のゲッター
	int GetPlayerNum() { return playerNum_; }

	int GetTurnNum() { return turnNum_; };

	void SetNextScene() { sceneState_ = (SCENESTATE)((int)sceneState_ + 1); }

	//GamePlayerの関数を呼び出す関数
	//引数：プレイヤー番号(0からスタート)
	void CallMapEdit(int _playerNum);
	void CallChallenge(int _playerNum);
};
