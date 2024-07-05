#include "Node.h"

Node::Node(Node* _parent, Work* _work)
	:parent_(_parent), myWork_(_work)
{
}

NodeState Node::Run()
{
	NodeState result = myWork_->operator()();

	//�����̃m�[�h���s��������
	switch (result)
	{
	case NodeState::RUNNING:
		ChildExcute();
		break;
	case NodeState::SUCCESS:
		//�q�m�[�h���s
		break;
	case NodeState::FAILED:
		//�e�ɒl�Ԃ�
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
