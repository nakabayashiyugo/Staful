#pragma once
#include "Engine/GameObject.h"
#include <iostream>
#include <fstream>
#include <vector>

class Button;

class ResultScene
	:public GameObject
{
	//player���N���A�������ǂ����̒l���󂯎��
	std::vector<bool> isClear_Player_;

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

	//player�̃��U���g��ǂݍ���
	void ResultRead();
};