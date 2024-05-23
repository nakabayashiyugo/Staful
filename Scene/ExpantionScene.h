#pragma once
#include "../Engine/GameObject.h"

class Button;

//ゲームの説明の画像の数
const int explanationNum = 3;

class ExpantionScene
	:public GameObject
{
	//それぞれの画像番号
	int hNext_, hExplanNextButton_, hExplanReturnButton_;
	int hExplan_[explanationNum];
	int hOverview_;
	//画像の位置
	Transform tNext_, tExplanNextButton_, tExplanReturnButton_;
	Transform tExplan_[explanationNum];
	Transform tOverview_;

	//ボタン
	Button* pNext_, *pExplanNextButton_, *pExplanReturnButton_;

	//ボタン番号
	int buttonNum_;
public:
	ExpantionScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//ボタンの初期化
	void ButtonInit();
	int GetButtonNum() { return buttonNum_; }
};