#pragma once
#include "Engine/GameObject.h"
#include "StageOrigin.h"
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
	SCENE_TURNEND,
};

const int PLAYERNUM = 2;

class SceneTransition
	:public GameObject, StageOrigin
{
	//player���N���A�������ǂ���
	bool isClear_Player_[PLAYERNUM];
	//���݂̃^�[����
	int turnNum_;
	//player�̔ԍ�
	int player_Num_;
	//�R�[�X��ۑ�����t�@�C���ԍ�
	int saveNum_;
	
	PlayScene* pPS_[PLAYERNUM];

	//���S�̉摜�ԍ�
	int hPlayer1_, hPlayer2_;
	int hWin_, hLose_;

	SCENESTATE sceneState_, prevSceneState_;
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

	int GetMathSize_x() { return XSIZE; };
	int GetMathSize_z() { return ZSIZE; };

	int GetSceneState() { return (int)sceneState_; };
	void SetSceneState(int _sceneState) { sceneState_ = (SCENESTATE)_sceneState; };

	int GetPlayerNum() { return player_Num_; }

	int GetTurnNum() { return turnNum_; };

	int GetSaveNum() { return saveNum_; };

	void SetIsClear(int _player_num, bool _isClear)
	{
		if (_player_num >= 2 && _player_num < 0)
		{
			return;
		}
		isClear_Player_[_player_num] = _isClear;
	}

	//�R�[�X�̃t�@�C���ւ̏�������
	void Write();
	//player���N���A�������ǂ����̃t�@�C���ւ̏�������
	void ResultWrite();


	void SetNextScene() { sceneState_ = (SCENESTATE)((int)sceneState_ + 1); }
};
