#include <vector>
#include "Node.h"

Node::Node(Node* _parent, Work* _work)
	:parent_(_parent), myWork_(_work)
{
	if(parent_ != nullptr)	parent_->ChildPushBack(this);
}


NodeState Node:: Run()
{
	NodeState result = NodeState::RUNNING;
	if(myWork_ != nullptr)	result = myWork_->operator()();

	result = Result(result);

	return result;
}

NodeState Node::Result(NodeState _result)
{
	NodeState ret = NodeState::RUNNING;
	switch (_result)
	{
	case NodeState::RUNNING:
		ret = ChildExcute();
		break;
	case NodeState::SUCCESS:
	case NodeState::FAILED:
		ret = _result;
	}
	return ret;
}

//子ノード実行
NodeState Node::ChildExcute()
{
	NodeState ret = NodeState::RUNNING;
	for (int i = 0; i < children_.size(); i++)
	{
		ret = children_[i]->Run();
	}
	return ret;
}

	//子ノード追加
void Node::ChildPushBack(Node* _node)
{
	children_.push_back(_node);
}

Node* Node::GetRootNode()
{
	if (this->parent_ == nullptr)
	{
		return this;
	}
	else
	{
		return this->parent_->GetRootNode();
	}
}
