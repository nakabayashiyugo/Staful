#include "Enemy.h"


#include "./AI/AStar.h"
#include "../Engine/RootNode.h"
#include "AI/AIWork.h"
#include "AI/EnemyAI.h"


#include "../Stage.h"
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

//アイドルステートの時の待つフレームの最大値
const int idleFrameMax = 30;

Enemy::Enemy(GameObject* _parent)
	:Player(_parent),
	//AIについて
	rootNode_(new RootNode(this, nullptr))
{
	//オブジェクトネーム決め
	objectName_ = "Enemy";

	//ルートノードに子ノード追加
	AddNode();

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

	//コスト入れる
	costs_.resize(mathVolume_.x);
	for (int x = 0; x < costs_.size(); x++)
	{
		costs_[x].resize(mathVolume_.z);
		for (int y = 0; y < costs_[x].size(); y++)
		{
			costs_[x][y] = risks_[math_[x][y].mathType_];
		}
	}
}

Enemy::~Enemy()
{
	delete rootNode_;
}

void Enemy::PlayerOperation()
{
	//何フレーム待つか
	static int idleFrame = (rand() % idleFrameMax);
	static int nowIdleFrame = 0;
	nowIdleFrame++;
	if (nowIdleFrame >= idleFrame)
	{
		rootNode_->Run();
	}
	//待つフレーム新しくする
	idleFrame = (rand() % idleFrameMax);
	nowIdleFrame = 0;
	
}

void Enemy::OnCollision(GameObject* pTarget)
{
	//死んだとき直前までいたマスのコストを上げる
	const int deadCost = 100;
	costs_[prevPos_.x][prevPos_.z] = deadCost;
	Player::OnCollision(pTarget);
}

void Enemy::AddNode()
{
	Node* selectDir = new Node_Enemy(rootNode_, new PlayerMoveDirSelect());
	Node* selectState = new Node_Select_State(selectDir, new Select_State());
	Node* walk = new Node_Enemy(selectState, new Action_Walk());
	Node* jump = new Node_Enemy(selectState, new Action_Jump());
}

void Enemy::SelectMoveDir()
{
	if (!Is_InSide_Table(transform_.position_))
	{
		return;
	}
	//Astar呼ぶ
	Astar* astar = new Astar;
	XMFLOAT2 startPos = { transform_.position_.x, transform_.position_.z };
	XMFLOAT2 goalPos = { goalPos_.x, goalPos_.z };
	astar->SetTable(costs_, mathVolume_.x, mathVolume_.z, startPos, goalPos);
	//経路探索実行
	astar->Excute();
	//スタートからゴールまでのパス取得
	std::vector<XMFLOAT2> path(astar->GetRouteSize());
	path = astar->GetRoute();

	XMFLOAT2 destPos;
	if (!path.empty())	destPos = *(path.begin() + 1);
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

bool Enemy::IsJumpWise()
{
	destPos_ = GetDestPos();
	//移動先が穴だったら
	if (GetMathType(destPos_).mathType_ == MATH_HOLE)
	{
		return true;
	}
	//移動先のマスにとげとげがあったら
	else if (((Stage*)FindObject("Stage"))->IsMathOnTogetoge(destPos_))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Enemy::GetCost(int x, int y)
{
	return costs_[x][y];
}

void Enemy::SetCost(int _cost, int x, int y)
{
	costs_[x][y] = _cost;
}
