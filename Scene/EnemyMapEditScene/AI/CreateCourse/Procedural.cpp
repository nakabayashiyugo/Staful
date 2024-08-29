#include "Procedural.h"

Seed::Seed(XMFLOAT2 _pos)
	:volume_(XMFLOAT3(0, 0, 0))
{
	position_ = _pos;
}


Procedural::Procedural(std::vector<std::vector<int>> _mathShades, int _xsize, int _ysize)
{
	//���ꂼ��̃}�X���Ƀ}�X�̎�ޓ����
	mathShades_.resize(_xsize);
	for (int i = 0; i < mathShades_.size(); i++)
	{
		mathShades_[i].resize(_ysize);
		for (int j = 0; j < mathShades_[i].size(); j++)
		{
			mathShades_[i][j] = _mathShades[i][j];
		}
	}
}

Procedural::~Procedural()
{
	for (int i = 0; i < seedList_.size(); i++)
	{
		delete seedList_[i];
	}
}

void Procedural::Excute(int _seedNum)
{
	SeedScatter(_seedNum);
}

void Procedural::SeedScatter(int _seedNum)
{
	//�����œn���ꂽ�΂�܂��񐔕��J��Ԃ�
	for (int i = 0; i < _seedNum; i++)
	{
		int randX = 0;
		int randY = 0;
		do {
			randX = rand() % mathShades_.size();
			randY = rand() % mathShades_[0].size();
		} while (IsSeedPresence(randX, randY));
		Seed* seed = new Seed(XMFLOAT2(randX, randY));	//�C���X�^���X���
		seed->SetShade(mathShades_[randX][randY]);		//�Z�W�����
		seedList_.push_back(seed);						//���X�g�ɓ����
		//���X�g�̃T�C�Y���}�X�̐��ȏ�ɂȂ�����J��Ԃ�������
		if (seedList_.size() >= (mathShades_.size() * mathShades_[0].size()))
		{
			break;
		}
	}
	SeedListSort();
}

bool Procedural::IsSeedPresence(int _x, int _y)
{
	for (int i = 0; i < seedList_.size(); i++)
	{
		if (seedList_[i]->GetPos().x == _x && seedList_[i]->GetPos().y == _y)
		{
			return true;
		}
	}
	return false;
}

void Procedural::VolumeCheck()
{
}

void Procedural::SeedListSort()
{
	for (int i = 0; i < seedList_.size() - 1; i++)
	{
		for (int j = i; j < seedList_.size(); j++)
		{
			if (seedList_[i]->GetShade() < seedList_[j]->GetShade())
			{
				Seed* tmp = seedList_[i];
				seedList_[i] = seedList_[j];
				seedList_[j] = tmp;
			}
		}
	}
}
