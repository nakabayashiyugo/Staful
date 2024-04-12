#include "MapEditScene.h"

#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Text.h"

#include "resource.h"
#include "SceneTransition.h"
#include "PlayScene.h"
#include "Button.h"

MapEditScene::MapEditScene(GameObject* parent)
	: GameObject(parent, "MapEditScene"),
	//�R�X�g�Ǘ��ɂ���
	costTextPos_(XMFLOAT3(0, 0, 0)),
	mathChangeNum_(0),
	//�}�X�̑I���ɂ���
	mathtype_(0),
	//�t�@�C���̏������݁E�ǂݏo���ɂ���
	saveNum_(2),
	//�e�X�g�v���C�ɂ���
	isClear_(false),
	canTest_(false),

	YSIZE(ZSIZE), 
	hTgtgRoute_(-1)
{
	for (int i = 0; i < MATHTYPE::MATH_MAX; i++)
	{
		hPict_[i] = -1;	
		hExpantion_[i] = -1;
	}
	pTrans_ = (SceneTransition*)FindObject("SceneTransition");
	XSIZE =  (int)pTrans_->GetMathSize_x();
	YSIZE =  (int)pTrans_->GetMathSize_z();

	Math_Resize(XSIZE, YSIZE, &math_);
	Math_Resize(XSIZE, YSIZE, &math_origin_);
	Math_Resize(XSIZE, YSIZE, &isConvRot_);

	Texture* pTexture = (Texture*)FindObject("Texture");

	saveNum_ = pTrans_->GetSaveNum();

	//��Q���̂����鐧������
	int mathChangeNumLimitFirst;
	int mathChangeNumLimitPlus;
	if (XSIZE >= YSIZE)
	{
		mathChangeNumLimitFirst = XSIZE;
		mathChangeNumLimitPlus = XSIZE / 2;
	}
	else
	{
		mathChangeNumLimitFirst = YSIZE;
		mathChangeNumLimitPlus = YSIZE / 2;
	}
	mathChangeNumLimit_ = mathChangeNumLimitFirst + (pTrans_->GetTurnNum() - 1) * mathChangeNumLimitPlus;

	Read();
}

void MapEditScene::Initialize()
{
	std::string filename[MATH_MAX] =
	{
		"Math_Floor.png",
		"Math_Wall.png",
		"MATH_Hole.png",
		"Math_Conveyor.png",
		"Math_Togetoge.png",
		"Math_PitFall.png",
		"Math_Start.png",
		"Math_Goal.png",
	};
	for (int i = 0; i < MATH_MAX; i++)
	{
		filename[i] = "Assets\\" + filename[i];
		hPict_[i] = Image::Load(filename[i]);
		assert(hPict_[i] >= 0);
	}
	//�e�N�X�`���̃T�C�Y
	XMFLOAT3 imageSize = Image::GetTextureSize(hPict_[0]);

	//�{�^���̏�����
	ButtonInit(imageSize);
	//�}�X�̐����̏�����
	ExpantionInit();
	//�}�X�̃T�C�Y����
	for (int x = 0; x < XSIZE; x++)
	{
		for (int y = 0; y < YSIZE; y++)
		{
			math_origin_[x][y] = math_[x][y];
			math_[x][y].mathPos_.scale_ = XMFLOAT3(1.0f / imageSize.x * MATHSIZE, 1.0f / imageSize.y * MATHSIZE, 1);
			math_[x][y].mathPos_.position_.x = ((float)x / Direct3D::scrWidth) * MATHSIZE + ((float)(x - XSIZE) / Direct3D::scrWidth) * MATHSIZE;
			math_[x][y].mathPos_.position_.y = ((float)y / Direct3D::scrHeight) * MATHSIZE + ((float)(y - YSIZE) / Direct3D::scrHeight) * MATHSIZE;
		}
	}

	hTgtgRoute_ = Image::Load("Assets\\Togetoge_Route.png");
	assert(hTgtgRoute_ >= 0);

	pText_ = new Text();
	pText_->Initialize();
}

