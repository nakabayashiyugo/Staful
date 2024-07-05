#include "Node.h"

Node::Node(Node* _parent, Work* _work)
	:parent_(_parent), myWork_(_work)
{
}

NodeState Node::Run()
{
	NodeState result = myWork_->operator()();

	//自分のノード実行した結果
	switch (result)
	{
	case NodeState::RUNNING:
		ChildExcute();
		break;
	case NodeState::SUCCESS:
		//子ノード実行
		break;
	case NodeState::FAILED:
		//親に値返す
		break;
	}
}

void Node::ChildExcute()
{
	for (int i = 0; i < children_.size(); i++)
	{
		children_[i]->Run();
	}
}

void Node::ChildPushBack(Work* _work)
{
	Node* child = new Node(this, _work);
	children_.push_back(child);
}
