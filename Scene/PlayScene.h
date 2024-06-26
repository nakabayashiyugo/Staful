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

	//�}�X���ς������
	bool table_Change_;
	//�n�ʂ̐ݒu���鍂��
	int floorHeight_;
	//player�̊�̍���
	int playerHeight_;

	Button* cancelButton_;

	GamePlayer* pGP_;

	//���y�̃n���h��
	int hAudio_Music_;
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

	std::vector<std::vector<MATHDEDAIL>> GetTableMath() { return math_; }
	std::vector<TOGETOGEROUTE> GetTogetogeRoute() { return tTgtgRoute_; }

	XMFLOAT3 GetPlayerPos();
	MATHDEDAIL GetPlayerStandMath();

	//�v���C���̃v���C���[�\��
	void PlayerNumDraw();

	void SetTableChange(bool _table_change) { table_Change_ = _table_change; }
	bool GetTableChange() { return table_Change_; }

	int GetPlayerNum() { return playerNum_; }

	int GetSaveNum() { return saveNum_; }

	//�n�ʂ̍����̃Q�b�^�[
	int GetFloorHeight() { return floorHeight_; }
	//�v���C���[�̍����̃Q�b�^�[
	int GetPlayerHeight() { return playerHeight_; }

	Stage* GetStagePointer() { return pStage_; }

	//Write���ĂԊ֐�
	void CallWrite();
	//Read���ĂԊ֐�
	void CallRead();

	//���y�̏�����
	void AudioInit();
};