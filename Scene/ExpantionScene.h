#pragma once
#include "../Engine/GameObject.h"

class Button;

//�Q�[���̐����̉摜�̐�
const int explanationNum = 3;

class ExpantionScene
	:public GameObject
{
	//���ꂼ��̉摜�ԍ�
	int hNext_, hExplanNextButton_, hExplanReturnButton_;
	int hExplan_[explanationNum];
	int hOverview_;
	//�摜�̈ʒu
	Transform tNext_, tExplanNextButton_, tExplanReturnButton_;
	Transform tExplan_[explanationNum];
	Transform tOverview_;

	//�{�^��
	Button* pNext_, *pExplanNextButton_, *pExplanReturnButton_;

	//�{�^���ԍ�
	int buttonNum_;
public:
	ExpantionScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//�{�^���̏�����
	void ButtonInit();
	int GetButtonNum() { return buttonNum_; }
};