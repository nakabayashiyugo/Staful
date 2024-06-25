#pragma once
#include "../Engine/GameObject.h"
#include "../StageOrigin.h"

class Stage;
class Player;
class Button;
class GamePlayer;

class PlayScene
	:public GameObject, StageOrigin
{
	Stage* pStage_;
	Player* pPlayer_;

	//マスが変わったか
	bool table_Change_;
	//地面の設置する高さ
	int floorHeight_;
	//playerの基準の高さ
	int playerHeight_;

	Button* cancelButton_;

	GamePlayer* pGP_;

	//音楽のハンドル
	int hAudio_Music_;
public:
	PlayScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	std::vector<std::vector<MATHDEDAIL>> GetTableMath() { return math_; }
	std::vector<TOGETOGEROUTE> GetTogetogeRoute() { return tTgtgRoute_; }

	XMFLOAT3 GetPlayerPos();
	MATHDEDAIL GetPlayerStandMath();

	//プレイ中のプレイヤー表示
	void PlayerNumDraw();

	void SetTableChange(bool _table_change) { table_Change_ = _table_change; }
	bool GetTableChange() { return table_Change_; }

	int GetPlayerNum() { return playerNum_; }

	int GetSaveNum() { return saveNum_; }

	//地面の高さのゲッター
	int GetFloorHeight() { return floorHeight_; }
	//プレイヤーの高さのゲッター
	int GetPlayerHeight() { return playerHeight_; }

	Stage* GetStagePointer() { return pStage_; }

	//Writeを呼ぶ関数
	void CallWrite();
	//Readを呼ぶ関数
	void CallRead();

	//音楽の初期化
	void AudioInit();
};