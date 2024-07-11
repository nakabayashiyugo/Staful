#pragma once
#include "../../../StageOrigin.h"

class MathManager;

class Math : public StageOrigin
{
	MATHDEDAIL mathDetail_;
	int cost_;			//コスト

	MathManager* parent_;	//親のマネージャー
public:
	Math(MathManager* _parent, int _type, int _cost);

//ゲッターセッター類
public:
	int GetMathType() { return mathDetail_.mathType_; }
	void SetMathType(int _type);
	Transform GetMathPos() { return mathDetail_.mathPos_; }
	void SetMathPos(Transform _transform) { mathDetail_.mathPos_ = _transform; }
	int GetCost() { return cost_; }
	void SetCost(int _cost) { cost_ = _cost; }
};

