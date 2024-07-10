#include "../../Engine/Node.h"

//移動方向決めるNode
class Node_SetMoveDir : public Node
{
public:
	Node_SetMoveDir(Node* _parent, Work* _work);
};

//playerのステートをWalkにするNode
class Node_Action_StateWalk : public Node
{
public:
	Node_Action_StateWalk(Node* _parent, Work* _work);
};

//playerのステートをJumpにするNode
class Node_Action_StateJump : public Node
{
public:
	Node_Action_StateJump(Node* _parent, Work* _work);
};

class Node_Select_State : public Node
{
public:
	Node_Select_State(Node* _parent, Work* _work);
	NodeState Result(NodeState _result) override;
};