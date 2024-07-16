#include "EnemyMapEditScene.h"

#include "../../Engine/Input.h"

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
	//スタートマスとゴールマスがあるかどうか全マス探索
	StartGoalCheck();
	//マス配置の試行回数
	const int num = 100;
	for (int i = 0; i < num; i++)
	{
		rootNode_->Run();
	}
}

void EnemyMapEditScene::Update()
{
	if (Input::IsKeyDown(DIK_SPACE))
	{
		CompButtonPush();
	}
}

void EnemyMapEditScene::ChangeSelectMath()
{
	//選ばれてるマスの種類がスタートマスだったら
	if (GetMathType() == MATH_START)
	{
		isPutStartMath_ = true;
	}
	else if (GetMathType() == MATH_GOAL)
	{
		isPutGoalMath_ = true;
	}
	MapEditScene::ChangeSelectMath();
}

void EnemyMapEditScene::StartGoalCheck()
{
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.z; y++)
		{
			if (GetTable()->GetMathType(XMFLOAT2(x, y)) == MATH_START)
			{
				isPutStartMath_ = true;
			}
			if (GetTable()->GetMathType(XMFLOAT2(x, y)) == MATH_GOAL)
			{
				isPutGoalMath_ = true;
			}

		}
	}
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
	//スタートマスが置かれていなかったら
	if (!isPutStartMath_)
	{
		setType = MATH_START;
	}
	//ゴールマスが置かれていなかったら
	else if (!isPutGoalMath_)
	{
		setType = MATH_GOAL;
	}
	//どっちも置かれていなかったら
	else
	{
		//スタートとゴール以外のマスにする
		do {
			setType = (MATHTYPE)(rand() % MATH_MAX);
		} while (setType == MATH_START || setType == MATH_GOAL);
		
	}
	SetMathType(setType);
}
