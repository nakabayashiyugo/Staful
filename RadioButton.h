#pragma once
#include <vector>

#include "Engine/GameObject.h"

class Button;

class RadioButton
{
	std::vector<Button*> radioButton_;
	//�{�^�����ŉ��s���邩
	int newLineNum_;
	//�{�^���̊��o
	XMFLOAT2 interval_;
	//��ƂȂ鍶��̃{�^���̈ʒu(���̂Ƃ��덶�ォ�牡���������ł��Ȃ�)
	XMFLOAT3 posBase_;
	//�{�^���̑傫��
	XMFLOAT3 buttonSize_;

public:
	//�����P�F�e�I�u�W�F�N�g
	//�����Q�F�摜�ԍ����������z��̃|�C���^
	//�����R�F�����Q�̔z��̃T�C�Y
	RadioButton(int _btn[], int _num);
	~RadioButton();

	int GetPushButtonHandle();

	//�{�^���Ƀ|�W�V�������Z�b�g
	void ButtonPosAssign();

//�Z�b�^�[�E�Q�b�^�[��
public:
	void SetNewLineNum(int _num) { newLineNum_ = _num; }
	void SetInterval(XMFLOAT2 _interval) { interval_ = _interval; }
	void SetPosBase(XMFLOAT3 _pos) { posBase_ = _pos; }
	void SetButtonSize(XMFLOAT3 _size) { buttonSize_ = _size; }

};

