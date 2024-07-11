#include "Math.h"

#include "MathManager.h"

Math::Math(MathManager* _parent, int _type, int _cost)
	:parent_(_parent), cost_(_cost)
{
	mathDetail_.mathType_ = (MATHTYPE)_type;
}

void Math::SetMathType(int _type)
{
	mathDetail_.mathType_ = (MATHTYPE)_type;
	cost_ = parent_->GetCost(_type);
}
