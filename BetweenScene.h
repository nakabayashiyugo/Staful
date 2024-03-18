#pragma once
#include "Engine/GameObject.h"

class BetweenScene
	:public GameObject
{
	float logoPlayerMoveCount_;
	float logoCourseMoveCount_;

	int hPlayer1_, hPlayer2_;
	int hLogoEmptyCourse_, hLogoCourse1_, hLogoCourse2_;

	Transform tLogoStandard_;
	Transform tPlayer1_, tPlayer2_;
	Transform tLogoCourse1_, tLogoCourse2_;
public:
	BetweenScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	void Player1MapEditUpdate();
	void Player2MapEditUpdate();
	void Player1PlayUpdate();
	void Player2PlayUpdate();

	float EaseInSine(float pos);
};