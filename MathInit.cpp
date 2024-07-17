#include "MathInit.h"

//�ۑ�����t�@�C���̔ԍ�
const int saveFile1 = 1;
const int saveFile2 = 2;

MathInit::MathInit()
{
	//�X�e�[�W�̍ŏ��T�C�Y
	const int stageSizeMin = 5;
	//�X�e�[�W�̍ő�T�C�Y
	const int stageSizeMax = 18;
	mathVolume_.x = (rand() % (stageSizeMax - stageSizeMin)) + stageSizeMin;
	mathVolume_.y = (rand() % (stageSizeMax - stageSizeMin)) + stageSizeMin;
	mathVolume_.z = mathVolume_.y;

	Math_Resize(mathVolume_.x, mathVolume_.z, &math_);

	//�}�X�̗ʏ�������
	MathVolumeWrite();

	//���ׂ�Floor�ŏ�����
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.z; y++)
		{
			math_[x][y].mathType_ = MATH_DELETE;
		}
	}
	saveNum_ = saveFile1;
	//��������
	Write();
	saveNum_ = saveFile2;
	Write();
}
