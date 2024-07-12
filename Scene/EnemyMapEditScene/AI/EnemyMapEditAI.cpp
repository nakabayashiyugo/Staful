#include "EnemyMapEditAI.h"

#include "MapEditAIWork.h"

#include "../../../Engine/RootNode.h"

EnemyMapEditAI::EnemyMapEditAI(Node* _parent, Work* _work)
	:Node(_parent, _work)
{
	dynamic_cast<MapEdit_Work*> (myWork_)->SetEditer(
		(MapEditScene*)(dynamic_cast<RootNode*>(this->GetRootNode())->GetGameObject_Parent()));
}
