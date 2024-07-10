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

Node_Select_State::Node_Select_State(Node* _parent, Work* _work)
	:Node(_parent, _work)
{
	dynamic_cast<Select_State*> (myWork_)->SetPlayer(
		(Player*)(dynamic_cast<RootNode*>(this->GetRootNode())->GetGameObject_Parent()));
}

NodeState Node_Select_State::Result(NodeState _result)
{
	NodeState ret = NodeState::RUNNING;
	auto itr = children_.begin();
	switch (_result)
	{
	case NodeState::RUNNING:
		ret = (*itr)->Run();
		break;
	case NodeState::SUCCESS:
	case NodeState::FAILED:
		itr++;
		ret = (*itr)->Run();
		break;
	}
	return ret;
	
}
