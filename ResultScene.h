#pragma once
#include "Engine/GameObject.h"
#include <iostream>
#include <fstream>
#include <vector>

class Button;

class ResultScene
	:public GameObject
{
	//playerがクリアしたかどうかの値を受け取る
	std::vector<bool> isClear_Player_;

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

	//playerのリザルトを読み込む
	void ResultRead();
};