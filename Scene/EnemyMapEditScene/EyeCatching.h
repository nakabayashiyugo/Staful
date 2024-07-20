#pragma once
#include "../../../Engine/GameObject.h"

class Timer;

class EyeCatching : public GameObject
{
	//画面を暗くする画像番号
	int hDarken_;
	//マップ制作中を示す画像番号
	int hMaking_, hMakingPoint_;
	//マップ制作中の「.」の数
	int makingPointNum_;

	Timer* makingPointTimer_;
public:
	EyeCatching(GameObject* _parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void EnemyMapEditMakingInit();
	void EnemyMapEditMakingDraw();
};

