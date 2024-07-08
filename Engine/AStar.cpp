#include "AStar.h"

Astar_Node::Astar_Node(Astar_Node* _parent, Pos _start)
	:parent_(_parent), pos_(_start)
{
	if(parent_ != nullptr)	parent_->AddChildNode(this);
}

void Astar_Node::ChangeState(ASTARSTATE _state)
{
	this->state_ = _state;
}

void Astar_Node::AddChildNode(Astar_Node* _node)
{
	childList_.push_back(_node);
}

void Astar_Node::RemoveChildNode(Astar_Node* _node)
{
	for (int i = 0; i < this->childList_.size(); i++)
	{
		if (childList_[i] == _node)
		{
			childList_.erase(childList_.begin() + i);
			break;
		}
	}
}

Astar_Node* Astar_Node::GetRootNode()
{
	if (this->parent_ == nullptr)
	{
		return this;
	}
	return this->parent_->GetRootNode();
}

Astar_Node* Astar_Node::GetFewCostChild()
{
	//最大のコスト(さすがにこれが最小になることないでしょ)
	const int costMax = 99999999;
	Astar_Node* ret;
	ret->SetActualCost(costMax);
	for (int i = 0; i < childList_.size(); i++)
	{
		//兄弟でバトル
		if (ret->GetCost() > childList_[i]->GetCost())
		{
			ret = childList_[i];
		}
		//自分の子供ともバトル
		Astar_Node* child = childList_[i]->GetFewCostChild();
		if (ret->GetCost() > child->GetCost())
		{
			ret = child;
		}
	}
	return ret;
}


//移動可能方向
const int dirVertical[DIR_MAX] = { -1, 0, 1, 0 };
const int dirBeside[DIR_MAX] = { 0, 1, 0, -1 };

Astar::Astar()
	:rootNode_(new Astar_Node(nullptr, startPos_))
{
}

Astar::~Astar()
{
}

void Astar::SetTable(std::vector<std::vector<int>> _table, int _xsize, int _ysize, Pos _start, Pos _goal)
{
	startPos_ = _start;
	goalPos_ = _goal;

	table_.resize(_xsize);
	for (int x = 0; x < table_.size(); x++)
	{
		table_[x].resize(_ysize);
		for (int y = 0; y < table_[x].size(); y++)
		{
			table_[x][y] = _table[x][y];
		}
	}
}

std::vector<DIR> Astar::Excute()
{
	//基準ノードの検索(最初はrootNode)
	static Astar_Node* checkNode = rootNode_;
	while (checkNode->GetNodePos().x != goalPos_.x || checkNode->GetNodePos().y != goalPos_.y)
	{
		//基準ノードの周りをopen
		NeighborNodeOpen(checkNode);

		//基準ノードのclose(NeighborNodeOpenでやってる)

		//基準ノードの検索
		checkNode = BaseNodeSearch();
	}

}

void Astar::NodeOpen(Astar_Node* _node)
{
	_node->ChangeState(STATE_OPEN);
	CalcCost(_node);
}

void Astar::NeighborNodeOpen(Astar_Node* _node)
{
	for (int i = 0; i < DIR_MAX; i++)
	{
		Pos openPos;
		//周りが範囲外でなければ
		if (_node->GetNodePos().x + dirVertical[i] >= 0 && 
			_node->GetNodePos().x + dirVertical[i] < table_.size())
		{
			openPos.x = _node->GetNodePos().x + dirVertical[i];
		}
		else
		{
			continue;
		}
		if (_node->GetNodePos().y + dirBeside[i] >= 0 &&
			_node->GetNodePos().y + dirBeside[i] < table_[0].size())
		{
			openPos.y = _node->GetNodePos().y + dirVertical[i];
		}
		else
		{
			continue;
		}

		NodeOpen(new Astar_Node(_node, openPos));
	}
	NodeClose(_node);
}

void Astar::NodeClose(Astar_Node* _node)
{
	_node->ChangeState(STATE_CLOSED);
}

Astar_Node* Astar::BaseNodeSearch()
{
	Astar_Node* baseNode;
	baseNode = rootNode_->GetFewCostChild();
	return baseNode;
}

void Astar::CalcCost(Astar_Node* _node)
{
	//実コスト
	static int cost = 0;
	cost += table_[_node->GetNodePos().x][_node->GetNodePos().y];
	_node->SetActualCost(cost);
	//推定コスト計算
	_node->SetHeuristicCost(abs(_node->GetNodePos().x - goalPos_.x) + abs(_node->GetNodePos().y - goalPos_.y));
}
