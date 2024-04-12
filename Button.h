#pragma once
#include "Engine/GameObject.h"

class Button
	:public GameObject
{
	//�N���b�N�������ǂ���
	bool isClick_;
	//�N���b�N���ė��������ǂ���
	bool isRelease_;
	//���̃{�^���������邩�ǂ���
	//true : ������
	//false : �����Ȃ�
	bool isCanPush_;
	//�J�[�\����������
	bool isCursorMatched_;
	//�{�^����\�����邩�ǂ���
	bool isDisp_;

	int hPict_;

	Transform tPict_;

	XMFLOAT3 mousePos_;


public:
	Button(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	void SetPictNum(int _pict);
	void SetTransform(Transform _transform) { tPict_ = _transform; }
	Transform GetTransform() { return tPict_; }

	bool GetIsClicked() { return isClick_; }
	void SetIsClicked(bool _isClick) { isClick_ = _isClick; }
	bool GetIsReleased() { return isRelease_; }
	void SetIsReleased(bool _isRelease) { isRelease_ = _isRelease; }

	void SetAlpha(int _alpha);

	void SetIsCanPush(bool _isPush) { isCanPush_ = _isPush; }

	//�{�^����\�����邩�̃Z�b�^�[
	void SetIsDisplay(bool _isDisp) { isDisp_ = _isDisp; }
};