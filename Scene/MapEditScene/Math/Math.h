#pragma once
#include "../../../StageOrigin.h"

class MathManager;

class Math : public StageOrigin
{
	MATHDEDAIL mathDetail_;
	int cost_;			//�R�X�g

	MathManager* parent_;	//�e�̃}�l�[�W���[
public:
	Math(MathManager* _parent, int _type, int _cost);

//�Q�b�^�[�Z�b�^�[��
public:
	int GetMathType() { return mathDetail_.mathType_; }
	void SetMathType(int _type);
	Transform GetMathPos() { return mathDetail_.mathPos_; }
	void SetMathPos(Transform _transform) { mathDetail_.mathPos_ = _transform; }
	int GetCost() { return cost_; }
	void SetCost(int _cost) { cost_ = _cost; }
};

