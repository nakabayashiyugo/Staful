#pragma once
#include "../Engine/GameObject.h"

//�Q�[���̐����̉摜�̐�
const int explanationNum = 3;

class ExpantionScene
	:public GameObject
{
	//���ꂼ��̉摜�ԍ�
	int hNext_, hExplanNextButton_, hExplanBackButton_;
	int hExplan_[explanationNum];
	int hOverview_;
	//�摜�̈ʒu
	Transform tNext_, tExplanNextButton_, tExplanBackButton_;
	Transform tExplan_[explanationNum];
	Transform tOverview_;

	//�{�^���̃n���h��
	int nextBtnHandle_, explanNextBtnHandle_, explanBackBtnHandle_;

	//�{�^���ԍ�
	int buttonNum_;

	//���݉�ʂɉf���o����Ă���Q�[���̐����̔ԍ�
	float curExplanNum_;
	int destPos_, prevPos_;

	//�����摜���ړ�����
	bool isMoving_;
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

	//�����摜�̈ʒu����
	void ExplanPositioning();

	//�����摜�ړ�
	void ExplanMove();
};