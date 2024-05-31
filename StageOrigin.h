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
	MATH_FLOOR,		//��
	MATH_WALL,		//��
	MATH_HOLE,		//��
	MATH_CONVEYOR,	//�R���x�A
	MATH_TOGETOGE,	//�Ƃ��Ƃ��̂��
	MATH_PITFALL,	//���Ƃ���
	MATH_TIMELIMITDEC, //�������Ԍ���
	MATH_START,		//�X�^�[�g�}�X
	MATH_GOAL,		//�S�[���}�X
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

class StageOrigin
{
protected:
	//�}�X�̑傫��
	const int MATHSIZE = 30;

	//Floor�̃��f���̍���
	const int MODELHEIGHT = 1;

	//���ׂẴ}�X�̏��
	std::vector<std::vector<MATHDEDAIL>> math_;
	//�Ƃ��Ƃ����ړ�����͈�
	std::vector<TOGETOGEROUTE> tTgtgRoute_;
	//�t�@�C���l�[���̊�{
	std::vector<std::string> fileNameInit_;

	SceneTransition* pST;

	//�}�X�̗�
	int XSIZE;
	int ZSIZE;

	//�Q�[�����v���C���ɕۑ�����t�@�C���̓����Ă�t�H���_��
	std::string saveFolderName = "SaveFile\\";
public:
	StageOrigin();
	//�}�X�̃T�C�Y�ύX
	void Math_Resize(int _xsize, int _zsize, std::vector<std::vector<MATHDEDAIL>> *_math)
	{
		_math->resize(_xsize);
		for (int x = 0; x < _xsize; x++)
		{
			_math->at(x).resize(_zsize);
		}
	}
	void Math_Resize(int _xsize, int _zsize, std::vector<std::vector<int>>* _math)
	{
		_math->resize(_xsize);
		for (int x = 0; x < _xsize; x++)
		{
			_math->at(x).resize(_zsize);
		}
	}
	void Math_Resize(int _xsize, int _zsize, std::vector<std::vector<bool>>* _math)
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
	//�����̃N���X�ɂ���math_��vector�ւ̑��
	void SetTableMath(std::vector<std::vector<MATHDEDAIL>> _math)
	{
		for (int x = 0; x < XSIZE; x++)
		{
			for (int z = 0; z < ZSIZE; z++)
			{
				math_.at(x).at(z) = _math.at(x).at(z);
			}
		}
	}

	void SetTogetogeRoute(std::vector<TOGETOGEROUTE> _tgtgRoute)
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
};

