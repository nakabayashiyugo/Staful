#pragma once
#include "StageOrigin.h"

class Text;
class Button;

class MapEditScene
	:public GameObject, StageOrigin
{
	int hPict_[MATH_MAX];
	
	int hTgtgRoute_;

	//�}�X�̏c�����̗�
	int YSIZE;


	//���X�̃}�X�̏��
	std::vector<std::vector<MATHDEDAIL>> math_origin_;
	//true : �I�����ꂽ�R���x�A�}�X�����
	//false : �R���x�A�}�X�͉��Ȃ�
	std::vector<std::vector<bool>> isConvRot_;

public:
	MapEditScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

//�R�X�g�Ǘ��ɂ���
private:
	//�u�����Q���̐�
	int costLimit_;
	//�����Ă��Q���̐�
	int curCost_;
	//���ꂼ��̃}�X�̃R�X�g
	std::vector<int> costs_;
	//�R�X�g��\������ʒu
	XMFLOAT3 costTextPos_;
	Text* pText_;
public:
	bool costManagement();


//�t�@�C���̏������݁E�ǂݏo���ɂ���
private:
	//�������ށE�ǂ݂����t�@�C��
	int saveNum_;
public:
	void Write();
	void Read();


//�}�X�̑I���ɂ���
private:
	//�I�΂�Ă�}�X�̎��
	int mathtype_;
public:
	//�}�X�̏�����
	void MathInit();
	//�}�X�̕\��
	void MathDraw();
	//�}�X�^�C�v�I��
	void SelectMathType();
	//�N���b�N�����}�X��I�����Ă�}�X�ɕς���
	void ChangeSelectMath(XMFLOAT3 _selectMath);


//�{�^���ɂ���
private:
	//��ʏ�̃}�X�̎�ނ�I������{�^��
	Button* pMathButton_[MATH_MAX];
	Button* pCompleteButton_;
	Button* pTestplayButton_;
	//�{�^���̔ԍ�
	int buttonNum_;
	//�e�N�X�`���̃T�C�Y
	XMFLOAT3 texture_size_;
public:
	//�}�X�I���{�^���̏�����
	void ButtonInit();

	int GetButtonNum() { return buttonNum_; }


//�}�X�̐����ɂ���
private:
	//�}�X�̐����̉摜�ԍ�
	int hExpantion_[MATH_MAX];
	//�}�X�̐����̈ʒu
	Transform tExpantion_;
public:		
	//�}�X�̐����̕\���̏�����
	void ExpantionInit();
	//�}�X�̐����̕\����Draw
	void ExpantionDraw();


//�e�X�g�v���C�ɂ���
private:
	//�e�X�g�v���C�N���A�������ǂ���
	//true : �N���A
	//false : ���N���A
	bool isClear_;
	//�e�X�g�v���C���ł��邩�ǂ���
	bool canTest_;

	//�}�b�v�G�f�B�^�[��\�����邩
	//true : �\��
	//false : ��\��
	bool isDisp_;
public:
	//�}�b�v�G�f�B�^�[��\�����邩���Ȃ���
	//���� false : �\��
	//�@�@ true : ��\��
	void IsDisplay(bool _isDisp);
	void SetIsDisp(bool _isDisp) { isDisp_ = _isDisp; }
	//�e�X�g�v���C���ł��邩�ǂ���
	void CheckCanTest();
	//�N���A�������ǂ����̃Z�b�^�[
	void SetIsClear(bool _isClear) { isClear_ = _isClear; }
};

