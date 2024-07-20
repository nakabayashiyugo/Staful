#include "EnemyAI.h"

#include "AIWork.h"

#include "../../Engine/RootNode.h"

Node_Enemy::Node_Enemy(Node* _parent, Work* _work)
	:Node(_parent, _work)
{
	//myWork_‚ÌSetPlayer()‚ð‚â‚é
	dynamic_cast<PlayerActionBase*> (myWork_)->SetPlayer(
		(Player*)(dynamic_cast<RootNode*>(this->GetRootNode())->GetGameObject_Parent()));
}


Node_Select_State::Node_Select_State(Node* _parent, Work* _work)
	:Node_Enemy(_parent, _work)
{
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