#include "MathInit.h"

const int saveFile1 = 1;
const int saveFile2 = 2;

MathInit::MathInit()
{
	//ステージの最小サイズ
	const int stageSizeMin = 5;
	//ステージの最大サイズ
	const int stageSizeMax = 18;
	XSIZE = (rand() % (stageSizeMax - stageSizeMin)) + stageSizeMin;
	ZSIZE = (rand() % (stageSizeMax - stageSizeMin)) + stageSizeMin;

	Math_Resize(XSIZE, ZSIZE, &math_);

	//すべてFloorで初期化
	for (int x = 0; x < XSIZE; x++)
	{
		for (int y = 0; y < ZSIZE; y++)
		{
			math_[x][y].mathType_ = MATH_FLOOR;
		}
	}
	saveNum_ = saveFile1;
	Write();
	saveNum_ = saveFile2;
	Write();
}
