#pragma once
#include "Engine/GameObject.h"
#include "Engine/Direct3D.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <vector>
class SceneTransition;

enum MATHTYPE
{
	MATH_WALL = 0,		//��
	MATH_HOLE,		//��
	MATH_CONVEYOR,	//�R���x�A
	MATH_TOGETOGE,	//�Ƃ��Ƃ��̂��
	MATH_PITFALL,	//���Ƃ���
	MATH_TIMELIMITDEC, //�������Ԍ���
	MATH_CONFUSION,	//����
	MATH_START,		//�X�^�[�g�}�X
	MATH_GOAL,		//�S�[���}�X
	MATH_DELETE,		//��
	MATH_MAX,
};

struct MATHDEDAIL
{
	MATHTYPE mathType_; //�}�X�̎��
	Transform mathPos_; //�}�X�̈ʒu
};

struct TOGETOGEROUTE
{
	Transform route_;	//�ړ��͈͂��������̃g�����X�t�H�[��
	XMFLOAT3 initPos_;	//�Ƃ��Ƃ��̍ŏ��̈ʒu
	XMFLOAT3 destPos_;	//�Ƃ��Ƃ��̉������铞�B�n�_
};

//�}�X�̐�
struct MATHVOLUME
{
	int x;
	int y;
	int z;
};
//�}�X�̑傫��
const int MATHSIZE = 30;
//Floor�̃��f���̍���
const int MODELHEIGHT = 1;

class StageOrigin
{
protected:
	//���ׂẴ}�X�̏��
	std::vector<std::vector<MATHDEDAIL>> math_;
	//�Ƃ��Ƃ����ړ�����͈�
	std::vector<TOGETOGEROUTE> tTgtgRoute_;

	//�t�@�C���l�[���̊�{
	std::vector<std::string> fileNameInit_;

	//�}�X�̗�
	MATHVOLUME mathVolume_;

	//�Q�[�����v���C���ɕۑ�����t�@�C���̓����Ă�t�H���_��
	std::string saveFolderName = "SaveFile\\";

	//�ۑ�����t�@�C���ԍ�
	int saveNum_;
	//�^�[����
	int turnNum_;
	//�v���C���[�̔ԍ�
	int playerNum_;
public:
	StageOrigin();
	//�}�X�̃T�C�Y�ύX
	void Math_Resize(int _xsize, int _zsize, std::vector<std::vector<MATHDEDAIL>>* _math);
	
	void Math_Resize(int _xsize, int _zsize, std::vector<std::vector<int>>* _math);
	
	void Math_Resize(int _xsize, int _zsize, std::vector<std::vector<bool>>* _math);
	
	//�����̃N���X�ɂ���math_��vector�ւ̑��
	void SetTableMath(std::vector<std::vector<MATHDEDAIL>> _math);

	void SetTogetogeRoute(std::vector<TOGETOGEROUTE> _tgtgRoute);

	MATHVOLUME GetMathVolume() { return mathVolume_; }

	//�}�X��񏑂�����
	virtual void Write();
	//�}�X���ǂݍ���
	virtual void Read();
	//�}�X�̗ʏ�������
	void MathVolumeWrite();
	//�}�X�̗ʓǂݍ���
	void MathVolumeRead();
	

};

