#include "EnemyMapEditScene.h"

#include "../../Engine/Input.h"
#include "../../Engine/Image.h"

#include "../../Engine/RootNode.h"

#include "../../Timer.h"

#include "AI/MapEditAIWork.h"
#include "AI/EnemyMapEditAI.h"
#include "AI/CreateCourse/Procedural.h"

#include "EyeCatching.h"

#include "../MapEditScene/Math/MathManager.h"

//「マップ制作中」を表示する時間
const float makingDrawTime = 3.0f;

EnemyMapEditScene::EnemyMapEditScene(GameObject* _parent)
	:MapEditScene(_parent),
	seedNum_(0),
	rootNode_(new RootNode(this, nullptr)),
	eyeCatch_(nullptr)
{
	this->objectName_ = "EnemyMapEditScene";

	AddNode();
}

EnemyMapEditScene::~EnemyMapEditScene()
{
	delete eyeCatch_;
	rootNode_->ChildRelease();
	delete rootNode_;
	delete makingTimer_;
}

void EnemyMapEditScene::Initialize()
{
	MapEditScene::Initialize();

	eyeCatch_ = (EyeCatching*)Instantiate<EyeCatching>(this);

	//スタートマスとゴールマス設定
	StartGoalSet();

	//バラまく種の数設定
	seedNum_ = GetTable()->GetCostLimit() - GetTable()->GetStageCost();

	//Procedural呼んでマスを置く候補に値入れる
	ProcExcute();
	
	//マスを置く試行回数
	const int num = candidatePositions_.size();
	for (int i = 0; i < num; i++)
	{
		rootNode_->Run();
	}

	makingTimer_ = new Timer(makingDrawTime);
}

void EnemyMapEditScene::Update()
{
	makingTimer_->Update();
	if (makingTimer_->isTimeUpped())
	{
		CompButtonPush();
	}
}

void EnemyMapEditScene::Draw()
{
	MapEditScene::Draw();
}

void EnemyMapEditScene::Release()
{
	MapEditScene::Release();
}

void EnemyMapEditScene::MathDraw()
{
	//AIによって変えられる前のマスをDrawする
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.y; y++)
		{
			Image::SetTransform(hPict_[math_[x][y].mathType_], GetTable()->GetMathTransform(XMFLOAT2(x, y)));
			Image::Draw(hPict_[math_[x][y].mathType_]);

		}
	}
}

void EnemyMapEditScene::ChangeSelectMath()
{
	if (GetMathType() == MATH_CONVEYOR)
	{
		//コンベアに代入するトランスフォーム
		Transform tConv = GetTable()->GetMathTransform(XMFLOAT2(GetSelectMath().x, GetSelectMath().y));
		//コンベアが向く方向
		const float left = 0;
		const float back = 90;
		const float right = 180;
		const float front = 270;

		//一番近くの「穴」の方向を向かせる
		const XMFLOAT2 holePos = FindNearMathPos(XMFLOAT2(GetSelectMath().x, GetSelectMath().y), MATH_HOLE);
		//holePosとselectMath_のxとyでそれぞれ引いて
		//絶対値が大きい方を出す
		//その数値が+か-かでどっちを向かせるか決める
		float xdis = GetSelectMath().x - holePos.x;
		float ydis = GetSelectMath().y - holePos.y;
		//横向かせるの確定
		if (abs(xdis) > abs(ydis))
		{
			//+だったら
			if (xdis > 0)	tConv.rotate_.z = left;
			//-だったら
			else	tConv.rotate_.z = right;
		}
		//縦向かせるの確定
		else
		{
			//+だったら
			if (ydis > 0)	tConv.rotate_.z = back;
			//-だったら
			else	tConv.rotate_.z = front;
		}

		GetTable()->SetMathTransform(XMFLOAT2(GetSelectMath().x, GetSelectMath().y), tConv);
	}
	MapEditScene::ChangeSelectMath();
}

