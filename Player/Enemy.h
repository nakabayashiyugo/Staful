#pragma once
#include "Player.h"
#include "../StageOrigin.h"

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

	//マスごとの危険度
	std::vector<int> risks_;

public:
	//子供追加する
	void AddChildNode();
	//移動方向決める
	void SelectMoveDir();
};

