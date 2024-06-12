#include "StageOrigin.h"
StageOrigin::StageOrigin()
{
	fileNameInit_.resize(MATH_MAX);
	fileNameInit_ = {
		"Floor",
		"Wall",
		"Hole",
		"Conveyor",
		"Togetoge",
		"PitFall",
		"TimeLimitDec",
		"Confusion",
		"Start",
		"Goal"
	};
}

void StageOrigin::Math_Resize(int _xsize, int _zsize, std::vector<std::vector<MATHDEDAIL>>* _math)
{
	_math->resize(_xsize);
	for (int x = 0; x < _xsize; x++)
	{
		_math->at(x).resize(_zsize);
	}
}

void StageOrigin::Math_Resize(int _xsize, int _zsize, std::vector<std::vector<int>>* _math)
{
	_math->resize(_xsize);
	for (int x = 0; x < _xsize; x++)
	{
		_math->at(x).resize(_zsize);
	}
}

void StageOrigin::Math_Resize(int _xsize, int _zsize, std::vector<std::vector<bool>>* _math)
{
	_math->resize(_xsize);
	for (int x = 0; x < _xsize; x++)
	{
		_math->at(x).resize(_zsize);
		for (int z = 0; z < _zsize; z++)
		{
			_math->at(x).at(z) = false;
		}
	}
}

void StageOrigin::SetTableMath(std::vector<std::vector<MATHDEDAIL>> _math)
{
	for (int x = 0; x < XSIZE; x++)
	{
		for (int z = 0; z < ZSIZE; z++)
		{
			math_.at(x).at(z) = _math.at(x).at(z);
		}
	}
}
void StageOrigin::SetTogetogeRoute(std::vector<TOGETOGEROUTE> _tgtgRoute)
{
	for (int i = 0; i < _tgtgRoute.size(); i++)
	{
		TOGETOGEROUTE* pTg = new TOGETOGEROUTE();
		pTg->route_ = _tgtgRoute[i].route_;
		pTg->initPos_ = _tgtgRoute[i].initPos_;
		pTg->destPos_ = _tgtgRoute[i].destPos_;
		tTgtgRoute_.push_back(*pTg);
		delete pTg;
	}
}

void StageOrigin::Write()
{
	std::ofstream write;
	std::string savefile = saveFolderName + "saveMath";
	savefile += std::to_string(saveNum_);

	write.open(savefile, std::ios::out);

	//  �t�@�C�����J���Ȃ������Ƃ��̃G���[�\��
	if (!write) {
		std::cout << "�t�@�C�� " << savefile << " ���J���܂���";
		return;
	}

	for (int i = 0; i < XSIZE; i++) {
		for (int j = 0; j < ZSIZE; j++)
		{
			write.write((char*)&math_[i][j], sizeof(math_[i][j]));
			//������ł͂Ȃ��f�[�^����������
		}
	}

	write.close();  //�t�@�C�������

	//�Ƃ��Ƃ����[�g
	savefile = saveFolderName + "tgtgRoute";
	savefile += std::to_string(saveNum_);
	write.open(savefile, std::ios::out);
	//  �t�@�C�����J���Ȃ������Ƃ��̃G���[�\��
	if (!write) {
		std::cout << "�t�@�C�� " << savefile << " ���J���܂���";
		return;
	}
	for (auto itr = tTgtgRoute_.begin(); itr != tTgtgRoute_.end(); itr++)
	{
		write.write((char*)&itr, sizeof(itr));
	}
	write.close();  //�t�@�C�������
}

void StageOrigin::Read()
{
	std::ifstream read;
	std::string openfile = saveFolderName + "saveMath";

	openfile += std::to_string(saveNum_);
	read.open(openfile, std::ios::in);
	//  �t�@�C�����J��
	//  ios::in �͓ǂݍ��ݐ�p  ios::binary �̓o�C�i���`��

	if (!read) {
		std::cout << "�t�@�C�����J���܂���";
		return;
	}
	//  �t�@�C�����J���Ȃ������Ƃ��̑΍�

	//�t�@�C���̍Ō�܂ő�����
	for (int i = 0; i < XSIZE; i++)
	{
		for (int j = 0; j < ZSIZE; j++)
		{
			read.read((char*)&math_[i][j], sizeof(math_[i][j]));
			//������ł͂Ȃ��f�[�^��ǂ݂���

		}
	}
	read.close();  //�t�@�C�������

	//�Ƃ��Ƃ����[�g
	openfile = saveFolderName + "tgtgRoute";
	openfile += std::to_string(saveNum_);
	read.open(openfile, std::ios::in);
	if (!read) {
		std::cout << "�t�@�C�����J���܂���";
		return;
	}

	while (!read.eof())
	{
		TOGETOGEROUTE pTg;
		read.read((char*)&pTg, sizeof(pTg));
		if (pTg.route_.scale_.x < 1)
		{
			tTgtgRoute_.push_back(pTg);
		}
	}
	read.close();  //�t�@�C�������
}

void StageOrigin::MathVolumeWrite()
{
}

void StageOrigin::MathVolumeRead()
{
}
