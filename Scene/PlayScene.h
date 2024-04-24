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

	//�}�X���ς������
	bool table_Change_;
	//�n�ʂ̐ݒu���鍂��
	int floorHeight_;
	//player�̊�̍���
	int playerHeight_;

	int player_Num_;

	int saveNum_;
public:
	PlayScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
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

	//�n�ʂ̍����̃Q�b�^�[
	int GetFloorHeight() { return floorHeight_; }
	//�v���C���[�̍����̃Q�b�^�[
	int GetPlayerHeight() { return playerHeight_; }

};