#include "../../Engine/Node.h"

//�ړ��������߂�Node
class MoveDirSet : public Node
{
public:
	MoveDirSet(Node* _parent, Work* _work);
};

//player�̃X�e�[�g��Walk�ɂ���Node
class Action_StateWalk : public Node
{
public:
	Action_StateWalk(Node* _parent, Work* _work);
};