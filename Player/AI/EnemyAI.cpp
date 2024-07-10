#include "EnemyAI.h"

#include "AIWork.h"

#include "../../Engine/RootNode.h"

Node_SetMoveDir::Node_SetMoveDir(Node* _parent, Work* _work)
	:Node(_parent, _work)
{
	dynamic_cast<PlayerMoveDirSelect*> (myWork_)->SetPlayer(
		(Player*)(dynamic_cast<RootNode*>(this->GetRootNode())->GetGameObject_Parent()));
}

Node_Action_StateWalk::Node_Action_StateWalk(Node* _parent, Work* _work)
	:Node(_parent, _work)
{
	dynamic_cast<Action_Walk*> (myWork_)->SetPlayer(
		(Player*)(dynamic_cast<RootNode*>(this->GetRootNode())->GetGameObject_Parent()));
}

Node_Action_StateJump::Node_Action_StateJump(Node* _parent, Work* _work)
	:Node(_parent, _work)
{
	dynamic_cast<Action_Jump*> (myWork_)->SetPlayer(
		(Player*)(dynamic_cast<RootNode*>(this->GetRootNode())->GetGameObject_Parent()));
}
