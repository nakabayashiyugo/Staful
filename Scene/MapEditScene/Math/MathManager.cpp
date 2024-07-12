#include "MathManager.h"

#include "Math.h"

MathManager::MathManager(int _xsize, int _ysize, std::vector<int> _costs, int _num)
{
	//コスト入れる
	costs_.resize(_num);
	_costs.resize(_num);
	for (int i = 0; i < costs_.size(); i++)
	{
		costs_[i] = _costs[i];
	}

	//マス初期化
	//マスの初期化の際のマスの種類
	const int initType = (int)MATH_DELETE;
	maths_.resize(_xsize);
	for (int x = 0; x < maths_.size(); x++)
	{
		maths_[x].resize(_ysize);
		for (int y = 0; y < maths_[x].size(); y++)
		{
			maths_[x][y] = new Math(this, initType, costs_[initType]);
		}
	}
}

void MathManager::SetMathType(XMFLOAT2 _pos, int _type)
{
	maths_[_pos.x][_pos.y]->SetMathType(_type);
	maths_[_pos.x][_pos.y]->SetCost(costs_[_type]);
}

int MathManager::GetMathType(XMFLOAT2 _pos)
{
	return maths_[_pos.x][_pos.y]->GetMathType();
}

Transform MathManager::GetMathTransform(XMFLOAT2 _pos)
{
	return maths_[_pos.x][_pos.y]->GetMathTransform();
}

void MathManager::SetMathTransform(XMFLOAT2 _pos, Transform _transform)
{
	maths_[_pos.x][_pos.y]->SetMathTransform(_transform);
}

int MathManager::GetCost(int _type)
{
	return costs_[_type];
}

int MathManager::GetStageCost()
{
	int ret = 0;
	for (int x = 0; x < maths_.size(); x++)
	{
		for (int y = 0; y < maths_[x].size(); y++)
		{
			ret += maths_[x][y]->GetCost();
		}
	}
	return ret;
}
