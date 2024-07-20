#include "../../Engine/Node.h"

class Node_Enemy : public Node
{
public:
	Node_Enemy(Node* _parent, Work* _work);
};

class Node_Select_State : public Node_Enemy
{
public:
	Node_Select_State(Node* _parent, Work* _work);
	NodeState Result(NodeState _result) override;
};