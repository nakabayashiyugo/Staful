#include "EnemyMapEditScene.h"

#include "../../Engine/RootNode.h"

#include "AI/MapEditAIWork.h"
#include "AI/EnemyMapEditAI.h"

#include "../MapEditScene/Math/MathManager.h"

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
	//マス配置の試行回数
	const int num = 100;
	for (int i = 0; i < num; i++)
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
	XMFLOAT3 pos = XMFLOAT3((rand() % mathVolume_.x), (rand() % mathVolume_.z), 0);
	if (GetTable()->GetMathType(XMFLOAT2(pos.x, pos.y)) == MATH_DELETE)
	{
		SetSelectMath(pos);
	}
}

void EnemyMapEditScene::SelectMathType()
{
	MATHTYPE setType = MATH_DELETE;
	//すでにスタートマスとゴールマスがあったら
	bool isStart = false, isGoal = false;
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.z; y++)
		{
			if (GetTable()->GetMathType(XMFLOAT2(x, y)) == MATH_START)
			{
				isStart = true;
			}
			if (GetTable()->GetMathType(XMFLOAT2(x, y)) == MATH_GOAL)
			{
				isGoal = true;
			}
		}
	}
	//スタートがないなら
	if (!isStart)
	{
		setType = MATH_START;
	}
	//ゴールがないなら
	else if (!isGoal)
	{
		setType = MATH_GOAL;
	}
	//両方あったら
	else
	{
		//スタートとゴール以外のマスランダムで選択
		do {
			setType = (MATHTYPE)(rand() % MATH_MAX);
		} while (setType == MATH_START || setType == MATH_GOAL);
	}
	SetMathType(rand() % MATH_MAX);
}
