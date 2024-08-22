#include "MapEditScene.h"

#include "../../Engine/SceneManager.h"
#include "../../Engine/Image.h"
#include "../../Engine/Input.h"
#include "../../Engine/Text.h"
#include "../../Engine/Audio.h"

#include "../../resource.h"
#include "../../ButtonManager.h"
#include "../../Button.h"
#include "../../GamePlayer.h"

#include "../SceneTransition.h"
#include "../PlayScene.h"

#include "Math/MathManager.h"

//�}�X�̈ʒu�̏����l
const XMFLOAT3 mathInitPos = XMFLOAT3(-1, -1, 0);
//���y�̑傫��
const float soundVolume = 0.2f;
//���ꂼ��̃R�X�g
const int floorCost = 0;
const int wallCost = 1;
const int holeCost = 1;
const int converyerCost = 1;
const int togetogeCost = 5;
const int pitfallCost = 3;
const int timeLimitDecCost = 10;
const int confusionCost = 8;
const int startCost = 0;
const int goalCost = 0;

MapEditScene::MapEditScene(GameObject* parent)
	: GameObject(parent, "MapEditScene"),
	//�}�X�̑I���ɂ���
	mathtype_(0),
	//�e�X�g�v���C�ɂ���
	isClear_(false),
	canTest_(false),
	isDisp_(true),
	testPS_(nullptr),
	//�Ƃ��Ƃ��ɂ���
	tgtgRouteMathDown_(mathInitPos),
	tgtgRouteMathUp_(mathInitPos),
	hTgtgRoute_(-1),
	//���y�ɂ���
	hAudio_Music_(-1)
{
	for (int i = 0; i < MATHTYPE::MATH_MAX; i++)
	{
		hPict_[i] = -1;	
		hExpantion_[i] = -1;
	}
	MathVolumeRead();

	Math_Resize(mathVolume_.x, mathVolume_.y, &math_);
	Math_Resize(mathVolume_.x, mathVolume_.y, &math_origin_);
	Math_Resize(mathVolume_.x, mathVolume_.y, &isConvRot_);

	pGP_ = (GamePlayer*)this->pParent_;
	saveNum_ = pGP_->GetSaveNum();

	//���ꂼ��̃}�X�̃R�X�g�ݒ�
	costs_.resize(MATH_MAX);

	costs_ = { 
		wallCost, 
		holeCost, 
		converyerCost, 
		togetogeCost, 
		pitfallCost, 
		timeLimitDecCost,
		confusionCost,
		startCost, 
		goalCost,
		floorCost};

	//��Q���̂����鐧������
	int costLimitFirst;
	int costLimitPlus;

	costLimitFirst = mathVolume_.x + mathVolume_.y;
	costLimitPlus = costLimitFirst / 2;
	turnNum_ = pGP_->GetTurnNum();
	int costLimit = costLimitFirst + (turnNum_ - 1) * costLimitPlus;

	table_ = new MathManager(mathVolume_.x, mathVolume_.y, MATH_DELETE, costs_, costs_.size(), costLimit);

	Read();
}

void MapEditScene::Initialize()
{
	//�}�X������
	MathInit();
	//�{�^���̏�����
	ButtonInit();
	//�}�X�̐����̏�����
	ExpantionInit();
	//���y�̏�����
	AudioInit();

	hTgtgRoute_ = Image::Load("Assets\\Togetoge_Route.png");
	assert(hTgtgRoute_ >= 0);

	pText_ = new Text();
	pText_->Initialize();
	Audio::Play(hAudio_Music_, soundVolume);
}

void MapEditScene::Update()
{
	

	//SelectMathSet();
	ButtonUpdate();

	//���N���b�N�N���b�N
	if (Input::IsMouseButton(0))
	{
		LeftClicked();
	}
	//�E�N���b�N
	if (Input::IsMouseButtonDown(1))
	{
		RightClicked();
	}
	//�E�N���b�N��������
	if (Input::IsMuoseButtonUp(1))
	{
		RightReleased();
	}
}

