#pragma once
#include "Engine/GameObject.h"

class Button
	:public GameObject
{
	//クリックしたかどうか
	bool isClick_;
	//クリックして離したかどうか
	bool isRelease_;
	//このボタンが押せるかどうか
	//true : 押せる
	//false : 押せない
	bool isCanPush_;

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

	void SetPictNum(int _pict);
	void SetTransform(Transform _transform) { tPict_ = _transform; }
	Transform GetTransform() { return tPict_; }

	bool GetIsClicked() { return isClick_; }
	bool GetIsReleased() { return isRelease_; }

	void SetAlpha(int _alpha);

	void SetIsCanPush(bool _isPush) { isCanPush_ = _isPush; }
};