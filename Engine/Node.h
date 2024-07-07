#pragma once
#include <vector>
#include "Work.h"

//親子関係と働きを持ったNode
class Node
{
public:
	Node(Node* _parent, Work* _work);
	NodeState Run();
	virtual NodeState Result(NodeState _result);
	//子ノード実行
	NodeState ChildExcute();

	//子ノード追加
	void ChildPushBack(Node* _node);
	//RootNode探索
	Node* GetRootNode();
protected:
	Node* parent_;
	std::vector<Node*> children_;

	Work* myWork_;
};