void MapEditScene::Draw()
{
	IsDisplay(isDisp_);
	if(isDisp_)
	{
		//�}�X�\��
		MathDraw();
		//�}�X�̐����\��
		ExpantionDraw();
		//�R�X�g�\��
		CostDraw();
		//�Ƃ��Ƃ����[�g�\��
		auto itr = tTgtgRoute_.begin();
		while (itr != tTgtgRoute_.end())
		{
			if (itr->route_.scale_.x != 1)
			{
				Image::SetTransform(hTgtgRoute_, itr->route_);
				Image::Draw(hTgtgRoute_);

			}
			itr++;
		}
		//�v���C���[�ԍ��\��
		pGP_->PlayerNumDraw();
	}
}

void MapEditScene::Release()
{
}

void MapEditScene::CostDraw()
{
	const std::string strCost = "cost";
	const std::string strCostLimit = "costLimit";
	//�����ucost�v�̕\���ʒu
	XMFLOAT3 costPos;
	//�����ucostLimit�v�̕\���ʒu
	XMFLOAT3 costLimitPos;
	//�R�X�g�̕\���ʒu
	XMFLOAT3 curCostPos;
	//�R�X�g�\���̊�̈ʒu
	const XMFLOAT3 costPosBase(300, 650, 0);
	//���3�̈ʒu�̂��ꂼ��̍�
	const XMFLOAT3 posDiff = XMFLOAT3(50, 30, 0);

	costPos = costPosBase;
	curCostPos = XMFLOAT3(costPos.x + posDiff.x, costPos.y + posDiff.y, costPosBase.z);
	costLimitPos = XMFLOAT3(curCostPos.x + posDiff.x, curCostPos.y + posDiff.y, costPosBase.z);
	//�����ucost�v�\��
	pText_->Draw(costPos.x, costPos.y, strCost.c_str());
	//�R�X�g�\��
	std::string str = std::to_string(table_->GetStageCost()) + " / " + std::to_string(table_->GetCostLimit());
	pText_->Draw(curCostPos.x, curCostPos.y, str.c_str());
	//�����ucostLimit�v�\��
	pText_->Draw(costLimitPos.x, costLimitPos.y, strCostLimit.c_str());
}

void MapEditScene::ChangeSelectMath()
{
	XMFLOAT2 set = XMFLOAT2(selectMath_.x, selectMath_.y);
	table_->SetMathType(set, mathtype_);

	//�N���b�N�����}�X��I��ł�}�X�ɕς���
	if (table_->GetMathType(set) != MATH_CONVEYOR)
	{
		Transform tMath = table_->GetMathTransform(set);
		tMath.rotate_ = XMFLOAT3(0, 0, 0);
		table_->SetMathTransform(set, tMath);
	}


	isClear_ = false;

	//�e�X�g�v���C�ł��邩�ǂ����`�F�b�N
	CheckCanTest();

	//�}�X��u��������SE
	//���O�܂ŃJ�[�\���̒u����Ă����}�X
	static XMFLOAT3 prevMath = mathInitPos;
	if (prevMath.x != selectMath_.x || prevMath.y != selectMath_.y)
	{
		Audio::Stop(hSE_PutMath_);
	}
	Audio::Play(hSE_PutMath_, 1);
	prevMath = selectMath_;
}

