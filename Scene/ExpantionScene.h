#pragma once
#include "../Engine/GameObject.h"

class Button;
class BillBoard;

//ゲームの説明の画像の数
const int explanationNum = 3;

class ExpantionScene
	:public GameObject
{
	//それぞれの画像番号
	int hNext_, hExplanNextButton_, hExplanReturn_;
	int hExplan_[explanationNum];
	//画像の位置
	Transform tStartButton_, tExplanNextButton_, hExplanReturn_;
	Transform tExplan_[explanationNum];

	//ボタン
	Button* pNext_, *pExplanNextButton_, *pExplanReturn_;
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
};