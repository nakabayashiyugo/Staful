#pragma once
#include "../Engine/GameObject.h"
#include "../StageOrigin.h"

class Stage;
class Player;

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

	int player_Num_;

	int saveNum_;
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

	void Read();

	std::vector<std::vector<MATHDEDAIL>> GetTableMath() { return math_; }
	std::vector<TOGETOGEROUTE> GetTogetogeRoute() { return tTgtgRoute_; }

	XMFLOAT3 GetPlayerPos();
	MATHDEDAIL GetPlayerStandMath();

	void SetTableChange(bool _table_change) { table_Change_ = _table_change; }
	bool GetTableChange() { return table_Change_; }

	int GetPlayerNum() { return player_Num_; }

	int GetSaveNum() { return saveNum_; }

	//地面の高さのゲッター
	int GetFloorHeight() { return floorHeight_; }
	//プレイヤーの高さのゲッター
	int GetPlayerHeight() { return playerHeight_; }

};