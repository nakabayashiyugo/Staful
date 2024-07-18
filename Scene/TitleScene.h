#pragma once
#include "../Engine/GameObject.h"

class Button;
class BillBoard;

class TitleScene
	:public GameObject
{
	int hSoloPlayButton_, hTwoPlayButton_;
	int hStaful_;
	Transform tSoloPlayButton_, tTwoPlayButton_;
	Transform tStaful_;

	//ボタンのハンドル
	int soloPlayHandle_, twoPlayHandle_;
	
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

	void GameStateWrite(int _state);
};