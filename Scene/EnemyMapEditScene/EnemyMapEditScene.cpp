#include "EnemyMapEditScene.h"

#include "../../Engine/RootNode.h"

#include "AI/MapEditAIWork.h"
#include "AI/EnemyMapEditAI.h"

EnemyMapEditScene::EnemyMapEditScene(GameObject* _parent)
	:MapEditScene(_parent),
	rootNode_(new RootNode(this, nullptr))
{
	this->objectName_ = "EnemyMapEditScene";

	AddNode();
}

void EnemyMapEditScene::Initialize()
{
	MapEditScene::Initialize();
	for (int i = 0; i < 100; i++)
	{
		rootNode_->Run();
	}
}

void EnemyMapEditScene::Update()
{
}

void EnemyMapEditScene::AddNode()
{
	Node* selectType = new EnemyMapEditAI(rootNode_, new MapEdit_MathTypeSelect());
	Node* costManage = new EnemyMapEditAI(selectType, new MapEdit_IsPutMath());
	Node* selectPos = new EnemyMapEditAI(costManage, new MapEdit_MathPosSelect());
	Node* putMath = new EnemyMapEditAI(selectPos, new MapEdit_Action_ChangeMath());
}

void EnemyMapEditScene::SelectMathSet()
{
	srand(time(NULL));
	XMFLOAT3 pos = XMFLOAT3((rand() % mathVolume_.x), (rand() % mathVolume_.z), 0);
	SetSelectMath(pos);
}

void EnemyMapEditScene::SelectMathType()
{
	SetMathType(0);
}
