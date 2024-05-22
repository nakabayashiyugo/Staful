#pragma once
#include "../Engine/GameObject.h"

class Button;
class BillBoard;

//�Q�[���̐����̉摜�̐�
const int explanationNum = 3;

class ExpantionScene
	:public GameObject
{
	//���ꂼ��̉摜�ԍ�
	int hNext_, hExplanNextButton_, hExplanReturn_;
	int hExplan_[explanationNum];
	//�摜�̈ʒu
	Transform tStartButton_, tExplanNextButton_, hExplanReturn_;
	Transform tExplan_[explanationNum];

	//�{�^��
	Button* pNext_, *pExplanNextButton_, *pExplanReturn_;
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
};