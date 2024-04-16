#pragma once
#include "Engine/GameObject.h"
#include <iostream>
#include <fstream>
#include <vector>

class Button;
class SceneTransition;

class ResultScene
	:public GameObject
{
	//playerがクリアしたかどうかの値を受け取る
	std::vector<bool> isClear_Player_;

	Button* pButton_;
	//ボタンの画像番号
	int hButton_;
	SceneTransition* pST_;
public:
	ResultScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	//次のターンに行くボタンの初期化
	//引数：ロードするファイルネーム
	void ButtonInit(std::string _fileName);
};