#pragma once
#include <vector>
#include "../../../StageOrigin.h"

class Math;

class MathManager
{
	std::vector<std::vector<Math*>> maths_;
	std::vector<int> costs_;	//���ꂼ��̃}�X�̎�ނ̃R�X�g
	int costLimit_;				//�z�u�\�ȃR�X�g��
public:
	//����1,2�F�X�e�[�W�̃T�C�Y
	//����3�F�}�X�̏����l
	//����4�F�R�X�g�������Ă�z��̐擪�̃A�h���X
	//����5�F�R�X�g�������Ă�z��̃T�C�Y
	//����6�F�z�u�\�ȃR�X�g��
	MathManager(int _xsize, int _ysize, int _initType, std::vector<int> _costs, int _num, int _costLimit);
	~MathManager();

	bool IsCostOver(XMFLOAT2 _pos, int _type);

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
	//�z�u�\�ȃR�X�g�擾
	int GetCostLimit() { return costLimit_; }
};

