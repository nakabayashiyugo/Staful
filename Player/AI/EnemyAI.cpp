#include "EnemyAI.h"

#include "AIWork.h"

#include "../../Engine/RootNode.h"

MoveDirSet::MoveDirSet(Node* _parent, Work* _work)
	:Node(_parent, _work)
{
	dynamic_cast<PlayerMoveDirSelect*> (myWork_)->SetPlayer(
		(Player*)(dynamic_cast<RootNode*>(this->GetRootNode())->GetGameObject_Parent()));
}

Action_StateWalk::Action_StateWalk(Node* _parent, Work* _work)
	:Node(_parent, _work)
{
	dynamic_cast<Action_Walk*> (myWork_)->SetPlayer(
		(Player*)(dynamic_cast<RootNode*>(this->GetRootNode())->GetGameObject_Parent()));
}