void MapEditScene::ButtonInit()
{
	//�}�X�I���{�^���̃t�@�C���l�[���w��
	std::string buttonName[MATH_MAX];
	for (int i = 0; i < MATH_MAX; i++)
	{
		buttonName[i] = "Button_" + fileNameInit_[i] + ".png";
	}
	//���̑��̃{�^��
	//�����{�^��
	std::string buttonComplete = "Button_Complete.png";
	//�e�X�g�v���C�{�^��
	std::string buttonTestplay = "Button_TestPlay.png";
	
	//��̃t�@�C���������Ă�t�H���_�[
	std::string folderName = "Assets\\Button\\MathSelect\\";

	//�}�X�I���{�^���̉摜�ԍ�
	int mathButtonNum[MATH_MAX];
	//���̑��̃{�^���̉摜�ԍ�
	int completeNum = -1;
	int testplayNum = -1;
	int cancelNum = -1;

	//�}�X�I���{�^���̃��[�h
	for (int i = 0; i < MATH_MAX; i++)
	{
		buttonName[i] = folderName + buttonName[i];

		mathButtonNum[i] = Image::Load(buttonName[i]);
		assert(mathButtonNum[i] >= 0);
	}

	//�{�^���̃e�N�X�`���T�C�Y
	XMFLOAT3 imageSize = Image::GetTextureSize(mathButtonNum[0]);

	//���̑��̃{�^���������Ă�t�H���_��
	folderName = "Assets\\Button\\SceneTrans\\";

	//�����{�^���̃��[�h
	buttonComplete = folderName + buttonComplete;
	completeNum = Image::Load(buttonComplete);
	assert(completeNum >= 0);

	//�e�X�g�v���C�{�^���̃��[�h
	buttonTestplay = folderName + buttonTestplay;
	testplayNum = Image::Load(buttonTestplay);
	assert(testplayNum >= 0);

	//�{�^���̕��ʂ̃}�X�Ƃ̔{��
	const float normalMathtoMult = 2.0f;
	//�{�^���̃T�C�Y
	const float mathButtonSize = EDITERMATHSIZE * normalMathtoMult;
	//�{�^���̑傫���ݒ�
	const XMFLOAT3 mbScale = XMFLOAT3(1.0f / imageSize.x * mathButtonSize, 1.0f / imageSize.y * mathButtonSize, 1);
	const int buttonInitNum = 0;
	//�{�^���̃I�u�W�F�N�g�l�[��
	std::string buttonStr;

	for (int i = buttonInitNum; i < MATH_MAX; i++)
	{
		pMathButton_[i] = ButtonManager::GetButton(ButtonManager::AddButton(("MathButton" + std::to_string(i)), this));
		pMathButton_[i]->SetPictNum(mathButtonNum[i]);

		
		//�}�X�I���{�^���̊�̈ʒu
		const XMFLOAT3 mbInitPos = XMFLOAT3(-0.9f, 0.9f, 0);
		//�}�X�I���{�^���������Ƃɉ��s����邩
		const int mbNewLineNum = 5;
		//�}�X�I���{�^���̈ʒu
		XMFLOAT3 mbPos;
		//�{�^���̑傫��
		const XMFLOAT3 mbSize = XMFLOAT3((1.0f / Direct3D::bfr_scrWidth) * mathButtonSize * 2,
			(1.0f / Direct3D::bfr_scrHeight) * mathButtonSize * 2, 0);
		//�}�X�I���{�^���̊Ԋu
		const XMFLOAT3 mbIntervalBase = XMFLOAT3(0, 0, 0);
		const XMFLOAT3 mbInterval = XMFLOAT3(((i - buttonInitNum) % mbNewLineNum) * mbIntervalBase.x,
			-((i - buttonInitNum) / mbNewLineNum) * mbIntervalBase.y, 0);
		mbPos.x =  (float)((i - buttonInitNum) % mbNewLineNum) * mbSize.x + mbInitPos.x + mbInterval.x;
		mbPos.y = -(float)((i - buttonInitNum) / mbNewLineNum) * mbSize.y + mbInitPos.y + mbInterval.y;
		mbPos.z = mbInitPos.z;

		Transform mbTransform;
		mbTransform.position_ = mbPos;
		mbTransform.scale_ = mbScale;
		pMathButton_[i]->SetTransform(mbTransform);
	}

	//���̑��̃{�^���̑傫��
	const XMFLOAT3 obScale = XMFLOAT3(0.3f, 0.3f, 1);
	//�����{�^��
	pCompleteButton_ = ButtonManager::GetButton(ButtonManager::AddButton("compButton", (GameObject*)this));

	const XMFLOAT3 cbPos = XMFLOAT3(-0.8f, -0.7f, 0);
	Transform cbTransform;
	cbTransform.position_ = cbPos;
	cbTransform.scale_ = obScale;
	pCompleteButton_->SetTransform(cbTransform);
	pCompleteButton_->SetPictNum(completeNum);

	//�e�X�g�v���C�{�^��
	//�e�X�g�v���C�{�^���̔ԍ�
	pTestplayButton_ = ButtonManager::GetButton(ButtonManager::AddButton("testButton", (GameObject*)this));

	const XMFLOAT3 tbPos = XMFLOAT3(-0.8f, -0.6f, 0);
	Transform tbTransform;
	tbTransform.position_ = tbPos;
	tbTransform.scale_ = obScale;
	pTestplayButton_->SetTransform(tbTransform);
	pTestplayButton_->SetPictNum(testplayNum);
}

