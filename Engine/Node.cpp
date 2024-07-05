#include <vector>
#include "Node.h"

Node::Node(Node* _parent, Work* _work)
	:parent_(_parent), myWork_(_work)
{
}


NodeState Node:: Run()
{
	NodeState result = myWork_->operator()();

	result = Result(result);

	return result;
}

NodeState Node::Result(NodeState _result)
{
	NodeState ret;
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
	NodeState ret;
	for (int i = 0; i < children_.size(); i++)
	{
		ret = children_[i]->Run();
	}
	return ret;
}

	//子ノード追加
void Node::ChildPushBack(Work* _work)
{
	Node* child = new Node(this, _work);
	children_.push_back(child);
	delete child;
}