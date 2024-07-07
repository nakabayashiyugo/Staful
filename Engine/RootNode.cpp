#include "RootNode.h"

RootNode::RootNode(GameObject* _parent, Work* _work)
	:parentGameObject_(_parent), Node(nullptr, _work)
{
}
