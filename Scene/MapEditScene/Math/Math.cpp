#include "Math.h"

#include "MathManager.h"
#include "../../../StageOrigin.h"

Math::Math(MathManager* _parent, int _type, int _cost)
	:parent_(_parent), cost_(_cost)
{
	mathType_ = (MATHTYPE)_type;
}

bool Math::IsMouseEnter(XMFLOAT2 _pos)
{
	float top = tMath_.position_.y + ((float)MATHSIZE / Direct3D::bfr_scrHeight);
	float bottom = tMath_.position_.y - ((float)MATHSIZE / Direct3D::bfr_scrHeight);
	float left = tMath_.position_.x - ((float)MATHSIZE / Direct3D::bfr_scrWidth);
	float right = tMath_.position_.x + ((float)MATHSIZE / Direct3D::bfr_scrWidth);
	
	if (_pos.x >= left && _pos.x <= right &&
		_pos.y >= bottom && _pos.y <= top)
	{
		return true;
	}

	return false;
}

void Math::SetMathType(int _type)
{
	mathType_ = (MATHTYPE)_type;
	cost_ = parent_->GetCost(_type);
}