void MapEditScene::Update()
{
	//�}�X�̈ʒu�̏����l
	const XMFLOAT3 mathInitPos = XMFLOAT3(-1, -1, 0);
	//�J�[�\�����u����Ă�}�X�̈ʒu
	static XMFLOAT3 selectMath;
	//�Ƃ��Ƃ��}�X���������ʒu
	static XMFLOAT3 tgtgRouteMathDown = XMFLOAT3(-1, -1, 0);
	//�Ƃ��Ƃ��}�X�������ăh���b�O���Ęb�����ʒu
	static XMFLOAT3 tgtgRouteMathUp = XMFLOAT3(-1, -1, 0);

	//�}�E�X�̈ʒu
	float mousePosX = Input::GetMousePosition().x;
	float mousePosY = Input::GetMousePosition().y;

	//x�����̃}�X�̐�
	const int xsizeMax = XSIZE - 1;
	//y�����̃}�X�̐�
	const int ysizeMax = YSIZE - 1;
	mousePosX -= ((math_[0][0].mathPos_.position_.x + 1.0f) * Direct3D::scrWidth / 2) - MATHSIZE / 2;
	mousePosY -= ((-(math_[xsizeMax][ysizeMax].mathPos_.position_.y) + 1.0f) * Direct3D::scrHeight / 2) - MATHSIZE / 2;

	selectMath.x = (int)(mousePosX / MATHSIZE);
	selectMath.y = ysizeMax - (int)(mousePosY / MATHSIZE);

	//�}�E�X�̈ʒu���}�X�ڂ���o����
	if (selectMath.x < 0 || selectMath.x >= XSIZE ||
		selectMath.y < 0 || selectMath.y >= YSIZE)
	{
		selectMath = mathInitPos;
	}

	//�I�΂�Ă�}�X�̎�ތ���
	SelectMathType();

	if (selectMath.x != -1 && selectMath.y != -1)
	{
		if (math_origin_[(int)selectMath.x][(int)selectMath.y].mathType_ == MATH_FLOOR)
		{
			switch ((MATHTYPE)mathtype_)
			{
			case MATH_START:
				if (Input::IsMouseButton(0))
				{
					if (pTrans_->GetTurnNum() == 1)
					{
						//�X�^�[�g�}�X�����łɂ��邩�ǂ����T��
						for (int x = 0; x < XSIZE; x++)
						{
							for (int y = 0; y < YSIZE; y++)
							{
								if (math_[x][y].mathType_ == MATH_START)
								{
									math_[x][y].mathType_ = MATH_FLOOR;
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
					if (pTrans_->GetTurnNum() == 1)
					{
						//�S�[���}�X�����łɂ��邩�ǂ����T��
						for (int x = 0; x < XSIZE; x++)
						{
							for (int y = 0; y < YSIZE; y++)
							{
								if (math_[x][y].mathType_ == MATH_GOAL)
								{
									math_[x][y].mathType_ = MATH_FLOOR;
								}
							}
						}
						ChangeSelectMath(selectMath);
					}
				}
				break;
			case MATH_FLOOR:
				if (Input::IsMouseButton(0))
				{
					ChangeSelectMath(selectMath);
				}
			case MATH_CONVEYOR:
				if (!isMathChangeNumLimit())
				{
					if (Input::IsMouseButton(0))
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
					if (!isMathChangeNumLimit())
					{
						tgtgRouteMathDown = XMFLOAT3((int)selectMath.x, (int)selectMath.y, 0);
						ChangeSelectMath(selectMath);

						//tTgtgRoute_�ɒǉ�
						TOGETOGEROUTE* ptg = new TOGETOGEROUTE();
						ptg->initPos_ = ptg->destPos_ = tgtgRouteMathDown;
						ptg->route_.scale_ = XMFLOAT3(0, 0, 0);
						tTgtgRoute_.push_back(*ptg);
						delete ptg;
					}
				}
				if (Input::IsMouseButtonDown(1))
				{
					if (math_[(int)selectMath.x][(int)selectMath.y].mathType_ == MATH_TOGETOGE)
					{
						tgtgRouteMathDown = XMFLOAT3((int)selectMath.x, (int)selectMath.y, 0);
						isClear_ = false;
					}
				}
				break;
			default:
				if (!isMathChangeNumLimit())
				{
					if (Input::IsMouseButton(0))
					{
						ChangeSelectMath(selectMath);
					}
				}
				break;
			}
		}
	}


	//�Ƃ��Ƃ��}�X�ŃN���b�N���Ęb�����Ƃ�
	if (Input::IsMuoseButtonUp(1))
	{
		if (tgtgRouteMathDown.x != -1)
		{
			tgtgRouteMathUp = XMFLOAT3((int)mousePosX / MATHSIZE, YSIZE - 1 - (int)(mousePosY / MATHSIZE), 0);

			auto itr = tTgtgRoute_.begin();

			while (itr != tTgtgRoute_.end())
			{
				if (itr->initPos_.x == tgtgRouteMathDown.x &&
					itr->initPos_.y == tgtgRouteMathDown.y)
				{
					break;
				}
				itr++;
			}
			//�Ƃ��Ƃ����[�g�̑���
			const int tgtgRouteThick = 5;
			//�c�ړ�
			if (abs(tgtgRouteMathUp.x - tgtgRouteMathDown.x) < abs(tgtgRouteMathUp.y - tgtgRouteMathDown.y))
			{

				itr->route_.scale_.x = math_[0][0].mathPos_.scale_.x / tgtgRouteThick;
				itr->route_.scale_.y = math_[0][0].mathPos_.scale_.y * abs(tgtgRouteMathUp.y - tgtgRouteMathDown.y);

				itr->route_.position_ = math_[(int)tgtgRouteMathDown.x][((int)tgtgRouteMathUp.y + tgtgRouteMathDown.y) / 2].mathPos_.position_;

				if (((int)tgtgRouteMathUp.y + (int)tgtgRouteMathDown.y) % 2 != 0)
				{
					itr->route_.position_.y += (math_[0][0].mathPos_.scale_.y) / 2;
				}
				itr->destPos_.y = tgtgRouteMathUp.y;
				itr->destPos_.x = tgtgRouteMathDown.x;
			}
			//���ړ�
			else
			{
				itr->route_.scale_.x = math_[0][0].mathPos_.scale_.x * abs(tgtgRouteMathUp.x - tgtgRouteMathDown.x);
				itr->route_.scale_.y = math_[0][0].mathPos_.scale_.y / tgtgRouteThick;

				itr->route_.position_ = math_[((int)tgtgRouteMathUp.x + (int)tgtgRouteMathDown.x) / 2][(int)tgtgRouteMathDown.y].mathPos_.position_;

				if (((int)tgtgRouteMathUp.x + (int)tgtgRouteMathDown.x) % 2 != 0)
				{
					itr->route_.position_.x += (math_[0][0].mathPos_.scale_.x) / 2;
				}
				itr->destPos_.x = tgtgRouteMathUp.x;
				itr->destPos_.y = tgtgRouteMathDown.y;
			}

			if (itr->route_.scale_.x <= 0 && itr->route_.scale_.y <= 0)
			{
				tTgtgRoute_.erase(itr);
			}
		}
		tgtgRouteMathDown = mathInitPos;
	}

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



void MapEditScene::Draw()
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
	const XMFLOAT3 costPos(1000, 700, 0);
	costTextPos_ = costPos;
	std::string str = std::to_string(mathChangeNum_) + " / " + std::to_string(mathChangeNumLimit_);
	pText_->Draw(costTextPos_.x, costTextPos_.y, str.c_str());

	//�}�X�̐����\��
	ExpantionDraw();

	for (int x = 0; x < XSIZE; x++)
	{
		for (int y = 0; y < YSIZE; y++)
		{
			//�R���x�A�̉�]
			if (isConvRot_[x][YSIZE - 1 - y])
			{
				math_[x][YSIZE - 1 - y].mathPos_.rotate_.z += 5;
			}
			if ((int)math_[x][YSIZE - 1 - y].mathPos_.rotate_.z % 90 == 0)
			{
				math_[x][YSIZE - 1 - y].mathPos_.rotate_.z = (int)(math_[x][YSIZE - 1 - y].mathPos_.rotate_.z / 90) * 90;
				isConvRot_[x][YSIZE - 1 - y] = false;
			}
			
			Image::SetTransform(hPict_[math_[x][y].mathType_], math_[x][y].mathPos_);
			Image::Draw(hPict_[math_[x][y].mathType_]);

		}
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
		case IDC_MAPEDIT_FLOOR:		mathtype_ = MATH_FLOOR; break;
		case IDC_MAPEDIT_WALL:		mathtype_ = MATH_WALL; break;
		case IDC_MAPEDIT_HOLL:		mathtype_ = MATH_HOLE; break;
		case IDC_MAPEDIT_CONVEYOR:	mathtype_ = MATH_CONVEYOR; break;
		case IDC_MAPEDIT_TOGETOGE:	mathtype_ = MATH_TOGETOGE; break;
		case IDC_MAPEDIT_PITFALL:	mathtype_ = MATH_PITFALL; break;
		case IDC_MAPEDIT_START:		mathtype_ = MATH_START; break;
		case IDC_MAPEDIT_GOAL:		mathtype_ = MATH_GOAL; break;
		case IDC_MAPEDIT_COMPLETE:	
			for (int x = 0; x < XSIZE; x++)
			{
				for (int y = 0; y < YSIZE; y++)
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

void MapEditScene::Write()
{
	std::ofstream write;
	std::string savefile = saveFolderName + "saveMath";
	savefile += std::to_string(saveNum_);
	write.open(savefile, std::ios::out);

	//  �t�@�C�����J���Ȃ������Ƃ��̃G���[�\��
	if (!write) {
		std::cout << "�t�@�C�� " << savefile << " ���J���܂���";
		return;
	}
	for (int i = 0; i < XSIZE; i++) {
		for (int j = 0; j < YSIZE; j++)
		{
			write.write((char*)&math_[i][j], sizeof(math_[i][j]));
			//������ł͂Ȃ��f�[�^����������
		}
	}
	write.close();  //�t�@�C�������

	//�Ƃ��Ƃ����[�g
	savefile = saveFolderName + "tgtgRoute";
	savefile += std::to_string(saveNum_);
	write.open(savefile, std::ios::out);
	//  �t�@�C�����J���Ȃ������Ƃ��̃G���[�\��
	if (!write) {
		std::cout << "�t�@�C�� " << savefile << " ���J���܂���";
		return;
	}
	for (int i = 0; i < tTgtgRoute_.size(); i++)
	{
		write.write((char*)&tTgtgRoute_[i], sizeof(tTgtgRoute_[i]));
	}
	write.close();  //�t�@�C�������
}

void MapEditScene::Read()
{
	std::ifstream read;
	std::string savefile = saveFolderName + "saveMath";
	
	savefile += std::to_string(saveNum_);
	read.open(savefile, std::ios::in);
	//  �t�@�C�����J��
	//  ios::in �͓ǂݍ��ݐ�p  ios::binary �̓o�C�i���`��

	if (!read) {
		std::cout << "�t�@�C�����J���܂���";
		return;
	}
	//  �t�@�C�����J���Ȃ������Ƃ��̑΍�

	//�t�@�C���̍Ō�܂ő�����
	for (int i = 0; i < XSIZE; i++)
	{
		for (int j = 0; j < YSIZE; j++)
		{
			read.read((char*)&math_[i][j], sizeof(math_[i][j]));
			//������ł͂Ȃ��f�[�^��ǂ݂���

		}
	}
	read.close();  //�t�@�C�������

	//�Ƃ��Ƃ����[�g
	savefile = saveFolderName + "tgtgRoute";
	savefile += std::to_string(saveNum_);
	read.open(savefile, std::ios::in);
	if (!read) {
		std::cout << "�t�@�C�����J���܂���";
		return;
	}

	int i = 0;
	while (!read.eof())
	{
		tTgtgRoute_.resize(tTgtgRoute_.size() + 1);
		read.read((char*)&tTgtgRoute_[i], sizeof(tTgtgRoute_[i]));
		i++;
	}
	read.close();  //�t�@�C�������
}

bool MapEditScene::isMathChangeNumLimit()
{
	int num = 0;
	for (int x = 0; x < XSIZE; x++)
	{
		for (int y = 0; y < YSIZE; y++)
		{
			if (math_[x][y].mathType_ != math_origin_[x][y].mathType_)
			{
				if (math_[x][y].mathType_ != MATH_START &&
					math_[x][y].mathType_ != MATH_GOAL)
				{
					num++;
				}
			}
		}
	}
	mathChangeNum_ = num;
	if (mathChangeNumLimit_ > num)
	{
		return false;
	}
	else
	{
		return true;
	}
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
}

void MapEditScene::ButtonInit(XMFLOAT3 _imageSize)
{
	//�}�X�I���{�^���̃t�@�C���l�[���w��
	std::string buttonName[MATH_MAX] =
	{
		"Button_Delete.png",
		"Button_Wall.png",
		"Button_Hole.png",
		"Button_Conveyor.png",
		"Button_Togetoge.png",
		"Button_PitFall.png",
		"Button_Start.png",
		"Button_Goal.png",
	};
	//���̑��̃{�^��
	//�����{�^��
	std::string buttonComplete = "Button_Complete.png";
	//�e�X�g�v���C�{�^��
	std::string buttonTestplay = "Button_TestPlay.png";
	//��̃t�@�C���������Ă�t�H���_�[
	const std::string folderName1 = "Assets\\";
	const std::string folderName2 = "MathButton\\";
	//�}�X�I���{�^���̉摜�ԍ�
	int mathButtonNum[MATH_MAX];
	//���̑��̃{�^���̉摜�ԍ�
	int completeNum = -1;
	int testplayNum = -1;
	//�}�X�I���{�^���̃��[�h
	for (int i = 0; i < MATH_MAX; i++)
	{
		buttonName[i] = folderName1 + folderName2 + buttonName[i];

		mathButtonNum[i] = Image::Load(buttonName[i]);
		assert(mathButtonNum[i] >= 0);
	}
	//�����{�^���̃��[�h
	buttonComplete = folderName1 + folderName2 + buttonComplete;
	completeNum = Image::Load(buttonComplete);
	assert(completeNum >= 0);
	//�e�X�g�v���C�{�^���̃��[�h
	buttonTestplay = folderName1 + folderName2 + buttonTestplay;
	testplayNum = Image::Load(buttonTestplay);
	assert(testplayNum >= 0);

	//�{�^���̕��ʂ̃}�X�Ƃ̔{��
	const float normalMathtoMult = 2.0f;
	//�{�^���̃T�C�Y
	const float mathButtonSize = MATHSIZE * normalMathtoMult;
	//�{�^���̑傫���ݒ�
	const XMFLOAT3 mbScale = XMFLOAT3(1.0f / _imageSize.x * mathButtonSize, 1.0f / _imageSize.y * mathButtonSize, 1);
	const int buttonInitNum = 1;
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
		const XMFLOAT3 mbInitPos = XMFLOAT3(-0.9f, 0.6f, 0);
		//�}�X�I���{�^���������Ƃɉ��s����邩
		const int mbNewLineNum = 4;
		//�}�X�I���{�^���̈ʒu
		XMFLOAT3 mbPos;
		mbPos.x = (float)((buttonNum_ - buttonInitNum) % mbNewLineNum) * mbScale.x + mbInitPos.x;
		mbPos.y = -(((float)((buttonNum_ - buttonInitNum) / mbNewLineNum * mbNewLineNum) / Direct3D::scrHeight) * mathButtonSize) + mbInitPos.y;
		mbPos.z = mbInitPos.z;

		Transform mbTransform;
		mbTransform.position_ = mbPos;
		mbTransform.scale_ = mbScale;
		pMathButton_[buttonNum_]->SetTransform(mbTransform);
	}

	//Delete�{�^��
	//Delete�{�^���̔ԍ�
	buttonNum_ = (int)MATH_FLOOR;

	pMathButton_[buttonNum_]->Instantiate<Button>(this);
	//Delete�{�^���̃I�u�W�F�N�g�l�[��
	buttonStr = "Button";
	buttonStr += std::to_string(buttonNum_);
	pMathButton_[buttonNum_] = (Button*)FindObject(buttonStr);
	pMathButton_[buttonNum_]->SetPictNum(mathButtonNum[buttonNum_]);

	//�}�X�I���{�^���̊�̈ʒu
	const XMFLOAT3 dbPos = XMFLOAT3(-0.9f, -0.3f, 0);
	
	Transform dbTransform;
	dbTransform.position_ = dbPos;
	dbTransform.scale_ = mbScale;
	pMathButton_[buttonNum_]->SetTransform(dbTransform);

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

	const XMFLOAT3 cbPos = XMFLOAT3(0.8f, -0.6f, 0);
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

	const XMFLOAT3 tbPos = XMFLOAT3(0.5f, -0.6f, 0);
	Transform tbTransform;
	tbTransform.position_ = tbPos;
	tbTransform.scale_ = obScale;
	pTestplayButton_->SetTransform(tbTransform);
}

void MapEditScene::ExpantionInit()
{
	//�}�X�̐����̃t�@�C���l�[��
	std::string expantionName[MATH_MAX] =
	{
		"",
		"Expantion_Wall.png",
		"Expantion_Hole.png",
		"Expantion_Conveyor.png",
		"Expantion_Togetoge.png",
		"Expantion_PitFall.png",
		"Expantion_Start.png",
		"Expantion_Goal.png",
	};
	//�}�X�̐����̉摜�������Ă�t�H���_��
	std::string folderName1 = "Assets\\";
	std::string folderName2 = "MathExpantion\\";
	//�}�X�̐����摜���[�h
	for (int i = 1; i < MATH_MAX; i++)
	{
		expantionName[i] = folderName1 + folderName2 + expantionName[i];

		hExpantion_[i] = Image::Load(expantionName[i]);
		assert(hExpantion_[i] >= 0);
	}
}

void MapEditScene::ExpantionDraw()
{
	const XMFLOAT3 exPos = XMFLOAT3(0.7f, 0.1f, 0);
	const XMFLOAT3 exScale = XMFLOAT3(0.7f, 0.7f, 1);
	tExpantion_.position_ = exPos;
	tExpantion_.scale_ = exScale;

	Image::SetTransform(hExpantion_[mathtype_], tExpantion_);
	Image::Draw(hExpantion_[mathtype_]);
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
	pTestplayButton_->SetIsCanPush(canTest_);
	//�e�X�g�v���C�{�^���������ꂽ��
	if (pTestplayButton_->GetIsClicked())
	{
		pTestplayButton_->SetIsClicked(false);
		IsHidden(true);
		Write();
		Instantiate<PlayScene>(this);
	}
	//�e�X�g�v���C�N���A�ł��Ȃ��Ɖ����Ȃ��悤�ɂ���
	pCompleteButton_->SetIsCanPush(isClear_);
	//�����{�^���������ꂽ��
	if (pCompleteButton_->GetIsClicked())
	{
		Write();
		pTrans_->SetNextScene();
		KillMe();
	}
}

void MapEditScene::IsHidden(bool _isHidden)
{
	//�}�X�\����\���؂�ւ�
	for (int i = 0; i < math_.size(); i++)
	{
		for (int j = 0; j < math_[i].size(); j++)
		{
			math_[i][j].mathPos_.position_.z = _isHidden;
		}
	}
	//�Ƃ��Ƃ����[�g�\����\���؂�ւ�
	for (int i = 0; i < tTgtgRoute_.size(); i++)
	{
		tTgtgRoute_[i].route_.position_.z = _isHidden;
	}
	//�R�X�g�\����\���؂�ւ�
	costTextPos_.z = _isHidden;
	//�}�X�̐����\����\���ؑ�
	tExpantion_.position_.z = _isHidden;
	//�{�^���\����\���؂�ւ�
	for (int i = 0; i < MATH_MAX; i++)
	{
		Transform mbTransform = pMathButton_[i]->GetTransform();
		mbTransform.position_.z = _isHidden;
		pMathButton_[i]->SetTransform(mbTransform);
	}
	//�����{�^���\����\���؂�ւ�
	Transform cpTransform = pCompleteButton_->GetTransform();
	cpTransform.position_.z = _isHidden;
	pCompleteButton_->SetTransform(cpTransform);
	//�e�X�g�v���C�{�^���\����\���؂�ւ�
	Transform tpTransform = pTestplayButton_->GetTransform();
	tpTransform.position_.z = _isHidden;
	pTestplayButton_->SetTransform(tpTransform);
}

void MapEditScene::CheckCanTest()
{
	//�X�^�[�g�}�X�ƃS�[���}�X�������ĂȂ��ƃe�X�g�v���C���ł��Ȃ��悤�ɂ���
	bool isStart = false, isGoal = false;
	for (int x = 0; x < XSIZE; x++)
	{
		for (int y = 0; y < YSIZE; y++)
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
