#include "Enemy.h"


#include "./AI/AStar.h"
#include "../Engine/RootNode.h"
#include "AI/AIWork.h"
#include "AI/EnemyAI.h"

#include "../StageOrigin.h"

//それぞれのマスの危険度
const int wallRisk = 500;
const int holeRisk = 5;
const int convRisk = 1;
const int togeRisk = 10;
const int pitRisk = 1;
const int timedecRisk = 1;
const int confRisk = 5;
const int startRisk = 1;
const int goalRisk = 1;
const int floorRisk = 1;

Enemy::Enemy(GameObject* _parent)
	:Player(_parent),
	//AIについて
	rootNode_(new RootNode(this, nullptr))
{
	//オブジェクトネーム決め
	objectName_ = "Enemy";

	//ルートノードに子ノード追加
	AddChildNode();

	risks_.resize(MATH_MAX);
	risks_ = {
		wallRisk,
		holeRisk,
		convRisk,
		togeRisk,
		pitRisk,
		timedecRisk,
		confRisk,
		startRisk,
		goalRisk,
		floorRisk,
	};
}

Enemy::~Enemy()
{
}

void Enemy::PlayerOperation()
{
	rootNode_->Run();
}

void Enemy::AddChildNode()
{
	Node* selectDir = new MoveDirSet(rootNode_, new PlayerMoveDirSelect());
	Node* walk = new Action_StateWalk(selectDir, new Action_Walk());
}

void Enemy::SelectMoveDir()
{
	std::vector<std::vector<int>> table;
	table.resize(mathVolume_.x);
	for (int x = 0; x < table.size(); x++)
	{
		table[x].resize(mathVolume_.z);
		for (int y = 0; y < table[x].size(); y++)
		{
			table[x][y] = risks_[math_[x][y].mathType_];
		}
	}
	//Astar呼ぶ
	Astar* astar = new Astar;
	XMFLOAT2 startPos = { transform_.position_.x, transform_.position_.z };
	XMFLOAT2 goalPos = { goalPos_.x, goalPos_.z };
	astar->SetTable(table, mathVolume_.x, mathVolume_.z, startPos, goalPos);
	astar->Excute();

	std::vector<XMFLOAT2> path(astar->GetRouteSize());
	path = astar->GetRoute();
	XMFLOAT2 destPos;
	if (path.size() > 1)	destPos = path[1];
	else
	{
		return;
	}

	PLAYER_MOVEDIR ret = DIR_W;
	switch ((int)(destPos.x - transform_.position_.x))
	{
	case 1:
		ret = DIR_D;
		break;
	case -1:
		ret = DIR_A;
		break;
	}
	switch ((int)(destPos.y - transform_.position_.z))
	{
	case 1:
		ret = DIR_W;
		break;
	case -1:
		ret = DIR_S;
		break;
	}
	SetMoveDir(ret);
}
