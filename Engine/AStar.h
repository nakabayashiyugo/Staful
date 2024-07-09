#pragma once
#include <iostream>
#include <vector>

#include "Direct3D.h"

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
	int actualCost_;		//実コスト
	int heuristicCost_;		//推定コスト
	Astar_Node* parent_;
	std::vector<Astar_Node*> childList_;
public:
	Astar_Node(Astar_Node* _parent, XMFLOAT2 _start);
	void ChangeState(ASTARSTATE _state);

	//子ノード追加
	void AddChildNode(Astar_Node* _node);
	//全ノード削除
	void RemoveAllNode();
	//ルートノード取得
	Astar_Node* GetRootNode();
	//子ノードの中で一番コストが小さいノード取得
	Astar_Node* GetFewCostChild();
	//経路探索が終わった後の経路取得
	void GetPath(std::vector<XMFLOAT2> &_path);
	//引数の位置のノードが存在するかチェック
	bool IsExistNode(XMFLOAT2 _pos);
//ゲッターセッター類
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
	//それぞれのマスを通るときにかかるコスト
	std::vector<std::vector<int>> table_;
	XMFLOAT2 startPos_;
	XMFLOAT2 goalPos_;
	Astar_Node* rootNode_;
	Astar_Node* checkNode_;
	//移動方向
	std::vector<XMFLOAT2> route_;
public:
	Astar();
	~Astar();

	void SetTable(std::vector<std::vector<int>> _table, int _xsize, int _ysize, XMFLOAT2 _start, XMFLOAT2 _goal);
	//経路探索開始
	void Excute();
	//ノードをオープンにする
	void NodeOpen(Astar_Node* _node);
	//周りのノードオープン
	void NeighborNodeOpen(Astar_Node* _node);
	//ノードをクローズする
	void NodeClose(Astar_Node* _node);
	//基準ノード探索
	Astar_Node* BaseNodeSearch();
	//コスト計算
	void CalcCost(Astar_Node* _node);
	//route_をリバースさせる
	void RouteReverse();

public:
	std::vector<XMFLOAT2> GetRoute() { return route_; }
	int GetRouteSize() { return route_.size(); }
};