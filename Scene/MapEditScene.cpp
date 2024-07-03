#include "MapEditScene.h"

#include "../Engine/SceneManager.h"
#include "../Engine/Image.h"
#include "../Engine/Input.h"
#include "../Engine/Text.h"
#include "../Engine/Audio.h"

#include "../resource.h"
#include "../Button.h"
#include "../GamePlayer.h"

#include "SceneTransition.h"
#include "PlayScene.h"

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
	//�}�E�X����
	mousePos_(0, 0, 0),
	//�R�X�g�Ǘ��ɂ���
	curCost_(0),
	//�}�X�̑I���ɂ���
	mathtype_(0),
	//�e�X�g�v���C�ɂ���
	isClear_(false),
	canTest_(false),
	isDisp_(true),
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

	Math_Resize(mathVolume_.x, mathVolume_.z, &math_);
	Math_Resize(mathVolume_.x, mathVolume_.z, &math_origin_);
	Math_Resize(mathVolume_.x, mathVolume_.z, &isConvRot_);

	pGP_ = (GamePlayer*)this->pParent_;
	saveNum_ = pGP_->GetSaveNum();

	Read();

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

	costLimitFirst = mathVolume_.x + mathVolume_.z;
	costLimitPlus = costLimitFirst / 2;
	turnNum_ = pGP_->GetTurnNum();
	costLimit_ = costLimitFirst + (turnNum_ - 1) * costLimitPlus;
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
}

void MapEditScene::Update()
{
	Audio::Play(hAudio_Music_, soundVolume);
	//�J�[�\�����u����Ă�}�X�̈ʒu
	static XMFLOAT3 selectMath;
	
	MousePosSet();

	selectMath.x = (int)(mousePos_.x / MATHSIZE);
	selectMath.y = mathVolume_.z - 1 - (int)(mousePos_.y / MATHSIZE);

	//�}�E�X�̈ʒu���}�X�ڂ���o����
	if (selectMath.x < 0 || selectMath.x >= mathVolume_.x ||
		selectMath.y < 0 || selectMath.y >= mathVolume_.z)
	{
		selectMath = mathInitPos;
	}

	//�I�΂�Ă�}�X�̎�ތ���
	SelectMathType();

	if (selectMath.x != -1 && selectMath.y != -1)
	{
		if (math_origin_[(int)selectMath.x][(int)selectMath.y].mathType_ == MATH_DELETE)
		{
			switch ((MATHTYPE)mathtype_)
			{
			case MATH_START:
				if (Input::IsMouseButton(0))
				{
					if (turnNum_ == 1)
					{
						//�X�^�[�g�}�X�����łɂ��邩�ǂ����T��
						for (int x = 0; x < mathVolume_.x; x++)
						{
							for (int y = 0; y < mathVolume_.z; y++)
							{
								if (math_[x][y].mathType_ == MATH_START)
								{
									math_[x][y].mathType_ = MATH_DELETE;
								}
							}
						}
						ChangeSelectMath(selectMath);
					}
				}
				break;
			case MATH_GOAL:
				if (Input::IsMouseButton(0))
				{
					if (turnNum_ == 1)
					{
						//�S�[���}�X�����łɂ��邩�ǂ����T��
						for (int x = 0; x < mathVolume_.x; x++)
						{
							for (int y = 0; y < mathVolume_.z; y++)
							{
								if (math_[x][y].mathType_ == MATH_GOAL)
								{
									math_[x][y].mathType_ = MATH_DELETE;
								}
							}
						}
						ChangeSelectMath(selectMath);
					}
				}
				break;
			case MATH_DELETE:
				if (Input::IsMouseButton(0))
				{
					ChangeSelectMath(selectMath);
				}
			case MATH_CONVEYOR:
				if (Input::IsMouseButton(0))
				{
					if (CostManagement(selectMath))
					{
						ChangeSelectMath(selectMath);
					}
				}
				if (Input::IsMouseButtonDown(1))
				{
					if (math_[(int)selectMath.x][(int)selectMath.y].mathType_ == MATHTYPE::MATH_CONVEYOR)
					{
						isConvRot_[(int)selectMath.x][(int)selectMath.y] = true;
						isClear_ = false;
					}
				}
				break;
			case MATH_TOGETOGE:
				if (Input::IsMouseButton(0))
				{
					if (CostManagement(selectMath))
					{
						tgtgRouteMathDown_ = XMFLOAT3((int)selectMath.x, (int)selectMath.y, 0);
						ChangeSelectMath(selectMath);

						//tTgtgRoute�̒��Ɉʒu�������ȂƂ��Ƃ������݂��Ă邩
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
						//���݂��Ȃ�������
						if (itr == tTgtgRoute_.end())
						{
							//tTgtgRoute_�ɒǉ�
							TOGETOGEROUTE* ptg = new TOGETOGEROUTE();
							ptg->initPos_ = ptg->destPos_ = tgtgRouteMathDown_;
							ptg->route_.scale_ = XMFLOAT3(0, 0, 0);
							tTgtgRoute_.push_back(*ptg);
							delete ptg;
						}
					}
				}
				if (Input::IsMouseButtonDown(1))
				{
					if (math_[(int)selectMath.x][(int)selectMath.y].mathType_ == MATH_TOGETOGE)
					{
						tgtgRouteMathDown_ = XMFLOAT3((int)selectMath.x, (int)selectMath.y, 0);
						isClear_ = false;
					}
				}
				break;
			default:
				if (Input::IsMouseButton(0))
				{
					if (CostManagement(selectMath))
					{
						ChangeSelectMath(selectMath);
					}
				}
				break;
			}
		}
	}

	//�Ƃ��Ƃ����[�g�ݒ�
	TogetogeRouteSet();
	//�Ƃ��Ƃ��}�X���ق��̃}�X�ɕς�������̗v�f�̍폜
	TogetogeElemDelete();
}

