#pragma once
#include <iostream>
#include <vector>

#include "../../Engine/Direct3D.h"

enum DIR
{
	UP = 0,
	DOWN,
	RIGHT,
	LEFT,
	MAX,
};

enum ASTARSTATE
{
	STATE_NONE = 0,
	STATE_OPEN,
	STATE_CLOSED,
};

class Astar_Node
{
	XMFLOAT2 pos_;
	ASTARSTATE state_;
	int actualCost_;		//���R�X�g
	int heuristicCost_;		//����R�X�g
	Astar_Node* parent_;
	std::vector<Astar_Node*> childList_;
public:
	Astar_Node(Astar_Node* _parent, XMFLOAT2 _start);
	void ChangeState(ASTARSTATE _state);

	//�q�m�[�h�ǉ�
	void AddChildNode(Astar_Node* _node);
	//�S�m�[�h�폜
	void RemoveAllNode();
	//���[�g�m�[�h�擾
	Astar_Node* GetRootNode();
	//�q�m�[�h�̒��ň�ԃR�X�g���������m�[�h�擾
	Astar_Node* GetFewCostChild();
	//�o�H�T�����I�������̌o�H�擾
	void GetPath(std::vector<XMFLOAT2> &_path);
	//�����̈ʒu�̃m�[�h�����݂��邩�`�F�b�N
	bool IsExistNode(XMFLOAT2 _pos);
//�Q�b�^�[�Z�b�^�[��
public:
	XMFLOAT2 GetNodePos() { return pos_; }
	void SetNodePos(XMFLOAT2 _pos) { pos_ = _pos; }
	ASTARSTATE GetState() { return state_; }
	int GetActualCost() { return actualCost_; }
	void SetActualCost(int _cost) { actualCost_ = _cost; }
	int GetHeuristicCost() { return heuristicCost_; }
	void SetHeuristicCost(int _cost) { heuristicCost_ = _cost; }
	int GetCost() { return actualCost_ + heuristicCost_; }
	Astar_Node* GetParentNode() { return parent_; }
	void SetParentNode(Astar_Node* _node) { parent_ = _node; }
	std::vector<Astar_Node*> GetChildList() { return childList_; }
};

class Astar
{
	//���ꂼ��̃}�X��ʂ�Ƃ��ɂ�����R�X�g
	std::vector<std::vector<int>> table_;
	XMFLOAT2 startPos_;
	XMFLOAT2 goalPos_;
	Astar_Node* rootNode_;
	Astar_Node* checkNode_;
	//�ړ�����
	std::vector<XMFLOAT2> route_;
public:
	Astar();
	~Astar();

	void SetTable(std::vector<std::vector<int>> _table, int _xsize, int _ysize, XMFLOAT2 _start, XMFLOAT2 _goal);
	//�o�H�T���J�n
	void Excute();
	//�m�[�h���I�[�v���ɂ���
	void NodeOpen(Astar_Node* _node);
	//����̃m�[�h�I�[�v��
	void NeighborNodeOpen(Astar_Node* _node);
	//�m�[�h���N���[�Y����
	void NodeClose(Astar_Node* _node);
	//��m�[�h�T��
	Astar_Node* BaseNodeSearch();
	//�R�X�g�v�Z
	void CalcCost(Astar_Node* _node);
	//route_�����o�[�X������
	void RouteReverse();

public:
	std::vector<XMFLOAT2> GetRoute() { return route_; }
	int GetRouteSize() { return route_.size(); }
};