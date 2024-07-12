#pragma once
#include "../../StageOrigin.h"

class Text;
class Button;
class GamePlayer;
class MathManager;

class MapEditScene
	:public GameObject, public StageOrigin
{
	int hPict_[MATH_MAX];
	
	int hTgtgRoute_;

	//���X�̃}�X�̏��
	std::vector<std::vector<MATHDEDAIL>> math_origin_;
	//true : �I�����ꂽ�R���x�A�}�X�����
	//false : �R���x�A�}�X�͉��Ȃ�
	std::vector<std::vector<bool>> isConvRot_;

	GamePlayer* pGP_;

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


//�}�E�X�̑���ɂ���
private:
	//�}�E�X�̈ʒu
	XMFLOAT3 mousePos_;
public:
	//�}�E�X�̈ʒu�̑��
	void MousePosSet();

//�R�X�g�Ǘ��ɂ���
private:
	//�u�����Q���̐�
	int costLimit_;
	//�����Ă��Q���̐�
	int curCost_;
	//���ꂼ��̃}�X�̃R�X�g
	std::vector<int> costs_;
	Text* pText_;
public:
	//�߂�l
	//true : �܂��u����
	//false : ����ȏ�u���Ȃ�
	bool CostManagement(XMFLOAT3 _selectMath);
	void CostDraw();


//�}�X�̑I���ɂ���
private:
	//�I�΂�Ă�}�X�̎��
	int mathtype_;
	//�J�[�\�����u����Ă�}�X�̈ʒu
	XMFLOAT3 selectMath_;

	MathManager* table_;
public:
	//�}�X�̏�����
	void MathInit();
	//�}�X�̕\��
	void MathDraw();

	//�J�[�\�����u����Ă�}�X�̈ʒu�̒l�����
	virtual void SelectMathSet();
	//�}�X�^�C�v�I��
	virtual void SelectMathType();

	//�N���b�N�����}�X��I�����Ă�}�X�ɕς���
	void ChangeSelectMath();

	int GetMathType() { return mathtype_; }
	void SetMathType(int _type) { mathtype_ = _type; }
	XMFLOAT3 GetSelectMath() { return selectMath_; }
	void SetSelectMath(XMFLOAT3 _select) { selectMath_ = _select; }


//�{�^���ɂ���
private:
	//��ʏ�̃}�X�̎�ނ�I������{�^��
	Button* pMathButton_[MATH_MAX];
	//�����{�^��
	Button* pCompleteButton_;
	//�e�X�g�v���C�{�^��
	Button* pTestplayButton_;
	//���~�{�^��
	Button* pCancelButton_;
	//�e�N�X�`���̃T�C�Y
	XMFLOAT3 texture_size_;
public:
	//�}�X�I���{�^���̏�����
	void ButtonInit();
	//���ꂼ��̃{�^���������ꂽ���̏���
	void ButtonUpdate();
	//�}�X�I���{�^���ȊO�̃{�^���������ꂽ�Ƃ�
	void OtherButtonPush();

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


//�Ƃ��Ƃ��ɂ���
private:
	//�Ƃ��Ƃ��}�X���������ʒu
	XMFLOAT3 tgtgRouteMathDown_;
	//�Ƃ��Ƃ��}�X�������ăh���b�O���Ęb�����ʒu
	XMFLOAT3 tgtgRouteMathUp_;
public:
	//�Ƃ��Ƃ����[�g�̐ݒ�
	void TogetogeRouteSet();
	//�Ƃ��Ƃ��}�X���ʂ̃}�X�ɕς�������̗v�f�̍폜
	void TogetogeElemDelete();


private:
	//���y�̃n���h��
	int hAudio_Music_;
	//�}�X��u�����Ƃ��ɂȂ�SE�̃n���h��
	int hSE_PutMath_;
	//�R���x�A����]�������Ƃ���SE�̃n���h��
	int hSE_ConvRot_;
	
public:
	void AudioInit();
};