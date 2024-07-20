#pragma once
#include <vector>
#include "Work.h"

//�e�q�֌W�Ɠ�����������Node
class Node
{
public:
	Node(Node* _parent, Work* _work);
	~Node();
	NodeState Run();
	virtual NodeState Result(NodeState _result);
	//�q�m�[�h���s
	NodeState ChildExcute();

	//�q�m�[�h�ǉ�
	void ChildPushBack(Node* _node);
	//�q�m�[�h�폜
	void ChildRelease();
	//RootNode�T��
	Node* GetRootNode();

protected:
	Node* parent_;
	std::vector<Node*> children_;

	Work* myWork_;
};