void MapEditScene::ButtonUpdate()
{
	SelectMathType();
	OtherButtonPush();
}

void MapEditScene::OtherButtonPush()
{
	pTestplayButton_->SetIsCanPush(canTest_ * isDisp_);
	//�e�X�g�v���C�N���A�ł��Ȃ��Ɖ����Ȃ��悤�ɂ���
	pCompleteButton_->SetIsCanPush(isClear_ * isDisp_);

	//�e�X�g�v���C�{�^���������ꂽ��
	if (pTestplayButton_->OnClick())
	{
		TestButtonPush();
	}
	//�����{�^���������ꂽ��
	else if (pCompleteButton_->OnClick())
	{
		CompButtonPush();
	}
}

void MapEditScene::CompButtonPush()
{
	//���y��~
	Audio::Stop(hAudio_Music_);
	//�t�@�C���ɏ�������
	Write();
	pGP_->MapEditFinished();
}

void MapEditScene::TestButtonPush()
{
	//�}�b�v�G�f�B�^�[��\��
	isDisp_ = false;
	Write();
	testPS_->Instantiate<PlayScene>(this);
	testPS_ = (PlayScene*)FindObject("PlayScene");
}

void MapEditScene::CancelButtonPush()
{
	testPS_->KillMe();
	testPS_ = nullptr;
	//�}�b�v�G�f�B�^�[�\��
	isDisp_ = true;
}

void MapEditScene::ExpantionInit()
{
	//�}�X�̐����̃t�@�C���l�[��
	std::string expantionName[MATH_MAX];
	for(int i = 0; i < MATH_MAX; i++)
	{
		expantionName[i] = "Expantion_" + fileNameInit_[i] + ".png";
	};
	//�}�X�̐����̉摜�������Ă�t�H���_��
	std::string folderName = "Assets\\MathExpantion\\";
	//�}�X�̐����摜���[�h
	for (int i = 0; i < MATH_MAX; i++)
	{
		if (i != (int)MATH_DELETE)
		{
			expantionName[i] = folderName + expantionName[i];

			hExpantion_[i] = Image::Load(expantionName[i]);
			assert(hExpantion_[i] >= 0);
		}
	}
}

void MapEditScene::ExpantionDraw()
{
	//�摜�̑傫��
	XMFLOAT3 exTexSize = Image::GetTextureSize(hExpantion_[mathtype_]);
	//�摜�̏�̈ʒu
	const XMFLOAT3 exPos = XMFLOAT3(-0.7f, 0.5f, 0);
	//�摜�̑傫��
	const XMFLOAT3 exScale = XMFLOAT3(0.7f, 0.7f, 1);
	tExpantion_.position_ = XMFLOAT3(exPos.x, 
		exPos.y - ((exTexSize.y) / Direct3D::bfr_scrHeight * exScale.y), 
		exPos.z);
	tExpantion_.scale_ = exScale;
	//���̃}�X�̎�ނ̃R�X�g�\��
	// ��ʂ̒������獶��܂ł̋���
	const XMFLOAT2 upperLeftDir = XMFLOAT2(1.0f, 1.0f);
	//��ʑS�̂̑傫��
	const XMFLOAT2 scrSize = XMFLOAT2(2.0f, 2.0f);
	//�ʒu
	XMFLOAT2 mathCostPos = XMFLOAT2((exPos.x + upperLeftDir.x) * Direct3D::bfr_scrWidth / scrSize.x,
		abs(exPos.y - upperLeftDir.y) * Direct3D::bfr_scrHeight / scrSize.y + (exTexSize.y * exScale.y) + pText_->GetHeight());
	//�\�����镶��
	std::string str = "cost : ";
	str += std::to_string(costs_[mathtype_]);

	//�R�X�g�\��
	pText_->Draw(mathCostPos.x, mathCostPos.y, str.c_str());

	Image::SetTransform(hExpantion_[mathtype_], tExpantion_);
	Image::Draw(hExpantion_[mathtype_]);
}