bool EnemyMapEditScene::StartGoalCheck()
{
	bool isPutStartMath = false, isPutGoalMath = false;
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.y; y++)
		{
			if (GetTable()->GetMathType(XMFLOAT2(x, y)) == MATH_START)
			{
				isPutStartMath = true;
			}
			if (GetTable()->GetMathType(XMFLOAT2(x, y)) == MATH_GOAL)
			{
				isPutGoalMath = true;
			}
		}
	}
	return isPutStartMath * isPutGoalMath;
}

void EnemyMapEditScene::StartGoalSet()
{
	//スタートマストゴールマスがすでに置かれているか
	if (!StartGoalCheck())
	{
		//スタートマストゴールマス決める
		//スタートの位置は完全にランダム
		startMathPos_ = XMFLOAT2((rand() % mathVolume_.x), (rand() % mathVolume_.y));
		//スタートとゴールの距離の最小値
		int disMin = 0;
		if (mathVolume_.x > mathVolume_.y)
		{
			disMin = mathVolume_.y;
		}
		else
		{
			disMin = mathVolume_.x;
		}
		disMin /= 2;
		do {
			//ゴールの位置はスタートからはなしたい
			//スタートとゴールの距離がステージ全体の
			// xとyの大きさの小さい方の半分(disMin)以上でないとダメ
			goalMathPos_ = XMFLOAT2((rand() % mathVolume_.x), (rand() % mathVolume_.y));
		} while (startMathPos_.x == goalMathPos_.x && startMathPos_.y == goalMathPos_.y ||
			GetDistance(startMathPos_, goalMathPos_) < disMin);
		SetMathType(MATH_START);
		SetSelectMath(XMFLOAT3(startMathPos_.x, startMathPos_.y, 0));
		ChangeSelectMath();
		SetMathType(MATH_GOAL);
		SetSelectMath(XMFLOAT3(goalMathPos_.x, goalMathPos_.y, 0));
		ChangeSelectMath();
	}
	else
	{
		//スタートとゴールの位置を探索する
		for (int x = 0; x < mathVolume_.x; x++)
		{
			for (int y = 0; y < mathVolume_.y; y++)
			{
				if (GetTable()->GetMathType(XMFLOAT2(x, y)) == MATH_START)
				{
					startMathPos_ = XMFLOAT2(x, y);
				}
				if (GetTable()->GetMathType(XMFLOAT2(x, y)) == MATH_GOAL)
				{
					goalMathPos_ = XMFLOAT2(x, y);
				}
			}
		}
	}
}

XMFLOAT2 EnemyMapEditScene::FindNearMathPos(XMFLOAT2 _pos, MATHTYPE _type)
{
	const int besideDir[4] = { 1, 0, 1, 0 };
	const int vertical[4] = { 0, 1, 0, 1 };

	//引数として渡されたマスの種類があるかどうかチェック
	std::vector<XMFLOAT2> types;
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.y; y++)
		{
			if (GetTable()->GetMathType(XMFLOAT2(x, y)) == _type)
			{
				XMFLOAT2 pos = XMFLOAT2(x, y);
				types.push_back(pos);
			}
		}
	}

	//引数として渡されたマスの種類がある場合
	if (types.size() > 0)
	{
		//typesの中から_posに一番距離近い要素を返す
		XMFLOAT2 ret = types[0];
		for (int i = 1; i < types.size(); i++)
		{
			if (GetDistance(_pos, ret) > GetDistance(_pos, types[i]))
			{
				ret = types[i];
			}
		}
		return ret;
	}
	//なかった場合
	else
	{
		//引数として渡されたマスの種類が「穴」だったら
		if (_type == MATH_HOLE)
		{
			//上下左右の範囲外の中で一番距離が近い要素を返す
			XMFLOAT2 outs[4] = { XMFLOAT2(-1, _pos.y), XMFLOAT2(_pos.x, -1),
								XMFLOAT2(mathVolume_.x, _pos.y) ,XMFLOAT2(_pos.x, mathVolume_.y) };
			XMFLOAT2 ret = outs[0];
			for (int i = 1; i < 4; i++)
			{
				if (GetDistance(_pos, ret) > GetDistance(_pos, outs[i]))
				{
					ret = outs[i];
				}
			}
			return ret;
		}
		//じゃなかったら
		else
		{
			return XMFLOAT2(-1, -1);
		}
	}
}

