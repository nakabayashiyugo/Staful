#include "MathManager.h"

#include "Math.h"

MathManager::MathManager(int _xsize, int _ysize, int _initType, std::vector<int> _costs, int _num, int _costLimit)
{
	costLimit_ = _costLimit;
	//コスト入れる
	costs_.resize(_num);
	_costs.resize(_num);
	for (int i = 0; i < costs_.size(); i++)
	{
		costs_[i] = _costs[i];
	}

	//マス初期化
	maths_.resize(_xsize);
	for (int x = 0; x < maths_.size(); x++)
	{
		maths_[x].resize(_ysize);
		for (int y = 0; y < maths_[x].size(); y++)
		{
			maths_[x][y] = new Math(this, _initType, costs_[_initType]);
		}
	}
}

MathManager::~MathManager()
{
	for (int x = 0; x < maths_.size(); x++)
	{
		for (int y = 0; y < maths_[x].size(); y++)
		{
			delete maths_[x][y];
		}
	}
}

bool MathManager::IsCostOver(XMFLOAT2 _pos, int _type)
{
	int costSum = 0;
	costSum = this->GetStageCost();
	//変更後のコストたす
	if (this->GetMathType(XMFLOAT2(_pos.x, _pos.y)) != _type)
	{
		costSum += costs_[_type];
		//コストの制限を超えていたら
		if (costSum > costLimit_)
		{
			costSum -= costs_[_type];
			return true;
		}
	}
	return false;
}

void MathManager::SetMathType(XMFLOAT2 _pos, int _type)
{
	if (!IsCostOver(_pos, _type))
	{
		maths_[_pos.x][_pos.y]->SetMathType(_type);
		maths_[_pos.x][_pos.y]->SetCost(costs_[_type]);
	}
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