void MapEditScene::MathInit()
{
	//�}�X�̉摜���[�h
	std::string filename[MATH_MAX];
	for(int i = 0; i < MATH_MAX; i++)
	{
		filename[i] = "Math_" + fileNameInit_[i] + ".png";
	};
	for (int i = 0; i < MATH_MAX; i++)
	{
		std::string folderName = "Assets\\MathType\\";
		filename[i] = folderName + filename[i];
		hPict_[i] = Image::Load(filename[i]);
		assert(hPict_[i] >= 0);
	}
	XMFLOAT3 imageSize = Image::GetTextureSize(hPict_[0]);
	//�}�X��u����̈ʒu�i�����̃}�X�̈ʒu)
	const XMFLOAT3 mathInitPos = XMFLOAT3(0.25f, 0, 0);

	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.y; y++)
		{
			math_origin_[x][y] = math_[x][y];
			XMFLOAT2 pos = XMFLOAT2(x, y);

			Transform tMath = math_[x][y].mathPos_;

			tMath.scale_ = XMFLOAT3(1.0f / imageSize.x * EDITERMATHSIZE, 1.0f / imageSize.y * EDITERMATHSIZE, 1);
			//1�}�X�̑傫��
			const XMFLOAT2 mathSize = XMFLOAT2(((float)x / Direct3D::bfr_scrWidth) * EDITERMATHSIZE * 2,
				((float)y / Direct3D::bfr_scrHeight) * EDITERMATHSIZE * 2);
			//�}�X��u���ʒu
			const XMFLOAT2 mathPos = XMFLOAT2(mathInitPos.x - ((float)mathVolume_.x / Direct3D::bfr_scrWidth) * EDITERMATHSIZE,
				mathInitPos.y - ((float)mathVolume_.y / Direct3D::bfr_scrHeight) * EDITERMATHSIZE);

			tMath.position_.x = mathSize.x + mathPos.x;
			tMath.position_.y = mathSize.y + mathPos.y;

			table_->SetMathTransform(pos, tMath);
		}
	}
}

void MapEditScene::MathDraw()
{
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.y; y++)
		{
			XMFLOAT2 drawPos = XMFLOAT2(x, mathVolume_.y - 1 - y);
			//�R���x�A�̉�]
			if (isConvRot_[drawPos.x][drawPos.y])
			{
				//�R���x�A����鉹
				Audio::Play(hSE_ConvRot_, 1);

				//1�t���[���ɉ�]�����
				const float rotValUpdate = 5;
				Transform tConv = table_->GetMathTransform(drawPos);
				tConv.rotate_.z += rotValUpdate;
				
				//1��]�̗�
				const float rotVal = 90;

				if ((int)tConv.rotate_.z % (int)rotVal == 0)
				{
					tConv.rotate_.z = (tConv.rotate_.z / rotVal) * rotVal;
					isConvRot_[x][mathVolume_.y - 1 - y] = false;
					Audio::Stop(hSE_ConvRot_);
				}
				table_->SetMathTransform(drawPos, tConv);
			}
			Image::SetTransform(hPict_[table_->GetMathType(XMFLOAT2(x, y))], table_->GetMathTransform(XMFLOAT2(x, y)));
			Image::Draw(hPict_[table_->GetMathType(XMFLOAT2(x, y))]);

		}
	}
}

