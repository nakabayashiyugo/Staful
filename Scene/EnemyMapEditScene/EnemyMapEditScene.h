#pragma once
#include "../MapEditScene/MapEditScene.h"

class RootNode;
class EyeCatching;
class Timer;

class EnemyMapEditScene : public MapEditScene
{
	EyeCatching* eyeCatch_;
	Timer* makingTimer_;

	//スタートマス、ゴールマスの位置
	XMFLOAT2 startMathPos_, goalMathPos_;
public:
	EnemyMapEditScene(GameObject* _parent);
	~EnemyMapEditScene();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void MathDraw() override;

	void ChangeSelectMath() override;

	//スタートマスとゴールマス探索
	bool StartGoalCheck();
	//スタートマスとゴールマスセット
	void StartGoalSet();
//AI
private:
	RootNode* rootNode_;

	//マスを置く候補(優先順位高い順)
	std::vector<XMFLOAT2> candidatePositions_;
	//種をばらまく数
	int seedNum_;
public:
	void AddNode();

	void SelectMathSet() override;
	void SelectMathType() override;

	//Procedural実行
	void ProcExcute();
	//引数1から引数2までの距離
	float GetDistance(XMFLOAT2 _pos1, XMFLOAT2 _pos2);
};

