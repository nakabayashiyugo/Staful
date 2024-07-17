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
	mathVolume_.x = (rand() % (stageSizeMax - stageSizeMin)) + stageSizeMin;
	mathVolume_.y = (rand() % (stageSizeMax - stageSizeMin)) + stageSizeMin;
	mathVolume_.z = mathVolume_.y;

	Math_Resize(mathVolume_.x, mathVolume_.z, &math_);

	//マスの量書き込み
	MathVolumeWrite();

	//すべてFloorで初期化
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.z; y++)
		{
			math_[x][y].mathType_ = MATH_DELETE;
		}
	}
	saveNum_ = saveFile1;
	//書き込み
	Write();
	saveNum_ = saveFile2;
	Write();
}
