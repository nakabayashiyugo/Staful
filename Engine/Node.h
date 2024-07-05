#include <vector>
#include "Work.h"

//�e�q�֌W�Ɠ�����������Node
class Node
{
public:
	Node(Node* _parent, Work* _work);
	NodeState Run();
	virtual NodeState Result(NodeState _result);
	//�q�m�[�h���s
	NodeState ChildExcute();

	//�q�m�[�h�ǉ�
	void ChildPushBack(Work* _work);
private:
	Node* parent_;
	std::vector<Node*> children_;

	Work* myWork_;
};