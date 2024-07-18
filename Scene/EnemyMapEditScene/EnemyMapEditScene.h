#pragma once
#include "../MapEditScene/MapEditScene.h"

class RootNode;

class EnemyMapEditScene : public MapEditScene
{
	//�摜�ԍ�
	int hPict_;

	//�X�^�[�g�}�X�A�S�[���}�X�̈ʒu
	XMFLOAT2 startMathPos_, goalMathPos_;
public:
	EnemyMapEditScene(GameObject* _parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void ChangeSelectMath() override;

	//�X�^�[�g�}�X�ƃS�[���}�X�T��
	bool StartGoalCheck();
	//�X�^�[�g�}�X�ƃS�[���}�X�Z�b�g
	void StartGoalSet();
//AI
private:
	RootNode* rootNode_;

	//�}�X��u�����(�D�揇�ʍ�����)
	std::vector<XMFLOAT2> candidatePositions_;
	//����΂�܂���
	int seedNum_;
public:
	void AddNode();

	void SelectMathSet() override;
	void SelectMathType() override;

	//Procedural���s
	void ProcExcute();
	//����1�������2�܂ł̋���
	float GetDistance(XMFLOAT2 _pos1, XMFLOAT2 _pos2);
};