void MapEditScene::LeftClicked()
{
	leftClickPos_ = table_->GetCursorCellIndex();
	selectMath_ = XMFLOAT3(leftClickPos_.x, leftClickPos_.y, 0);

	//�������Ƃ��낪�}�X�͈̔͊O��������
	if (leftClickPos_.x != -1)
	{
		if (math_origin_[(int)leftClickPos_.x][(int)leftClickPos_.y].mathType_ == MATH_DELETE)
		{
			switch ((MATHTYPE)mathtype_)
			{
			case MATH_START:
				if (turnNum_ == 1)
				{
					//�X�^�[�g�}�X�����łɂ��邩�ǂ����T��
					for (int x = 0; x < mathVolume_.x; x++)
					{
						for (int y = 0; y < mathVolume_.y; y++)
						{
							if (table_->GetMathType(XMFLOAT2(x, y)) == MATH_START)
							{
								table_->SetMathType(XMFLOAT2(x, y), MATH_DELETE);
							}
						}
					}
					ChangeSelectMath();
				}
				break;
			case MATH_GOAL:
				if (turnNum_ == 1)
				{
					//�S�[���}�X�����łɂ��邩�ǂ����T��
					for (int x = 0; x < mathVolume_.x; x++)
					{
						for (int y = 0; y < mathVolume_.y; y++)
						{
							if (table_->GetMathType(XMFLOAT2(x, y)) == MATH_GOAL)
							{
								table_->SetMathType(XMFLOAT2(x, y), MATH_DELETE);
							}
						}
					}
					ChangeSelectMath();
				}
				break;
			case MATH_TOGETOGE:
				XMFLOAT3 mousePos = XMFLOAT3((int)leftClickPos_.x, (int)leftClickPos_.y, 0);
				ChangeSelectMath();

				TgtgPushBack(mousePos);
				break;
			default:
				ChangeSelectMath();
				break;
			}
		}
	}

	//�Ƃ��Ƃ��}�X���ق��̃}�X�ɕς�������̗v�f�̍폜
	TogetogeElemDelete();
}

void MapEditScene::RightClicked()
{
	rightClickPos_ = table_->GetCursorCellIndex();
	

	//�R���x�A�}�X��]
	//�������Ƃ��낪�}�X�͈̔͊O��������
	if (rightClickPos_.x != -1)
	{
		if ((MATHTYPE)mathtype_ == MATH_CONVEYOR)
		{
			if (table_->GetMathType(rightClickPos_) == MATHTYPE::MATH_CONVEYOR)
			{
				isConvRot_[(int)rightClickPos_.x][(int)rightClickPos_.y] = true;
				isClear_ = false;
			}
		}
		if ((MATHTYPE)mathtype_ == MATH_TOGETOGE)
		{
			if (table_->GetMathType(rightClickPos_) == MATHTYPE::MATH_TOGETOGE)
			{
				tgtgRouteMathDown_ = XMFLOAT3(rightClickPos_.x, rightClickPos_.y, 0);
			}
		}
	}
}

void MapEditScene::RightReleased()
{
	rightReleasePos_ = table_->GetCursorCellIndex();
	tgtgRouteMathUp_ = XMFLOAT3(rightReleasePos_.x, rightReleasePos_.y, 0);
	
	//�������Ƃ��낪�}�X�͈̔͊O����Ȃ�������
	if (rightReleasePos_.x != -1)
	{
		if ((MATHTYPE)mathtype_ == MATH_TOGETOGE)
		{
			//�Ƃ��Ƃ����[�g�ݒ�
			TogetogeRouteSet();
		}
	}

}

void MapEditScene::TgtgPushBack(XMFLOAT3 _mousePos)
{
	//tTgtgRoute�̒��Ɉʒu�������ȂƂ��Ƃ������݂��Ă邩
	auto itr = tTgtgRoute_.begin();
	while (itr != tTgtgRoute_.end())
	{
		if (itr->initPos_.x == _mousePos.x &&
			itr->initPos_.y == _mousePos.y)
		{
			break;
		}
		itr++;
	}
	//���݂��Ȃ�������
	if (itr == tTgtgRoute_.end())
	{
		//tTgtgRoute_�ɒǉ�
		TOGETOGEROUTE* ptg = new TOGETOGEROUTE();
		ptg->initPos_ = ptg->destPos_ = _mousePos;
		ptg->route_.scale_ = XMFLOAT3(0, 0, 0);
		tTgtgRoute_.push_back(*ptg);
		delete ptg;
	}
}

