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
	//�}�X��񏑂�����
	std::string savefile = saveFolderName + "saveMath";
	savefile += std::to_string(saveNum_);
	write.open(savefile, std::ios::out);
	//  �t�@�C�����J���Ȃ������Ƃ��̃G���[�\��
	if (!write) {
		std::cout << "�t�@�C�� " << savefile << " ���J���܂���";
		return;
	}
	for (int i = 0; i < mathVolume_.x; i++) {
		for (int j = 0; j < mathVolume_.z; j++)
		{
			write.write((char*)&math_[i][j], sizeof(math_[i][j]));
			//������ł͂Ȃ��f�[�^����������
		}
	}
	write.close();  //�t�@�C�������

	//�Ƃ��Ƃ����[�g�̔z��̃T�C�Y��������
	savefile = saveFolderName + "tgtgRouteVolume";
	savefile += std::to_string(saveNum_);
	write.open(savefile, std::ios::out);
	//  �t�@�C�����J���Ȃ������Ƃ��̃G���[�\��
	if (!write) {
		std::cout << "�t�@�C�� " << savefile << " ���J���܂���";
		return;
	}
	int tgtgSize = tTgtgRoute_.size();
	write.write((char*)&tgtgSize, sizeof(tgtgSize));
	//������ł͂Ȃ��f�[�^����������
	write.close();  //�t�@�C�������

	//�Ƃ��Ƃ����[�g�̏�������
	savefile = saveFolderName + "tgtgRoute";
	savefile += std::to_string(saveNum_);
	write.open(savefile, std::ios::out);
	//  �t�@�C�����J���Ȃ������Ƃ��̃G���[�\��
	if (!write) {
		std::cout << "�t�@�C�� " << savefile << " ���J���܂���";
		return;
	}
	for (int i = 0; i < tTgtgRoute_.size(); i++)
	{
		write.write((char*)&tTgtgRoute_[i], sizeof(TOGETOGEROUTE));
	}
	write.close();  //�t�@�C�������
}

void StageOrigin::Read()
{
	std::ifstream read;
	//�}�X���ǂݍ���
	std::string openfile = saveFolderName + "saveMath";
	openfile += std::to_string(saveNum_);
	read.open(openfile, std::ios::in);
	if (!read) {
		std::cout << "�t�@�C�����J���܂���";
		return;
	}
	//  �t�@�C�����J���Ȃ������Ƃ��̑΍�
	//�t�@�C���̍Ō�܂ő�����
	for (int i = 0; i < mathVolume_.x; i++)
	{
		for (int j = 0; j < mathVolume_.z; j++)
		{
			read.read((char*)&math_[i][j], sizeof(math_[i][j]));
			//������ł͂Ȃ��f�[�^��ǂ݂���
		}
	}
	read.close();  //�t�@�C�������

	//�Ƃ��Ƃ����[�g�̔z��̃T�C�Y�̓ǂݍ���
	openfile = saveFolderName + "tgtgRouteVolume";
	openfile += std::to_string(saveNum_);
	read.open(openfile, std::ios::in);
	//  �t�@�C�����J��
	//  ios::in �͓ǂݍ��ݐ�p  ios::binary �̓o�C�i���`��
	if (!read) {
		std::cout << "�t�@�C�����J���܂���";
		return;
	}
	//  �t�@�C�����J���Ȃ������Ƃ��̑΍�
	int tgtgSize = 0;
	read.read((char*)&tgtgSize, sizeof(tgtgSize));
	//������ł͂Ȃ��f�[�^��ǂ݂���
	tTgtgRoute_.resize(tgtgSize);
	read.close();  //�t�@�C�������


	//�Ƃ��Ƃ����[�g�̓ǂݍ���
	openfile = saveFolderName + "tgtgRoute";
	openfile += std::to_string(saveNum_);
	read.open(openfile, std::ios::in);
	if (!read) {
		std::cout << "�t�@�C�����J���܂���";
		return;
	}
	for (int i = 0; i < tgtgSize; i++)
	{
		TOGETOGEROUTE pTg;
		read.read((char*)&pTg, sizeof(TOGETOGEROUTE));
		tTgtgRoute_[i] = pTg;

	}


	read.close();  //�t�@�C�������
}

void StageOrigin::MathVolumeWrite()
{
	std::ofstream write;
	std::string savefile = saveFolderName + "mathVolume";

	write.open(savefile, std::ios::out);

	//  �t�@�C�����J���Ȃ������Ƃ��̃G���[�\��
	if (!write) {
		std::cout << "�t�@�C�� " << savefile << " ���J���܂���";
		return;
	}

	write.write((char*)&mathVolume_, sizeof(mathVolume_));
	//������ł͂Ȃ��f�[�^����������

	write.close();  //�t�@�C�������
}

void StageOrigin::MathVolumeRead()
{
	std::ifstream read;
	std::string openfile = saveFolderName + "mathVolume";

	read.open(openfile, std::ios::in);
	//  �t�@�C�����J��
	//  ios::in �͓ǂݍ��ݐ�p  ios::binary �̓o�C�i���`��

	if (!read) {
		std::cout << "�t�@�C�����J���܂���";
		return;
	}
	//  �t�@�C�����J���Ȃ������Ƃ��̑΍�

	read.read((char*)&mathVolume_, sizeof(mathVolume_));
			//������ł͂Ȃ��f�[�^��ǂ݂���
	read.close();  //�t�@�C�������
}
