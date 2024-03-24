#pragma once
#include "Engine/GameObject.h"

class Button;

class TitleScene
	:public GameObject
{
	int hStartButton_, hStaful_;

	Transform tStartButton_, tStaful_;

	Button* pStartButton_;
public:
	TitleScene(GameObject* parent);

	//‰Šú‰»
	void Initialize() override;

	//XV
	void Update() override;

	//•`‰æ
	void Draw() override;

	//ŠJ•ú
	void Release() override;
};