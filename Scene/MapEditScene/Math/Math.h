#pragma once
#include "../../../Engine/Transform.h"

class MathManager;

class Math
{
	int mathType_;
	Transform tMath_;
	int cost_;			//コスト

	MathManager* parent_;	//親のマネージャー
public:
	Math(MathManager* _parent, int _type, int _cost);

	//自分のマスの中に引数の値の位置が入っているか
	bool IsMouseEnter(XMFLOAT2 _pos);

//ゲッターセッター類
public:
	int GetMathType() { return mathType_; }
	void SetMathType(int _type);
	Transform GetMathTransform() { return tMath_; }
	void SetMathTransform(Transform _transform) { tMath_ = _transform; }
	int GetCost() { return cost_; }
	void SetCost(int _cost) { cost_ = _cost; }
};

