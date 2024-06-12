#include "MathInit.h"

const int saveFile1 = 1;
const int saveFile2 = 2;

MathInit::MathInit()
{
	//�X�e�[�W�̍ŏ��T�C�Y
	const int stageSizeMin = 5;
	//�X�e�[�W�̍ő�T�C�Y
	const int stageSizeMax = 18;
	XSIZE = (rand() % (stageSizeMax - stageSizeMin)) + stageSizeMin;
	ZSIZE = (rand() % (stageSizeMax - stageSizeMin)) + stageSizeMin;

	Math_Resize(XSIZE, ZSIZE, &math_);

	//���ׂ�Floor�ŏ�����
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
