#include "../../Engine/Node.h"

//�ړ��������߂�Node
class Node_SetMoveDir : public Node
{
public:
	Node_SetMoveDir(Node* _parent, Work* _work);
};

//player�̃X�e�[�g��Walk�ɂ���Node
class Node_Action_StateWalk : public Node
{
public:
	Node_Action_StateWalk(Node* _parent, Work* _work);
};

//player�̃X�e�[�g��Jump�ɂ���Node
class Node_Action_StateJump : public Node
{
public:
	Node_Action_StateJump(Node* _parent, Work* _work);
};