void EnemyMapEditScene::AddNode()
{
	Node* selectType = new EnemyMapEditAI(rootNode_, new MapEdit_MathTypeSelect());
	Node* selectPos = new EnemyMapEditAI(selectType, new MapEdit_MathPosSelect());
	Node* putMath = new EnemyMapEditAI(selectPos, new MapEdit_Action_ChangeMath());
}

void EnemyMapEditScene::SelectMathSet()
{
	//SelectMath_に設定する位置
	//candidatePositions_の先頭の値をselectMathにセットしてセットした先頭をeraseする
	XMFLOAT2 pos = XMFLOAT2(-1, -1);
	SetSelectMath(XMFLOAT3(pos.x, pos.y, 0));
	if (!candidatePositions_.empty())
	{
		pos = *candidatePositions_.begin();
		candidatePositions_.erase(candidatePositions_.begin());
	}

	if (GetTable()->GetMathType(pos) == MATH_DELETE)
	{
		SetSelectMath(XMFLOAT3(pos.x, pos.y, 0));
	}
}

void EnemyMapEditScene::SelectMathType()
{
	MATHTYPE setType = MATH_DELETE;

	//スタートとゴール以外のマスにする
	do {
		setType = (MATHTYPE)(rand() % MATH_MAX);
	} while (setType == MATH_START || setType == MATH_GOAL);
	SetMathType(setType);
}

void EnemyMapEditScene::ProcExcute()
{
	//Proceduralに渡すそれぞれのマスの濃淡
	std::vector<std::vector<int>> shades_;
	shades_.resize(mathVolume_.x);
	for (int x = 0; x < shades_.size(); x++)
	{
		shades_[x].resize(mathVolume_.y);
		for (int y = 0; y < shades_[x].size(); y++)
		{
			//スタートマスからの距離とゴールマスからの距離を合わせた値を濃淡の基準にする
			//濃淡はマスの左上から右下までの最大の距離からスタートマスからの距離を引いた値、
			// ゴールマスからの距離を引いた値のふたつを足した値
			float maxDir = GetDistance(XMFLOAT2(0, 0), XMFLOAT2(mathVolume_.x - 1, mathVolume_.y - 1));
			float startToShade = maxDir - GetDistance(startMathPos_, XMFLOAT2(x, y));
			float goalToShade = maxDir - GetDistance(goalMathPos_, XMFLOAT2(x, y));

			shades_[x][y] = startToShade + goalToShade;
		}
	}

	
	//Procedural呼ぶ
	Procedural* proc = new Procedural(shades_, mathVolume_.x, mathVolume_.z);
	proc->Excute(seedNum_);
	//Proceduralから受け取るSeed
	std::vector<Seed*> seeds(proc->GetSeedListSize());
	seeds = proc->GetSeedList();
	candidatePositions_.resize(seeds.size());
	for (int i = 0; i < seeds.size(); i++)
	{
		candidatePositions_[i] = seeds[i]->GetPos();
	}

}

float EnemyMapEditScene::GetDistance(XMFLOAT2 _pos1, XMFLOAT2 _pos2)
{
	float ret = 0;
	ret = abs(sqrt((_pos1.x - _pos2.x) * (_pos1.x - _pos2.x) + (_pos1.y - _pos2.y) * (_pos1.y - _pos2.y)));
	return ret;
}
