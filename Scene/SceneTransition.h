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

//プレイヤーの人数
const int PLAYERNUMMAX = 2;

class SceneTransition
	:public GameObject
{
	//playerがクリアしたかどうか
	bool isClear_Player_[PLAYERNUMMAX];
	//現在のターン数
	int turnNum_;
	//現在プレイ中のプレイヤーの番号
	int playerNum_;
	
	GamePlayer* gPlayer_[PLAYERNUMMAX];

	SCENESTATE sceneState_, prevSceneState_;

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

	bool GetIsClear(int _player_num)
	{
		if (_player_num < PLAYERNUMMAX && _player_num >= 0)
		{
			return isClear_Player_[_player_num];
		}
		return 0;
	}
	void SetIsClear(int _player_num, bool _isClear)
	{
		if (_player_num < PLAYERNUMMAX && _player_num >= 0)
		{
			isClear_Player_[_player_num] = _isClear;
		}
	}
};
