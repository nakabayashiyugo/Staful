#pragma once
#include "Engine/GameObject.h"

class Button
	:public GameObject
{
	//クリックしたかどうか
	bool isClick_;
	//クリックして離したかどうか
	bool isRelease_;
	//選ばれてるかどうか
	bool isSelect_;
	//このボタンが押せるかどうか
	//true : 押せる
	//false : 押せない
	bool isCanPush_;
	//カーソルが合うか
	bool isCursorMatched_;
	//ボタンを表示するかどうか
	bool isDisp_;
	
	//ボタンのSEのハンドル
	int hSE_;

	int hPict_;

	Transform tPict_;

	XMFLOAT3 mousePos_;

public:
	Button(GameObject* parent);
	Button(std::string _btnName, GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//マウスの位置の(0, 0)を画面の中心にする
	void MousePosSetting();

	//ボタンの中にマウスカーソルが入ってるか
	bool IsMouseEnter();

	//ボタンが押されたらtrue
	bool OnClick();

//セッターゲッター類
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

	//ボタンを表示するかのセッター
	void SetIsDisplay(bool _isDisp) { isDisp_ = _isDisp; }
};