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
	//�ő�̃R�X�g(�������ɂ��ꂪ�ŏ��ɂȂ邱�ƂȂ��ł���)
	const int costMax = 99999999;
	Astar_Node* ret = new Astar_Node(nullptr, {0, 0});
	ret->SetActualCost(costMax);
	for (int i = 0; i < childList_.size(); i++)
	{
		//�Z��Ńo�g��
		if (childList_[i]->GetState() != STATE_CLOSED && ret->GetCost() > childList_[i]->GetCost())
		{
			ret = childList_[i];
		}
		//�����̎q���Ƃ��o�g��
		Astar_Node* child = childList_[i]->GetFewCostChild();
		if (ret->GetCost() > child->GetCost())
		{
			ret = child;
		}
	}
	return ret;
}

void Astar_Node::GetPath(std::vector<Pos> &_path)
{
	_path.push_back(this->GetNodePos());
	if (this->GetParentNode() == nullptr)
	{
		return;
	}
	this->GetParentNode()->GetPath(_path);
}


//�ړ��\����
const int dirVertical[MAX] = { -1, 0, 1, 0 };
const int dirBeside[MAX] = { 0, 1, 0, -1 };

Astar::Astar()
{
}

Astar::~Astar()
{
}

void Astar::SetTable(std::vector<std::vector<int>> _table, int _xsize, int _ysize, Pos _start, Pos _goal)
{
	startPos_ = _start;
	goalPos_ = _goal;
	rootNode_ = new Astar_Node(nullptr, startPos_);

	table_.resize(_xsize);
	for (int x = 0; x < table_.size(); x++)
	{
		table_[x].resize(_ysize);
		for (int y = 0; y < table_[x].size(); y++)
		{
			table_[x][y] = _table[x][y];
		}
	}
	NodeOpen(rootNode_);
}

Pos Astar::Excute()
{
	//��m�[�h�̌���(�ŏ���rootNode)
	static Astar_Node* checkNode = rootNode_;
	while (checkNode->GetNodePos().x != goalPos_.x || checkNode->GetNodePos().y != goalPos_.y)
	{
		//��m�[�h�̎����open
		NeighborNodeOpen(checkNode);

		//��m�[�h��close(NeighborNodeOpen�ł���Ă�)

		//��m�[�h�̌���
		checkNode = BaseNodeSearch();
	}

	checkNode->GetPath(route_);
	//route_���Q�Ƃ���l
	static int refVal = route_.size() - 2;
	Pos ret = route_[refVal];
	refVal--;
	return ret;
}

void Astar::NodeOpen(Astar_Node* _node)
{
	_node->ChangeState(STATE_OPEN);
	CalcCost(_node);
}

void Astar::NeighborNodeOpen(Astar_Node* _node)
{
	for (int i = 0; i < MAX; i++)
	{
		Pos openPos;
		//���肪�͈͊O�łȂ����
		if (_node->GetNodePos().x + dirBeside[i] >= 0 &&
			_node->GetNodePos().x + dirBeside[i] < table_.size())
		{
			openPos.x = _node->GetNodePos().x + dirBeside[i];
		}
		else
		{
			continue;
		}
		if (_node->GetNodePos().y + dirVertical[i] >= 0 &&
			_node->GetNodePos().y + dirVertical[i] < table_[0].size())
		{
			openPos.y = _node->GetNodePos().y + dirVertical[i];
		}
		else
		{
			continue;
		}

		if(_node->GetState() == STATE_OPEN)		NodeOpen(new Astar_Node(_node, openPos));
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
	//���R�X�g
	if (_node->GetParentNode() != nullptr)
	{
		_node->SetActualCost(_node->GetParentNode()->GetActualCost() + table_[_node->GetNodePos().x][_node->GetNodePos().y]);
	}
	else
	{
		_node->SetActualCost(0);
	}
	//����R�X�g�v�Z
	_node->SetHeuristicCost(abs(_node->GetNodePos().x - goalPos_.x) + abs(_node->GetNodePos().y - goalPos_.y));
}