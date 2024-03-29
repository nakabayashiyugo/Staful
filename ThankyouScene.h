#pragma once
#include "Engine/GameObject.h"

class Button;

class ThankyouScene
	:public GameObject
{
	int hReturnTitle_, hThankyou_;

	Transform tReturnTitle_, tThankyou_;
	XMFLOAT3 mousePos_;

	Button* pReturnButton_;
public:
	ThankyouScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;
};