void MapEditScene::Draw()
{
	IsDisplay(isDisp_);
	if(isDisp_)
	{
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
		//�R�X�g�\��
		CostDraw();

		//�}�X�̐����\��
		ExpantionDraw();
		//�}�X�\��
		MathDraw();

		//�v���C���[�ԍ��\��
		pGP_->PlayerNumDraw();
	}
}

void MapEditScene::Release()
{
}

BOOL MapEditScene::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		//���W�I�{�^���̏����l
		SendMessage(GetDlgItem(hDlg, IDC_MAPEDIT_FLOOR), BM_SETCHECK, BST_CHECKED, 0);

		return TRUE;

	case WM_COMMAND:
		bool startFlg = false, goalFlg = false;
		switch (LOWORD(wp))
		{
		case IDC_MAPEDIT_FLOOR:		mathtype_ = MATH_DELETE; break;
		case IDC_MAPEDIT_WALL:		mathtype_ = MATH_WALL; break;
		case IDC_MAPEDIT_HOLL:		mathtype_ = MATH_HOLE; break;
		case IDC_MAPEDIT_CONVEYOR:	mathtype_ = MATH_CONVEYOR; break;
		case IDC_MAPEDIT_TOGETOGE:	mathtype_ = MATH_TOGETOGE; break;
		case IDC_MAPEDIT_PITFALL:	mathtype_ = MATH_PITFALL; break;
		case IDC_MAPEDIT_START:		mathtype_ = MATH_START; break;
		case IDC_MAPEDIT_GOAL:		mathtype_ = MATH_GOAL; break;
		case IDC_MAPEDIT_COMPLETE:	
			for (int x = 0; x < mathVolume_.x; x++)
			{
				for (int y = 0; y < mathVolume_.z; y++)
				{
					if (math_[x][y].mathType_ == MATH_START)
					{
						startFlg = true;
					}
					if (math_[x][y].mathType_ == MATH_GOAL)
					{
						goalFlg = true;
					}
				}
			}
			if (startFlg && goalFlg)
			{
				Write();
				EndDialog(hDlg, 0);
			}
			break;
		default: break;
		}

		return TRUE;
	}
	return FALSE;
}

void MapEditScene::MousePosSet()
{
	//�}�E�X�̈ʒu
	mousePos_.x = Input::GetMousePosition().x;
	mousePos_.y = Input::GetMousePosition().y;

	mousePos_.x -= ((math_[0][0].mathPos_.position_.x + 1.0f) * Direct3D::bfr_scrWidth / 2) - MATHSIZE / 2;
	mousePos_.y -= ((-(math_[mathVolume_.x - 1][mathVolume_.z - 1].mathPos_.position_.y) + 1.0f) * Direct3D::bfr_scrHeight / 2) - MATHSIZE / 2;
}

