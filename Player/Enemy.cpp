#include "Enemy.h"

#include "../Engine/RootNode.h"
#include "AI/AIWork.h"
#include "AI/EnemyAI.h"

Enemy::Enemy(GameObject* _parent)
	:Player(_parent),
	//AI�ɂ���
	rootNode_(new RootNode(this, nullptr))
{
	//�I�u�W�F�N�g�l�[������
	objectName_ = "Enemy";

	//���[�g�m�[�h�Ɏq�m�[�h�ǉ�
	AddChildNode();
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

PLAYER_MOVEDIR Enemy::SelectMoveDir()
{

}
