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

//�u�}�b�v���쒆�v��\�����鎞��
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
	if (GetMathType() == MATH_CONVEYOR)
	{
		//�R���x�A�ɑ������g�����X�t�H�[��
		Transform tConv = GetTable()->GetMathTransform(XMFLOAT2(GetSelectMath().x, GetSelectMath().y));
		//�R���x�A����������
		const float left = 0;
		const float back = 90;
		const float right = 180;
		const float front = 270;

		//��ԋ߂��́u���v�̕�������������
		const XMFLOAT2 holePos = FindNearMathPos(XMFLOAT2(GetSelectMath().x, GetSelectMath().y), MATH_HOLE);
		//holePos��selectMath_��x��y�ł��ꂼ�������
		//��Βl���傫�������o��
		//���̐��l��+��-���łǂ������������邩���߂�
		float xdis = GetSelectMath().x - holePos.x;
		float ydis = GetSelectMath().y - holePos.y;
		//����������̊m��
		if (abs(xdis) > abs(ydis))
		{
			//+��������
			if (xdis > 0)	tConv.rotate_.z = left;
			//-��������
			else	tConv.rotate_.z = right;
		}
		//�c��������̊m��
		else
		{
			//+��������
			if (ydis > 0)	tConv.rotate_.z = back;
			//-��������
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
	//�X�^�[�g�}�X�g�S�[���}�X�����łɒu����Ă��邩
	if (!StartGoalCheck())
	{
		//�X�^�[�g�}�X�g�S�[���}�X���߂�
		//�X�^�[�g�̈ʒu�͊��S�Ƀ����_��
		startMathPos_ = XMFLOAT2((rand() % mathVolume_.x), (rand() % mathVolume_.y));
		//�X�^�[�g�ƃS�[���̋����̍ŏ��l
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
			//�S�[���̈ʒu�̓X�^�[�g����͂Ȃ�����
			//�X�^�[�g�ƃS�[���̋������X�e�[�W�S�̂�
			// x��y�̑傫���̏��������̔���(disMin)�ȏ�łȂ��ƃ_��
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
		//�X�^�[�g�ƃS�[���̈ʒu��T������
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

	//�����Ƃ��ēn���ꂽ�}�X�̎�ނ����邩�ǂ����`�F�b�N
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

	//�����Ƃ��ēn���ꂽ�}�X�̎�ނ�����ꍇ
	if (types.size() > 0)
	{
		//types�̒�����_pos�Ɉ�ԋ����߂��v�f��Ԃ�
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
	//�Ȃ������ꍇ
	else
	{
		//�����Ƃ��ēn���ꂽ�}�X�̎�ނ��u���v��������
		if (_type == MATH_HOLE)
		{
			//�㉺���E�͈̔͊O�̒��ň�ԋ������߂��v�f��Ԃ�
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
		//����Ȃ�������
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
