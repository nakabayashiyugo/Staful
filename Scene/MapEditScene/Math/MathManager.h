#pragma once
#include <vector>
#include "../../../StageOrigin.h"

class Math;

class MathManager : public StageOrigin
{
	std::vector<std::vector<Math*>> maths_;
	std::vector<int> costs_;	//���ꂼ��̃}�X�̎�ނ̃R�X�g
public:
	//����1,2�F�X�e�[�W�̃T�C�Y
	//����3�F�R�X�g�������Ă�z��̐擪�̃A�h���X
	//����4�F�R�X�g�������Ă�z��̃T�C�Y
	MathManager(int _xsize, int _ysize, std::vector<int> _costs, int _num);

public:
	//�����P�F�Z�b�g����v�f��
	//�����Q�F�Z�b�g����}�X�̃^�C�v
	void SetMathType(XMFLOAT2 _pos, int _type);
	//�����P�F�Q�b�g����v�f��
	int GetMathType(XMFLOAT2 _pos);
	//����1�F�Q�b�g����v�f��
	Transform GetMathTransform(XMFLOAT2 _pos);
	//����1�F�Z�b�g����v�f��
	//����2�F�Z�b�g����g�����X�t�H�[��
	void SetMathTransform(XMFLOAT2 _pos, Transform _transform);
	//�����P�F�Q�b�g����}�X�̃^�C�v
	int GetCost(int _type);
	//���݂̃R�[�X�ɂ������Ă�R�X�g�擾
	int GetStageCost();
};

