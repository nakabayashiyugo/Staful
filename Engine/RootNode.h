#pragma once
#include "Node.h"
#include "GameObject.h"


class RootNode : public Node
{
public:
	RootNode(GameObject* _parent, Work* _work);

	GameObject* GetGameObject_Parent() { return parentGameObject_;}
private:
	GameObject* parentGameObject_;
};

