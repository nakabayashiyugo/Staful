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
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int z = 0; z < mathVolume_.z; z++)
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
	//マス情報書き込み
	std::string savefile = saveFolderName + "saveMath";
	savefile += std::to_string(saveNum_);
	write.open(savefile, std::ios::out);
	//  ファイルが開けなかったときのエラー表示
	if (!write) {
		std::cout << "ファイル " << savefile << " が開けません";
		return;
	}
	for (int i = 0; i < mathVolume_.x; i++) {
		for (int j = 0; j < mathVolume_.z; j++)
		{
			write.write((char*)&math_[i][j], sizeof(math_[i][j]));
			//文字列ではないデータをかきこむ
		}
	}
	write.close();  //ファイルを閉じる

	//とげとげルートの配列のサイズ書き込み
	savefile = saveFolderName + "tgtgRouteVolume";
	savefile += std::to_string(saveNum_);
	write.open(savefile, std::ios::out);
	//  ファイルが開けなかったときのエラー表示
	if (!write) {
		std::cout << "ファイル " << savefile << " が開けません";
		return;
	}
	int tgtgSize = tTgtgRoute_.size();
	write.write((char*)&tgtgSize, sizeof(tgtgSize));
	//文字列ではないデータをかきこむ
	write.close();  //ファイルを閉じる

	//とげとげルートの書き込み
	savefile = saveFolderName + "tgtgRoute";
	savefile += std::to_string(saveNum_);
	write.open(savefile, std::ios::out);
	//  ファイルが開けなかったときのエラー表示
	if (!write) {
		std::cout << "ファイル " << savefile << " が開けません";
		return;
	}
	for (int i = 0; i < tTgtgRoute_.size(); i++)
	{
		write.write((char*)&tTgtgRoute_[i], sizeof(TOGETOGEROUTE));
	}
	write.close();  //ファイルを閉じる
}

void StageOrigin::Read()
{
	std::ifstream read;
	//マス情報読み込み
	std::string openfile = saveFolderName + "saveMath";
	openfile += std::to_string(saveNum_);
	read.open(openfile, std::ios::in);
	if (!read) {
		std::cout << "ファイルが開けません";
		return;
	}
	//  ファイルが開けなかったときの対策
	//ファイルの最後まで続ける
	for (int i = 0; i < mathVolume_.x; i++)
	{
		for (int j = 0; j < mathVolume_.z; j++)
		{
			read.read((char*)&math_[i][j], sizeof(math_[i][j]));
			//文字列ではないデータを読みこむ
		}
	}
	read.close();  //ファイルを閉じる

	//とげとげルートの配列のサイズの読み込み
	openfile = saveFolderName + "tgtgRouteVolume";
	openfile += std::to_string(saveNum_);
	read.open(openfile, std::ios::in);
	//  ファイルを開く
	//  ios::in は読み込み専用  ios::binary はバイナリ形式
	if (!read) {
		std::cout << "ファイルが開けません";
		return;
	}
	//  ファイルが開けなかったときの対策
	int tgtgSize = 0;
	read.read((char*)&tgtgSize, sizeof(tgtgSize));
	//文字列ではないデータを読みこむ
	tTgtgRoute_.resize(tgtgSize);
	read.close();  //ファイルを閉じる


	//とげとげルートの読み込み
	openfile = saveFolderName + "tgtgRoute";
	openfile += std::to_string(saveNum_);
	read.open(openfile, std::ios::in);
	if (!read) {
		std::cout << "ファイルが開けません";
		return;
	}
	for (int i = 0; i < tgtgSize; i++)
	{
		TOGETOGEROUTE pTg;
		read.read((char*)&pTg, sizeof(TOGETOGEROUTE));
		tTgtgRoute_[i] = pTg;

	}


	read.close();  //ファイルを閉じる
}

void StageOrigin::MathVolumeWrite()
{
	std::ofstream write;
	std::string savefile = saveFolderName + "mathVolume";

	write.open(savefile, std::ios::out);

	//  ファイルが開けなかったときのエラー表示
	if (!write) {
		std::cout << "ファイル " << savefile << " が開けません";
		return;
	}

	write.write((char*)&mathVolume_, sizeof(mathVolume_));
	//文字列ではないデータをかきこむ

	write.close();  //ファイルを閉じる
}

void StageOrigin::MathVolumeRead()
{
	std::ifstream read;
	std::string openfile = saveFolderName + "mathVolume";

	read.open(openfile, std::ios::in);
	//  ファイルを開く
	//  ios::in は読み込み専用  ios::binary はバイナリ形式

	if (!read) {
		std::cout << "ファイルが開けません";
		return;
	}
	//  ファイルが開けなかったときの対策

	read.read((char*)&mathVolume_, sizeof(mathVolume_));
			//文字列ではないデータを読みこむ
	read.close();  //ファイルを閉じる
}
