#pragma once
#include "../../../Engine/Transform.h"

class MathManager;

class Math
{
	int mathType_;
	Transform tMath_;
	int cost_;			//�R�X�g

	MathManager* parent_;	//�e�̃}�l�[�W���[
public:
	Math(MathManager* _parent, int _type, int _cost);

	//�����̃}�X�̒��Ɉ����̒l�̈ʒu�������Ă��邩
	bool IsMouseEnter(XMFLOAT2 _pos);

//�Q�b�^�[�Z�b�^�[��
public:
	int GetMathType() { return mathType_; }
	void SetMathType(int _type);
	Transform GetMathTransform() { return tMath_; }
	void SetMathTransform(Transform _transform) { tMath_ = _transform; }
	int GetCost() { return cost_; }
	void SetCost(int _cost) { cost_ = _cost; }
};