bool MapEditScene::CostManagement(XMFLOAT3 _selectMath)
{
	int costSum = 0;
	//�ύX�O�̃R�X�g�v�Z
	for (int x = 0; x < math_.size(); x++)
	{
		for (int y = 0; y < math_[x].size(); y++)
		{
			costSum += costs_[math_[x][y].mathType_];
		}
	}
	//�ύX��̃R�X�g����
	if (math_[_selectMath.x][_selectMath.y].mathType_ != mathtype_)
	{
		costSum += costs_[mathtype_];
		//�R�X�g�̐����𒴂��Ă�����
		if (costSum > costLimit_)
		{
			costSum -= costs_[mathtype_];
			curCost_ = costSum;
			return false;
		}
	}
	curCost_ = costSum;
	return true;
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
	std::string str = std::to_string(curCost_) + " / " + std::to_string(costLimit_);
	pText_->Draw(curCostPos.x, curCostPos.y, str.c_str());
	//�����ucostLimit�v�\��
	pText_->Draw(costLimitPos.x, costLimitPos.y, strCostLimit.c_str());
}

void MapEditScene::ChangeSelectMath(XMFLOAT3 _selectMath)
{
	//�N���b�N�����}�X��I��ł�}�X�ɕς���
	if (math_[(int)_selectMath.x][(int)_selectMath.y].mathType_ != MATH_CONVEYOR)
	{
		math_[(int)_selectMath.x][(int)_selectMath.y].mathPos_.rotate_ = XMFLOAT3(0, 0, 0);
	}
	math_[(int)_selectMath.x][(int)_selectMath.y].mathType_ = (MATHTYPE)mathtype_;
	isClear_ = false;

	//�e�X�g�v���C�ł��邩�ǂ����`�F�b�N
	CheckCanTest();

	//�}�X��u��������SE
	//���O�܂ŃJ�[�\���̒u����Ă����}�X
	static XMFLOAT3 prevMath = mathInitPos;
	if (prevMath.x != _selectMath.x || prevMath.y != _selectMath.y)
	{
		Audio::Stop(hSE_PutMath_);
	}
	Audio::Play(hSE_PutMath_, 1);
	prevMath = _selectMath;
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
	//���~�{�^��
	std::string buttonCancel = "Button_Cancel.png";

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

	//���~�{�^���̃��[�h
	folderName = "Assets\\Button\\";
	buttonCancel = folderName + buttonCancel;
	cancelNum = Image::Load(buttonCancel);
	assert(cancelNum >= 0);

	//�{�^���̕��ʂ̃}�X�Ƃ̔{��
	const float normalMathtoMult = 2.0f;
	//�{�^���̃T�C�Y
	const float mathButtonSize = MATHSIZE * normalMathtoMult;
	//�{�^���̑傫���ݒ�
	const XMFLOAT3 mbScale = XMFLOAT3(1.0f / imageSize.x * mathButtonSize, 1.0f / imageSize.y * mathButtonSize, 1);
	const int buttonInitNum = 0;
	//�{�^���̃I�u�W�F�N�g�l�[��
	std::string buttonStr;

	for (buttonNum_ = buttonInitNum; buttonNum_ < MATH_MAX; buttonNum_++)
	{
		pMathButton_[buttonNum_]->Instantiate<Button>(this);
		//�T���{�^���̃I�u�W�F�N�g�l�[��
		buttonStr = "Button";
		buttonStr += std::to_string(buttonNum_);
		pMathButton_[buttonNum_] = (Button*)FindObject(buttonStr);
		pMathButton_[buttonNum_]->SetPictNum(mathButtonNum[buttonNum_]);

		
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
		const XMFLOAT3 mbInterval = XMFLOAT3(((buttonNum_ - buttonInitNum) % mbNewLineNum) * mbIntervalBase.x,
			-((buttonNum_ - buttonInitNum) / mbNewLineNum) * mbIntervalBase.y, 0);
		mbPos.x =  (float)((buttonNum_ - buttonInitNum) % mbNewLineNum) * mbSize.x + mbInitPos.x + mbInterval.x;
		mbPos.y = -(float)((buttonNum_ - buttonInitNum) / mbNewLineNum) * mbSize.y + mbInitPos.y + mbInterval.y;
		mbPos.z = mbInitPos.z;

		Transform mbTransform;
		mbTransform.position_ = mbPos;
		mbTransform.scale_ = mbScale;
		pMathButton_[buttonNum_]->SetTransform(mbTransform);
	}

	//���̑��̃{�^���̑傫��
	const XMFLOAT3 obScale = XMFLOAT3(0.3f, 0.3f, 1);
	//�����{�^��
	//�����{�^���̔ԍ�
	buttonNum_ = (int)MATH_MAX;
	pCompleteButton_->Instantiate<Button>(this);
	//�����{�^���̃I�u�W�F�N�g�l�[��
	buttonStr = "Button";
	buttonStr += std::to_string(buttonNum_);
	pCompleteButton_ = (Button*)FindObject(buttonStr);
	pCompleteButton_->SetPictNum(completeNum);

	const XMFLOAT3 cbPos = XMFLOAT3(-0.8f, -0.7f, 0);
	Transform cbTransform;
	cbTransform.position_ = cbPos;
	cbTransform.scale_ = obScale;
	pCompleteButton_->SetTransform(cbTransform);

	//�e�X�g�v���C�{�^��
	//�e�X�g�v���C�{�^���̔ԍ�
	buttonNum_++;
	pTestplayButton_->Instantiate<Button>(this);
	//�e�X�g�v���C�{�^���̃I�u�W�F�N�g�l�[��
	buttonStr = "Button";
	buttonStr += std::to_string(buttonNum_);
	pTestplayButton_ = (Button*)FindObject(buttonStr);
	pTestplayButton_->SetPictNum(testplayNum);

	const XMFLOAT3 tbPos = XMFLOAT3(-0.8f, -0.6f, 0);
	Transform tbTransform;
	tbTransform.position_ = tbPos;
	tbTransform.scale_ = obScale;
	pTestplayButton_->SetTransform(tbTransform);

	//���~�{�^��
	buttonNum_++;
	pCancelButton_->Instantiate<Button>(this);
	//���~�{�^���̃I�u�W�F�N�g�l�[��
	buttonStr = "Button";
	buttonStr += std::to_string(buttonNum_);
	pCancelButton_ = (Button*)FindObject(buttonStr);
	pCancelButton_->SetPictNum(cancelNum);

	const XMFLOAT3 ccbPos = tbPos;
	Transform ccbTransform;
	ccbTransform.position_ = ccbPos;
	ccbTransform.scale_ = obScale;
	pCancelButton_->SetTransform(ccbTransform);

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
	//�}�X�̃T�C�Y����
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.z; y++)
		{
			math_origin_[x][y] = math_[x][y];
			math_[x][y].mathPos_.scale_ = XMFLOAT3(1.0f / imageSize.x * MATHSIZE, 1.0f / imageSize.y * MATHSIZE, 1);
			//1�}�X�̑傫��
			const XMFLOAT2 mathSize = XMFLOAT2(((float)x / Direct3D::bfr_scrWidth) * MATHSIZE * 2,
				((float)y / Direct3D::bfr_scrHeight) * MATHSIZE * 2);
			//�}�X��u���ʒu
			const XMFLOAT2 mathPos = XMFLOAT2(mathInitPos.x - ((float)mathVolume_.x / Direct3D::bfr_scrWidth) * MATHSIZE,
				mathInitPos.y - ((float)mathVolume_.z / Direct3D::bfr_scrHeight) * MATHSIZE);

			math_[x][y].mathPos_.position_.x = mathSize.x + mathPos.x;
			math_[x][y].mathPos_.position_.y = mathSize.y + mathPos.y;
		}
	}
}

