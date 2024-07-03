#pragma once
#include "../Engine/GameObject.h"

//ゲームの説明の画像の数
const int explanationNum = 3;

class ExpantionScene
	:public GameObject
{
	//それぞれの画像番号
	int hNext_, hExplanNextButton_, hExplanBackButton_;
	int hExplan_[explanationNum];
	int hOverview_;
	//画像の位置
	Transform tNext_, tExplanNextButton_, tExplanBackButton_;
	Transform tExplan_[explanationNum];
	Transform tOverview_;

	//ボタンのハンドル
	int nextBtnHandle_, explanNextBtnHandle_, explanBackBtnHandle_;

	//ボタン番号
	int buttonNum_;

	//現在画面に映し出されているゲームの説明の番号
	float curExplanNum_;
	int destPos_, prevPos_;

	//説明画像が移動中か
	bool isMoving_;
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

	//説明画像の位置決め
	void ExplanPositioning();

	//説明画像移動
	void ExplanMove();
};