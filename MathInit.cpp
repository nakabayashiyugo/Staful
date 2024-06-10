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
	saveElem_.mathVolue_.x = (rand() % (stageSizeMax - stageSizeMin)) + stageSizeMin;
	saveElem_.mathVolue_.z = (rand() % (stageSizeMax - stageSizeMin)) + stageSizeMin;
	//�}�X�T�C�Y�ύX
	Math_Resize(saveElem_.mathVolue_.x, saveElem_.mathVolue_.z, &saveElem_.math_);
	//�^�[����(�ŏ��̃^�[���������̃N���X�Ă΂�Ȃ��̂�1)
	saveElem_.turn_ = 1;
	saveNum_ = saveFile1;
	//��������
	Write();
	saveNum_ = saveFile2;
	Write();
}