void MapEditScene::MathDraw()
{
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.z; y++)
		{
			//�R���x�A�̉�]
			if (isConvRot_[x][mathVolume_.z - 1 - y])
			{
				math_[x][mathVolume_.z - 1 - y].mathPos_.rotate_.z += 5;
				Audio::Play(hSE_ConvRot_, 1);
				if ((int)math_[x][mathVolume_.z - 1 - y].mathPos_.rotate_.z % 90 == 0)
				{
					math_[x][mathVolume_.z - 1 - y].mathPos_.rotate_.z = (int)(math_[x][mathVolume_.z - 1 - y].mathPos_.rotate_.z / 90) * 90;
					isConvRot_[x][mathVolume_.z - 1 - y] = false;
					Audio::Stop(hSE_ConvRot_);
				}
			}

			Image::SetTransform(hPict_[math_[x][y].mathType_], math_[x][y].mathPos_);
			Image::Draw(hPict_[math_[x][y].mathType_]);

		}
	}
}

void MapEditScene::SelectMathType()
{
	for (int i = 0; i < MATH_MAX; i++)
	{
		if (pMathButton_[i]->GetIsClicked())
		{
			mathtype_ = i;
		}
	}
	static PlayScene* pTestplay = nullptr;

	pTestplayButton_->SetIsCanPush(canTest_ * isDisp_);
	//�e�X�g�v���C�{�^���������ꂽ��
	if (pTestplayButton_->GetIsClicked())
	{
		pTestplayButton_->SetIsClicked(false);
		//�}�b�v�G�f�B�^�[��\��
		isDisp_ = false;
		Write();
		pTestplay->Instantiate<PlayScene>(this);
		pTestplay = (PlayScene*)FindObject("PlayScene");
	}

	//�e�X�g�v���C�N���A�ł��Ȃ��Ɖ����Ȃ��悤�ɂ���
	pCompleteButton_->SetIsCanPush(isClear_ * isDisp_);
	//�����{�^���������ꂽ��
	if (pCompleteButton_->GetIsClicked())
	{
		//���y��~
		Audio::Stop(hAudio_Music_);
		//�t�@�C���ɏ�������
		Write();
		pGP_->MapEditFinished();
	}

	//���~�{�^��
	//�e�X�g�v���C���Ă鎞����������悤�ɂ���
	pCancelButton_->SetIsCanPush(!isDisp_);
	//�e�X�g�v���C���Ă���Ƃ�
	if (pTestplay != nullptr)
	{
		if (pCancelButton_->GetIsClicked())
		{
			pCancelButton_->SetIsClicked(false);
			pTestplay->KillMe();
			pTestplay = nullptr;
			//�}�b�v�G�f�B�^�[�\��
			isDisp_ = true;
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
	//���~�{�^���\�����\���ؑ�
	pCancelButton_->SetIsDisplay(!_isDisp);
}

void MapEditScene::CheckCanTest()
{
	//�X�^�[�g�}�X�ƃS�[���}�X�������ĂȂ��ƃe�X�g�v���C���ł��Ȃ��悤�ɂ���
	bool isStart = false, isGoal = false;
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.z; y++)
		{
			if (math_[x][y].mathType_ == MATH_START)
			{
				isStart = true;
			}
			if (math_[x][y].mathType_ == MATH_GOAL)
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
	//�Ƃ��Ƃ��}�X�ŃN���b�N���Ęb�����Ƃ�
	if (Input::IsMuoseButtonUp(1))
	{
		if (tgtgRouteMathDown_.x != -1)
		{
			tgtgRouteMathUp_ = XMFLOAT3((int)mousePos_.x / MATHSIZE, mathVolume_.z - 1 - (int)(mousePos_.y / MATHSIZE), 0);
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
			else if (tgtgRouteMathUp_.y >= mathVolume_.x)
			{
				tgtgRouteMathUp_.y = mathVolume_.z - 1;
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
			//�c�ړ�
			if (abs(tgtgRouteMathUp_.x - tgtgRouteMathDown_.x) < abs(tgtgRouteMathUp_.y - tgtgRouteMathDown_.y))
			{

				itr->route_.scale_.x = math_[0][0].mathPos_.scale_.x / tgtgRouteThick;
				itr->route_.scale_.y = math_[0][0].mathPos_.scale_.y * abs(tgtgRouteMathUp_.y - tgtgRouteMathDown_.y);

				itr->route_.position_ = math_[(int)tgtgRouteMathDown_.x][((int)tgtgRouteMathUp_.y + tgtgRouteMathDown_.y) / 2].mathPos_.position_;

				if (((int)tgtgRouteMathUp_.y + (int)tgtgRouteMathDown_.y) % 2 != 0)
				{
					itr->route_.position_.y += (math_[0][0].mathPos_.scale_.y) / 2;
				}
				itr->destPos_.y = tgtgRouteMathUp_.y;
				itr->destPos_.x = tgtgRouteMathDown_.x;
			}
			//���ړ�
			else
			{
				itr->route_.scale_.x = math_[0][0].mathPos_.scale_.x * abs(tgtgRouteMathUp_.x - tgtgRouteMathDown_.x);
				itr->route_.scale_.y = math_[0][0].mathPos_.scale_.y / tgtgRouteThick;

				itr->route_.position_ = math_[((int)tgtgRouteMathUp_.x + (int)tgtgRouteMathDown_.x) / 2][(int)tgtgRouteMathDown_.y].mathPos_.position_;

				if (((int)tgtgRouteMathUp_.x + (int)tgtgRouteMathDown_.x) % 2 != 0)
				{
					itr->route_.position_.x += (math_[0][0].mathPos_.scale_.x) / 2;
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
	}
}

void MapEditScene::TogetogeElemDelete()
{
	//���[�g������Ƃ��Ƃ��̃}�X�����̃}�X�ɕύX�ɂȂ����Ƃ�
	auto itr = tTgtgRoute_.begin();
	while (itr != tTgtgRoute_.end())
	{
		if (math_[itr->initPos_.x][itr->initPos_.y].mathType_ != MATH_TOGETOGE)
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