void MapEditScene::SelectMathSet()
{
	selectMath_ = XMFLOAT3(table_->GetCursorCellIndex().x,
							table_->GetCursorCellIndex().y, 0);
}

void MapEditScene::SelectMathType()
{
	for (int i = 0; i < MATH_MAX; i++)
	{
		if (pMathButton_[i]->OnClick())
		{
			mathtype_ = i;
		}
	}
	//�I�΂�Ă�{�^��
	for (int i = 0; i < MATH_MAX; i++)
	{
		pMathButton_[i]->SetIsSelect(false);
		if (mathtype_ == i)
		{
			pMathButton_[i]->SetIsSelect(true);
		}
	}

}

void MapEditScene::IsDisplay(bool _isDisp)
{
	//�{�^���\����\���؂�ւ�
	for (int i = 0; i < MATH_MAX; i++)
	{
		pMathButton_[i]->SetIsDisplay(_isDisp);
	}
	//�����{�^���\����\���؂�ւ�
	pCompleteButton_->SetIsDisplay(_isDisp);
	//�e�X�g�v���C�{�^���\����\���؂�ւ�
	pTestplayButton_->SetIsDisplay(_isDisp);
}

void MapEditScene::CheckCanTest()
{
	//�X�^�[�g�}�X�ƃS�[���}�X�������ĂȂ��ƃe�X�g�v���C���ł��Ȃ��悤�ɂ���
	bool isStart = false, isGoal = false;
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.y; y++)
		{
			if (table_->GetMathType(XMFLOAT2(x, y)) == MATH_START)
			{
				isStart = true;
			}
			if (table_->GetMathType(XMFLOAT2(x, y)) == MATH_GOAL)
			{
				isGoal = true;
			}
		}
	}
	if (isStart && isGoal)
	{
		canTest_ = true;
	}
	else
	{
		canTest_ = false;
	}
}

void MapEditScene::TogetogeRouteSet()
{
	if (tgtgRouteMathDown_.x != -1)
	{
		//tgtgRouteMathUp���}�X�͈̔͊O��������
		if (tgtgRouteMathUp_.x < 0)
		{
			tgtgRouteMathUp_.x = 0;
		}
		else if (tgtgRouteMathUp_.x >= mathVolume_.x)
		{
			tgtgRouteMathUp_.x = mathVolume_.x - 1;
		}
		if (tgtgRouteMathUp_.y < 0)
		{
			tgtgRouteMathUp_.y = 0;
		}
		else if (tgtgRouteMathUp_.y >= mathVolume_.y)
		{
			tgtgRouteMathUp_.y = mathVolume_.y - 1;
		}

		auto itr = tTgtgRoute_.begin();
		while (itr != tTgtgRoute_.end())
		{
			if (itr->initPos_.x == tgtgRouteMathDown_.x &&
				itr->initPos_.y == tgtgRouteMathDown_.y)
			{
				break;
			}
			itr++;
		}
		//�Ƃ��Ƃ����[�g�̑���
		const int tgtgRouteThick = 5;
		//1�}�X�̑傫��
		XMFLOAT3 mathSize = table_->GetMathTransform(XMFLOAT2(0, 0)).scale_;
		//�c�ړ�
		if (abs(tgtgRouteMathUp_.x - tgtgRouteMathDown_.x) < abs(tgtgRouteMathUp_.y - tgtgRouteMathDown_.y))
		{
			//y�����̑傫��
			float ysize = abs(tgtgRouteMathUp_.y - tgtgRouteMathDown_.y);

			itr->route_.scale_.x = mathSize.x / tgtgRouteThick;
			itr->route_.scale_.y = mathSize.y * ysize;

			//���[�g��\������ʒu
			//y�����̕\������ʒu(�}�E�X���������ʒu�Ɨ������ʒu�̒��Ԃ̈ʒu)
			float ypos = ((int)tgtgRouteMathUp_.y + tgtgRouteMathDown_.y) / 2;
			XMFLOAT3 routePos = table_->GetMathTransform(
				XMFLOAT2((int)tgtgRouteMathDown_.x, ypos)).position_;
			itr->route_.position_ = routePos;

			if (((int)tgtgRouteMathUp_.y + (int)tgtgRouteMathDown_.y) % 2 != 0)
			{
				itr->route_.position_.y += (mathSize.y) / 2;
			}
			itr->destPos_.y = tgtgRouteMathUp_.y;
			itr->destPos_.x = tgtgRouteMathDown_.x;
		}
		//���ړ�
		else
		{
			//x�����̑傫��
			float xsize = abs(tgtgRouteMathUp_.x - tgtgRouteMathDown_.x);

			itr->route_.scale_.x = mathSize.x * xsize;
			itr->route_.scale_.y = mathSize.y / tgtgRouteThick;

			//���[�g��\������ʒu
			//x�����̕\������ʒu(�}�E�X���������ʒu�Ɨ������ʒu�̒��Ԃ̈ʒu)
			float xpos = ((int)tgtgRouteMathUp_.x + tgtgRouteMathDown_.x) / 2;
			XMFLOAT3 routePos = table_->GetMathTransform(
				XMFLOAT2(xpos, (int)tgtgRouteMathDown_.y)).position_;
			itr->route_.position_ = routePos;

			if (((int)tgtgRouteMathUp_.x + (int)tgtgRouteMathDown_.x) % 2 != 0)
			{
				itr->route_.position_.x += (mathSize.x) / 2;
			}
			itr->destPos_.x = tgtgRouteMathUp_.x;
			itr->destPos_.y = tgtgRouteMathDown_.y;
		}

		if (itr->route_.scale_.x <= 0 && itr->route_.scale_.y <= 0)
		{
			tTgtgRoute_.erase(itr);
		}
	}
	tgtgRouteMathDown_ = mathInitPos;
	tgtgRouteMathUp_ = mathInitPos;
}

