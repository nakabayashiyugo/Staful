#pragma once
#include "Engine/GameObject.h"
#include <iostream>
#include <fstream>
#include <vector>

class Button;
class SceneTransition;

class ResultScene
	:public GameObject
{
	//player���N���A�������ǂ����̒l���󂯎��
	std::vector<bool> isClear_Player_;

	Button* pButton_;
	//�{�^���̉摜�ԍ�
	int hButton_;
	//�摜�ԍ�
	int hPlayer1_, hPlayer2_;
	int hWin_, hLose_, hDraw_;

	SceneTransition* pST_;
public:
	ResultScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//���̃^�[���ɍs���{�^���̏�����
	//�����F���[�h����t�@�C���l�[��
	void ButtonInit(std::string _fileName);

	//�摜�̃��[�h
	void PictInit();
	//�摜�̕\��
	void PictDraw();
};