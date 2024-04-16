#pragma once
#include "Engine/GameObject.h"

class Button;

class TitleScene
	:public GameObject
{
	int hStartButton_, hExitButton_;
	int hStaful_;
	Transform tStartButton_;
	Transform tStaful_;

	Button* pStartButton_;
public:
	TitleScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};