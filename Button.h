#pragma once
#include "Engine/GameObject.h"

class Button
	:public GameObject
{
	bool isClick_;
	bool isRelease_;

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

	bool GetIsClicked() { return isClick_; }
	bool GetIsReleased() { return isRelease_; }
};