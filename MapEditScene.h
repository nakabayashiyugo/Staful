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

	//�I�΂�Ă�}�X�̎��
	int mathtype_;
	//�������ށE�ǂ݂����t�@�C��
	int saveNum_;
	//�u�����Q���̐�
	int mathChangeNumLimit_;
	//�����Ă��Q���̐�
	int mathChangeNum_;

	XMFLOAT3 texture_size_;

	//���X�̃}�X�̏��
	std::vector<std::vector<MATHDEDAIL>> math_origin_;
	//true : �I�����ꂽ�R���x�A�}�X�����
	//false : �R���x�A�}�X�͉��Ȃ�
	std::vector<std::vector<bool>> isConvRot_;

	Text* pText_;

	//��ʏ�̃}�X�̎�ނ�I������{�^��
	Button* pMathButton_[MATH_MAX];
	Button* pCompleteButton_;
	Button* pTestplayButton_;
	//�{�^���̔ԍ�
	int buttonNum_;

	//�e�X�g�v���C�N���A�������ǂ���
	//true : �N���A
	//false : ���N���A
	bool isClear_;
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

	void Write();
	void Read();

	bool isMathChangeNumLimit();

	//�N���b�N�����}�X��I�����Ă�}�X�ɕς���
	void ChangeSelectMath(XMFLOAT3 _selectMath);

	//�}�X�I���{�^���̏�����
	void ButtonInit(XMFLOAT3 _imageSize);
	int GetButtonNum() { return buttonNum_; }

	//�}�X�^�C�v�I��
	void SelectMathType();

	//�}�b�v�G�f�B�^�[��\�����邩���Ȃ���
	//���� false : �\��
	//�@�@ true : ��\��
	void IsHidden(bool _isHidden);

	void SetIsClear(bool _isClear) { isClear_ = _isClear; }
};

