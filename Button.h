#pragma once
#include "Engine/GameObject.h"

class Button
	:public GameObject
{
	//�N���b�N�������ǂ���
	bool isClick_;
	//�N���b�N���ė��������ǂ���
	bool isRelease_;
	//�I�΂�Ă邩�ǂ���
	bool isSelect_;
	//���̃{�^���������邩�ǂ���
	//true : ������
	//false : �����Ȃ�
	bool isCanPush_;
	//�J�[�\����������
	bool isCursorMatched_;
	//�{�^����\�����邩�ǂ���
	bool isDisp_;
	
	//�{�^����SE�̃n���h��
	int hSE_;

	int hPict_;

	Transform tPict_;

	XMFLOAT3 mousePos_;

public:
	Button(GameObject* parent);
	Button(std::string _btnName, GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	//�}�E�X�̈ʒu��(0, 0)����ʂ̒��S�ɂ���
	void MousePosSetting();

	//�{�^���̒��Ƀ}�E�X�J�[�\���������Ă邩
	bool IsMouseEnter();

	//�{�^���������ꂽ��true
	bool OnClick();

//�Z�b�^�[�Q�b�^�[��
public:
	void SetPictNum(int _pict);

	void SetTransform(Transform _transform) { tPict_ = _transform; }
	Transform GetTransform() { return tPict_; }

	bool GetIsClicked() { return isClick_; }
	void SetIsClicked(bool _isClick) { isClick_ = _isClick; }
	bool GetIsReleased() { return isRelease_; }
	void SetIsReleased(bool _isRelease) { isRelease_ = _isRelease; }
	bool GetIsSelect() { return isSelect_; }
	void SetIsSelect(bool _isSelect) { isSelect_ = _isSelect; }

	void SetAlpha(int _alpha);

	void SetIsCanPush(bool _isPush) { isCanPush_ = _isPush; }

	//�{�^����\�����邩�̃Z�b�^�[
	void SetIsDisplay(bool _isDisp) { isDisp_ = _isDisp; }
};