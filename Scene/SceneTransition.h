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

//�v���C���[�̐l��
const int PLAYERNUMMAX = 2;

class SceneTransition
	:public GameObject
{
	//player���N���A�������ǂ���
	bool isClear_Player_[PLAYERNUMMAX];
	//���݂̃^�[����
	int turnNum_;
	//���݃v���C���̃v���C���[�̔ԍ�
	int player_Num_;
	//�R�[�X��ۑ�����t�@�C���ԍ�
	int saveNum_;
	
	PlayScene* pPS_[PLAYERNUMMAX];

	SCENESTATE sceneState_, prevSceneState_;

	//���S�̉摜�ԍ�
	int hPlayer1_, hPlayer2_;
public:
	SceneTransition(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//���݃v���C���̃v���C���[�ԍ��\��
	void PlayerNumDraw();

	int GetSceneState() { return (int)sceneState_; };
	void SetSceneState(int _sceneState) { sceneState_ = (SCENESTATE)_sceneState; };

	//���݃v���C���̃v���C���[�̔ԍ��̃Q�b�^�[
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

	//player���N���A�������ǂ����̃t�@�C���ւ̏�������
	void ResultWrite();


	void SetNextScene() { sceneState_ = (SCENESTATE)((int)sceneState_ + 1); }
};
