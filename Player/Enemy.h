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

	//当たり判定
	void OnCollision(GameObject* pTarget) override;

//AIについて(仮)
private:
	//ルートノード
	RootNode* rootNode_;

	//マスごとの危険度
	std::vector<int> risks_;
	//Astarに渡すマスごとのコスト
	std::vector<std::vector<int>> costs_;

public:
	//rootNode_に子供を追加する
	void AddNode();
	//移動方向決める
	void SelectMoveDir();

	int GetCost(int x, int y);
	void SetCost(int _cost, int x, int y);
};

