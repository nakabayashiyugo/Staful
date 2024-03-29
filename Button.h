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

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void SetPictNum(int _pict) { hPict_ = _pict; }
	void SetTransform(Transform _transform) { tPict_ = _transform; }

	bool GetIsClicked() { return isClick_; }
	bool GetIsReleased() { return isRelease_; }
};