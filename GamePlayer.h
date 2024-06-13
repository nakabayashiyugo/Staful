#pragma once
#include "Engine/GameObject.h"

class SceneTransition;
class MapEditScene;
class PlayScene;

class GamePlayer : public GameObject
{
	//プレイヤーのロゴの画像番号
	int hPlayer_;
	//プレイヤーの番号
	int playerNum_;
	//ファイル番号
	int saveNum_;
	//コースをクリアしたかどうか
	bool isClear_;
	//現在実行中のシーンが終了したかどうか
	bool isSceneFinished_;

	//シーン遷移担当
	SceneTransition* pST_;
	//マップエディット
	MapEditScene* pMES_;
	//プレイシーン
	PlayScene* pPS_;

public:
	GamePlayer(GameObject* parent);
	~GamePlayer();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//マップエディット
	void MapEdit();
	//コース挑戦
	void Challenge();

	//プレイ中のプレイヤーの画像番号のゲッター・セッター
	int GetPlayerPictNum() { return hPlayer_; }
	void SetPlayerPictNum(int _hPlayer) { hPlayer_ = _hPlayer; }

	//プレイヤーの番号のゲッター・セッター
	int GetPlayerNum() { return playerNum_; }
	void SetPlayerNum(int _playerNum) { playerNum_ = _playerNum; }

	//保存するファイル番号のゲッター・セッター
	int GetSaveNum() { return saveNum_; }
	void SetSaveNum(int _saveNum) { saveNum_ = _saveNum; }

	//クリアしたかどうかのフラグのゲッター・セッター
	int GetIsClear() { return isClear_; }
	void SetIsClear(int _isClear) { isClear_ = _isClear; }

	//マップエディットを終了したかどうかのゲッター・セッター
	int GetIsSceneFinished() { return isSceneFinished_; }
	void SetIsSceneFinished(bool _isFinished) { isSceneFinished_ = _isFinished; }

	//ターン数のゲッター
	int GetTurnNum();

	//プレイ中のプレイヤー表示
	void PlayerNumDraw();

	//結果書き込み
	void ResultWrite(bool _isClear);
};

