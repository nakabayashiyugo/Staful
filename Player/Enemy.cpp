#include "Enemy.h"


#include "./AI/AStar.h"
#include "../Engine/RootNode.h"
#include "AI/AIWork.h"
#include "AI/EnemyAI.h"

#include "../StageOrigin.h"

//���ꂼ��̃}�X�̊댯�x
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
	//AI�ɂ���
	rootNode_(new RootNode(this, nullptr))
{
	//�I�u�W�F�N�g�l�[������
	objectName_ = "Enemy";

	//���[�g�m�[�h�Ɏq�m�[�h�ǉ�
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

	//�R�X�g�����
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
}

void Enemy::PlayerOperation()
{
	rootNode_->Run();
}

void Enemy::OnCollision(GameObject* pTarget)
{
	//���񂾂Ƃ����O�܂ł����}�X�̃R�X�g���グ��
	const int deadCost = 100;
	costs_[prevPos_.x][prevPos_.z] = deadCost;
	Player::OnCollision(pTarget);
}

void Enemy::AddNode()
{
	Node* selectDir = new Node_SetMoveDir(rootNode_, new PlayerMoveDirSelect());
	Node* selectState = new Node_Select_State(selectDir, new Select_State());
	Node* walk = new Node_Action_StateWalk(selectState, new Action_Walk());
	Node* jump = new Node_Action_StateJump(selectState, new Action_Jump());
}

void Enemy::SelectMoveDir()
{
	if (!Is_InSide_Table(transform_.position_))
	{
		return;
	}
	//Astar�Ă�
	Astar* astar = new Astar;
	XMFLOAT2 startPos = { transform_.position_.x, transform_.position_.z };
	XMFLOAT2 goalPos = { goalPos_.x, goalPos_.z };
	astar->SetTable(costs_, mathVolume_.x, mathVolume_.z, startPos, goalPos);
	//�o�H�T�����s
	astar->Excute();
	//�X�^�[�g����S�[���܂ł̃p�X�擾
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

int Enemy::GetCost(int x, int y)
{
	return costs_[x][y];
}

void Enemy::SetCost(int _cost, int x, int y)
{
	costs_[x][y] = _cost;
}
