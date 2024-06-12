#pragma once
#include "../Engine/GameObject.h"
#include "../StageOrigin.h"
class PlayScene;

enum SCENESTATE
{
	SCENE_BETWEEN1 = 0,
	SCENE_MAPEDIT1,
	SCENE_BETWEEN2,
	SCENE_MAPEDIT2,
	SCENE_BETWEEN3,
	SCENE_STAGE1,
	SCENE_BETWEEN4,
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
	int player_Num_;
	//コースを保存するファイル番号
	int saveNum_;
	
	PlayScene* pPS_[PLAYERNUMMAX];

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

	//現在プレイ中のプレイヤー番号表示
	void PlayerNumDraw();

	int GetSceneState() { return (int)sceneState_; };
	void SetSceneState(int _sceneState) { sceneState_ = (SCENESTATE)_sceneState; };

	//現在プレイ中のプレイヤーの番号のゲッター
	int GetPlayerNum() { return player_Num_; }

	int GetTurnNum() { return turnNum_; };

	int GetSaveNum() { return saveNum_; };

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

	//playerがクリアしたかどうかのファイルへの書き込み
	void ResultWrite();


	void SetNextScene() { sceneState_ = (SCENESTATE)((int)sceneState_ + 1); }
};
