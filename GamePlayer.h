#pragma once
#include "Engine/GameObject.h"

class SceneTransition;
class MapEditScene;
class PlayScene;

class GamePlayer : public GameObject
{
	//�v���C���[�̃��S�̉摜�ԍ�
	int hPlayer_;
	//�v���C���[�̔ԍ�
	int playerNum_;
	//�t�@�C���ԍ�
	int saveNum_;
	//�R�[�X���N���A�������ǂ���
	bool isClear_;

	//�V�[���J�ڒS��
	SceneTransition* pST_;
	//�}�b�v�G�f�B�b�g
	MapEditScene* pMES_;
	//�v���C�V�[��
	PlayScene* pPS_;

public:
	GamePlayer(GameObject* parent);
	~GamePlayer();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	//�}�b�v�G�f�B�b�g
	void MapEdit();
	//�R�[�X����
	void Challenge();

	//�}�b�v�G�f�B�b�g���I�������Ƃ�
	void MapEditFinished();
	//�R�[�X���킵�I������Ƃ�
	void ChallengeFinished();

	//�v���C���̃v���C���[�̉摜�ԍ��̃Q�b�^�[�E�Z�b�^�[
	int GetPlayerPictNum() { return hPlayer_; }
	void SetPlayerPictNum(int _hPlayer) { hPlayer_ = _hPlayer; }

	//�v���C���[�̔ԍ��̃Q�b�^�[�E�Z�b�^�[
	int GetPlayerNum() { return playerNum_; }
	void SetPlayerNum(int _playerNum) { playerNum_ = _playerNum; }

	//�ۑ�����t�@�C���ԍ��̃Q�b�^�[�E�Z�b�^�[
	int GetSaveNum() { return saveNum_; }
	void SetSaveNum(int _saveNum) { saveNum_ = _saveNum; }

	//�N���A�������ǂ����̃t���O�̃Q�b�^�[�E�Z�b�^�[
	int GetIsClear() { return isClear_; }
	void SetIsClear(int _isClear) { isClear_ = _isClear; }

	//�^�[�����̃Q�b�^�[
	int GetTurnNum();

	//�v���C���̃v���C���[�\��
	void PlayerNumDraw();

	//���ʏ�������
	void ResultWrite(bool _isClear);
};

