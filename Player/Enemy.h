#pragma once
#include "Player.h"

class Enemy : public Player
{
private:

public:
	Enemy(GameObject* _parent);
	~Enemy();

	void PlayerOperation() override;

//AIについて(仮)
private:
	//ルートノード
	RootNode* rootNode_;

public:
	//子供追加する
	void AddChildNode();
	//移動方向決める
	void SelectMoveDir();
};

