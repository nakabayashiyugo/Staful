#include "MathInit.h"

//保存するファイルの番号
const int saveFile1 = 1;
const int saveFile2 = 2;

MathInit::MathInit()
{
	//ステージの最小サイズ
	const int stageSizeMin = 5;
	//ステージの最大サイズ
	const int stageSizeMax = 18;
	saveElem_.mathVolue_.x = (rand() % (stageSizeMax - stageSizeMin)) + stageSizeMin;
	saveElem_.mathVolue_.z = (rand() % (stageSizeMax - stageSizeMin)) + stageSizeMin;
	//マスサイズ変更
	Math_Resize(saveElem_.mathVolue_.x, saveElem_.mathVolue_.z, &saveElem_.math_);
	//ターン数(最初のターンしかこのクラス呼ばれないので1)
	saveElem_.turn_ = 1;
	saveNum_ = saveFile1;
	//書き込み
	Write();
	saveNum_ = saveFile2;
	Write();
}
