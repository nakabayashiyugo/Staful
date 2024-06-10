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

	//  ファイルが開けなかったときのエラー表示
	if (!write) {
		std::cout << "ファイル " << savefile << " が開けません";
		return;
	}
	write.write((char*)&saveElem_, sizeof(saveElem_));

	write.close();  //ファイルを閉じる
}

void StageOrigin::Read()
{
	std::ifstream read;
	std::string savefile = saveFolderName + "saveMath";

	savefile += std::to_string(saveNum_);
	read.open(savefile, std::ios::in);
	//  ファイルを開く
	//  ios::in は読み込み専用  ios::binary はバイナリ形式

	read.read((char*)&saveElem_, sizeof(saveElem_));

	read.close();  //ファイルを閉じる
}

