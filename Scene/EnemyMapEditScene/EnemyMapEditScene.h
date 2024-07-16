#pragma once
#include "../MapEditScene/MapEditScene.h"

class RootNode;

class EnemyMapEditScene : public MapEditScene
{
	//スタートマス、ゴールマスが押されたか
	bool isPutStartMath_, isPutGoalMath_;
public:
	EnemyMapEditScene(GameObject* _parent);

	void Initialize() override;
	void Update() override;

	void ChangeSelectMath() override;

	//スタートマスとゴールマス探索
	void StartGoalCheck();
//AI
private:
	RootNode* rootNode_;
public:
	void AddNode();

	void SelectMathSet() override;
	void SelectMathType() override;
};

