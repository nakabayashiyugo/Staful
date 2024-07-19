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

EnemyMapEditScene::EnemyMapEditScene(GameObject* _parent)
	:MapEditScene(_parent),
	seedNum_(0),
	rootNode_(new RootNode(this, nullptr))
{
	this->objectName_ = "EnemyMapEditScene";

	AddNode();

	eyeCatch_ = (EyeCatching*)Instantiate<EyeCatching>(this);
}

void EnemyMapEditScene::Initialize()
{
	MapEditScene::Initialize();

	//�X�^�[�g�}�X�ƃS�[���}�X�ݒ�
	StartGoalSet();

	//�o���܂���̐��ݒ�
	seedNum_ = GetTable()->GetCostLimit() - GetTable()->GetStageCost();

	//Procedural�Ă�Ń}�X��u�����ɒl�����
	ProcExcute();
	
	//�}�X��u�����s��
	const int num = candidatePositions_.size();
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
	//AI�ɂ���ĕς�����O�̃}�X��Draw����
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
	if (!StartGoalCheck())
	{
		//�X�^�[�g�}�X�g�S�[���}�X���߂�
		startMathPos_ = XMFLOAT2((rand() % mathVolume_.x), (rand() % mathVolume_.y));
		do {
			goalMathPos_ = XMFLOAT2((rand() % mathVolume_.x), (rand() % mathVolume_.y));
		} while (startMathPos_.x == goalMathPos_.x && startMathPos_.y == goalMathPos_.y);
		SetMathType(MATH_START);
		SetSelectMath(XMFLOAT3(startMathPos_.x, startMathPos_.y, 0));
		ChangeSelectMath();
		SetMathType(MATH_GOAL);
		SetSelectMath(XMFLOAT3(goalMathPos_.x, goalMathPos_.y, 0));
		ChangeSelectMath();
	}
	else
	{
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

void EnemyMapEditScene::AddNode()
{
	Node* selectType = new EnemyMapEditAI(rootNode_, new MapEdit_MathTypeSelect());
	Node* costManage = new EnemyMapEditAI(selectType, new MapEdit_IsPutMath());
	Node* selectPos = new EnemyMapEditAI(costManage, new MapEdit_MathPosSelect());
	Node* putMath = new EnemyMapEditAI(selectPos, new MapEdit_Action_ChangeMath());
}

void EnemyMapEditScene::SelectMathSet()
{
	//SelectMath_�ɐݒ肷��ʒu
	//candidatePositions_�̐擪�̒l��selectMath�ɃZ�b�g���ăZ�b�g�����擪��erase����
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

	//�X�^�[�g�ƃS�[���ȊO�̃}�X�ɂ���
	do {
		setType = (MATHTYPE)(rand() % MATH_MAX);
	} while (setType == MATH_START || setType == MATH_GOAL);
	SetMathType(setType);
}

void EnemyMapEditScene::ProcExcute()
{
	//Procedural�ɓn�����ꂼ��̃}�X�̔Z�W
	std::vector<std::vector<int>> shades_;
	shades_.resize(mathVolume_.x);
	for (int x = 0; x < shades_.size(); x++)
	{
		shades_[x].resize(mathVolume_.y);
		for (int y = 0; y < shades_[x].size(); y++)
		{
			//�X�^�[�g�}�X����̋����ƃS�[���}�X����̋��������킹���l��Z�W�̊�ɂ���
			//�Z�W�̓}�X�̍��ォ��E���܂ł̍ő�̋�������X�^�[�g�}�X����̋������������l�A
			// �S�[���}�X����̋������������l�̂ӂ��𑫂����l
			float maxDir = GetDistance(XMFLOAT2(0, 0), XMFLOAT2(mathVolume_.x - 1, mathVolume_.y - 1));
			float startToShade = maxDir - GetDistance(startMathPos_, XMFLOAT2(x, y));
			float goalToShade = maxDir - GetDistance(goalMathPos_, XMFLOAT2(x, y));

			shades_[x][y] = startToShade + goalToShade;
		}
	}

	
	//Procedural�Ă�
	Procedural* proc = new Procedural(shades_, mathVolume_.x, mathVolume_.z);
	proc->Excute(seedNum_);
	//Procedural����󂯎��Seed
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
