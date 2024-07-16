#pragma once
#include "../MapEditScene/MapEditScene.h"

class RootNode;

class EnemyMapEditScene : public MapEditScene
{
	//�X�^�[�g�}�X�A�S�[���}�X�������ꂽ��
	bool isPutStartMath_, isPutGoalMath_;
public:
	EnemyMapEditScene(GameObject* _parent);

	void Initialize() override;
	void Update() override;

	void ChangeSelectMath() override;

	//�X�^�[�g�}�X�ƃS�[���}�X�T��
	void StartGoalCheck();
//AI
private:
	RootNode* rootNode_;
public:
	void AddNode();

	void SelectMathSet() override;
	void SelectMathType() override;
};

