#include "ResultScene.h"

#include "SceneTransition.h"

#include "../Button.h"

#include "../Engine/Image.h"
#include "../Engine/Input.h"
#include "../Engine/Direct3D.h"
#include "../Engine/SceneManager.h"

ResultScene::ResultScene(GameObject* parent)
	: GameObject(parent, "ResultScene")
{
	pST_ = (SceneTransition*)FindObject("SceneTransition");
}

void ResultScene::Initialize()
{
	isClear_.resize(PLAYERNUMMAX);
	//���ꂼ��̃v���C���[���N���A�������ǂ�������
	ResultRead();
	//�{�^���̉摜���[�h
	std::string ntFileName = "Button_NextTurn.png";
	std::string rtFileName = "Button_ReturnTitle.png";
	//�Q�[�����I��������
	if (isClear_[0] != isClear_[1])
	{
		ButtonInit(rtFileName);
	}
	//���s��������
	else
	{
		ButtonInit(ntFileName);
	}
	PictInit();
}

void ResultScene::Update()
{
	if (pButton_->GetIsClicked())
	{
		//�Q�[�����I��������
		if (isClear_[0] != isClear_[1])
		{
			SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
			pSM->ChangeScene(SCENE_ID_TITLE);
		}
		//���s��������
		else
		{
			pST_->SetNextScene();
		}
		KillMe();
	}
}

void ResultScene::Draw()
{
	PictDraw();
}

void ResultScene::Release()
{
}

void ResultScene::ButtonInit(std::string _fileName)
{
	//�{�^���̉摜�������Ă�t�H���_��
	std::string folderName = "Assets\\Button\\SceneTrans\\";
	std::string fileName = folderName + _fileName;
	hButton_ = Image::Load(fileName);
	assert(hButton_ >= 0);

	pButton_->Instantiate<Button>(this);
	pButton_ = (Button*)FindObject("Button");
	pButton_->SetPictNum(hButton_);

	//�{�^���̈ʒu
	const XMFLOAT3 bPos = XMFLOAT3(0, -0.7, 0);
	//�{�^���̑傫��
	const XMFLOAT3 bScale = XMFLOAT3(0.5f, 0.5f, 0.5f);
	Transform bTransform;
	bTransform.position_ = bPos;
	bTransform.scale_ = bScale;
	pButton_->SetTransform(bTransform);
}

void ResultScene::PictInit()
{
	//���[�h����t�@�C���̖��O
	std::string player1 = "Logo_Player1.png";
	std::string player2 = "Logo_Player2.png";
	std::string win = "Logo_Win.png";
	std::string lose = "Logo_Lose.png";
	std::string draw = "Logo_Draw.png";
	//�t�@�C���������Ă�t�H���_��
	std::string folderName = "Assets\\";
	//�t�H���_������
	player1 = folderName + player1;
	player2 = folderName + player2;
	win = folderName + win;
	lose = folderName + lose;
	draw = folderName + draw;
	//���[�h
	hPlayer1_ = Image::Load(player1);
	hPlayer2_ = Image::Load(player2);
	hWin_ = Image::Load(win);
	hLose_ = Image::Load(lose);
	hDraw_ = Image::Load(draw);
	assert(hPlayer1_ >= 0);
	assert(hPlayer2_ >= 0);
	assert(hWin_ >= 0);
	assert(hLose_ >= 0);
	assert(hDraw_ >= 0);
}
void ResultScene::PictDraw()
{
	//�摜��\������ʒu
	Transform tPict;
	const XMFLOAT3 p1Pos = XMFLOAT3(-0.5f, 0.3f, 0);
	const XMFLOAT3 p2Pos = XMFLOAT3(0.5f, 0.3f, 0);
	const XMFLOAT3 wPos = XMFLOAT3(0.5f, 0, 0);
	const XMFLOAT3 lPos = XMFLOAT3(0.5f, 0, 0);
	const XMFLOAT3 dPos = XMFLOAT3(0, -0.3f, 0);

	//�v���C���[1���S�̐ݒ�
	tPict.position_ = p1Pos;
	Image::SetTransform(hPlayer1_, tPict);
	Image::Draw(hPlayer1_);

	//�v���C���[�Q���S�̐ݒ�
	tPict.position_ = p2Pos;
	Image::SetTransform(hPlayer2_, tPict);
	Image::Draw(hPlayer2_);

	//�����A�s�k�A�����������S�̐ݒ�
	if (isClear_[0] && !isClear_[1])
	{
		tPict.position_ = XMFLOAT3(p1Pos.x, wPos.y, wPos.z);
		Image::SetTransform(hWin_, tPict);
		tPict.position_ = XMFLOAT3(p2Pos.x, lPos.y, lPos.z);
		Image::SetTransform(hLose_, tPict);
		Image::Draw(hWin_);
		Image::Draw(hLose_);
	}
	else if (!isClear_[0] && isClear_[1])
	{
		tPict.position_ = XMFLOAT3(p2Pos.x, wPos.y, wPos.z);
		Image::SetTransform(hWin_, tPict);
		tPict.position_ = XMFLOAT3(p1Pos.x, lPos.y, lPos.z);
		Image::SetTransform(hLose_, tPict);
		Image::Draw(hWin_);
		Image::Draw(hLose_);
	}
	else
	{
		tPict.position_ = dPos;
		Image::SetTransform(hDraw_, tPict);
		Image::Draw(hDraw_);
	}
}

void ResultScene::ResultRead()
{
	for (int i = 0; i < PLAYERNUMMAX; i++)
	{
		std::ifstream read;
		std::string openfile = "SaveFile\\result";
		openfile += std::to_string(i);

		read.open(openfile, std::ios::in);
		//  �t�@�C�����J��
		//  ios::in �͓ǂݍ��ݐ�p  ios::binary �̓o�C�i���`��

		if (!read) {
			std::cout << "�t�@�C�����J���܂���";
			return;
		}
		//  �t�@�C�����J���Ȃ������Ƃ��̑΍�
		bool isClear;
		read.read((char*)&isClear, sizeof(isClear));
		isClear_[i] = isClear;
		//������ł͂Ȃ��f�[�^��ǂ݂���
		read.close();  //�t�@�C�������

	}
}

