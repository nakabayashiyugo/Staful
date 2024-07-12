#include "Math.h"

#include "MathManager.h"

Math::Math(MathManager* _parent, int _type, int _cost)
	:parent_(_parent), cost_(_cost)
{
	mathType_ = (MATHTYPE)_type;
}

void Math::SetMathType(int _type)
{
	mathType_ = (MATHTYPE)_type;
	cost_ = parent_->GetCost(_type);
}
