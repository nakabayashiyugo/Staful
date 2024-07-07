#include "../../Engine/Node.h"

//移動方向決めるNode
class MoveDirSet : public Node
{
public:
	MoveDirSet(Node* _parent, Work* _work);
};

//playerのステートをWalkにするNode
class Action_StateWalk : public Node
{
public:
	Action_StateWalk(Node* _parent, Work* _work);
};