void MapEditScene::TogetogeElemDelete()
{
	//���[�g������Ƃ��Ƃ��̃}�X�����̃}�X�ɕύX�ɂȂ����Ƃ�
	auto itr = tTgtgRoute_.begin();
	while (itr != tTgtgRoute_.end())
	{
		if (table_->GetMathType(XMFLOAT2(itr->initPos_.x, itr->initPos_.y)) != (int)MATH_TOGETOGE)
		{
			tTgtgRoute_.erase(itr);
			break;
		}
		itr++;
	}
}

void MapEditScene::AudioInit()
{
	//���y�������Ă�t�H���_��
	const std::string folderName = "Assets\\Audio\\";
	//���y
	const std::string musicFolder = "Music\\";
	//���y���[�h
	std::string music = folderName + musicFolder + "Audio_MapEdit.wav";
	hAudio_Music_ = Audio::Load(music, true);

	//SE
	const std::string SEFolder = "SE\\";
	//SE���[�h
	std::string se = folderName + SEFolder + "SE_PutMath.wav";
	hSE_PutMath_ = Audio::Load(se, false);
	//�R���x�A����]�������Ƃ���SE
	std::string convSe = folderName + SEFolder + "SE_Rotate.wav";
	hSE_ConvRot_ = Audio::Load(convSe, false);
}

void MapEditScene::Write()
{
	//math_�ɒl�����
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int z = 0; z < mathVolume_.y; z++)
		{
			math_[x][z].mathType_ = (MATHTYPE)table_->GetMathType(XMFLOAT2(x, z));
			math_[x][z].mathPos_ = table_->GetMathTransform(XMFLOAT2(x, z));
		}
	}
	StageOrigin::Write();
}

void MapEditScene::Read()
{
	StageOrigin::Read();
	//math_����l�󂯎��
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int z = 0; z < mathVolume_.y; z++)
		{
			table_->SetMathType(XMFLOAT2(x, z), math_[x][z].mathType_);
			table_->SetMathTransform(XMFLOAT2(x, z), math_[x][z].mathPos_);
		}
	}
}
