#pragma once
#include "../Engine/GameObject.h"

class SceneTransition;
class Easing;

enum COURSELOGOSTATE
{
	STATE_WAIT = 0,
	STATE_MOVE1,
	STATE_MOVE2,
}; 

class BetweenScene
	:public GameObject
{
	//�e���S�̓����𐧌䂷��J�E���g
	float logoPlayerMoveCount_;
	float logoCourseMoveCount_;

	//�e���S�̉摜�ԍ�
	int hPlayer_;
	int hPlayer1_, hPlayer2_;
	int hLogoEmptyCourse_, hLogoCourse1_, hLogoCourse2_;
	int hLogoMapCreate_, hLogoPlay_;

	//�e���S�̃g�����X�t�H�[��
	Transform tLogoStandard_;
	Transform tPlayer1_, tPlayer2_;
	Transform tLogoCourse1_, tLogoCourse2_;
	Transform tLogoMapCreate_;

	COURSELOGOSTATE logoState_;

	SceneTransition* pST_;

	int curScene_;

	//�C�[�W���O���g����悤�ɂ���N���X
	Easing* pEasing_;
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

	//���ɌĂԊ֐�
	void NextCallFunction();

	//���̃V�[�����I�������
	void Finished();
};