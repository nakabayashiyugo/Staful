#include "PlayScene.h"

#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Image.h"

#include "SceneTransition.h"
#include "MapEditScene.h"

#include "../Timer.h"
#include "../Stage.h"
#include "../Player.h"

PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), table_Change_(false), saveNum_(2),
	floorHeight_(0), playerHeight_(floorHeight_ + MODELHEIGHT)
{
	pST = (SceneTransition*)FindObject("SceneTransition");
	XSIZE = (int)pST->GetMathSize_x();
	ZSIZE = (int)pST->GetMathSize_z();

	player_Num_ = pST->GetPlayerNum();
	Math_Resize(XSIZE, ZSIZE, &math_);
	
	saveNum_ = pST->GetSaveNum();

	Read();
}

void PlayScene::Initialize()
{
	pStage_->Instantiate<Stage>(this);
	pPlayer_->Instantiate<Player>(this);
	pPlayer_ = (Player*)FindObject("Player");
	pStage_ = (Stage*)FindObject("Stage");
	
}

void PlayScene::Update()
{
	if (pParent_->GetObjectName() == "SceneTransition")
	{
		if (pPlayer_->GetStageState() == STATE_GOAL)
		{
			pST->SetNextScene();
			pST->SetIsClear(player_Num_, true);
			KillMe();
		}
		if (pPlayer_->GetStageState() == STATE_FAILURE)
		{
			pST->SetNextScene();
			pST->SetIsClear(player_Num_, false);
			KillMe();
		}
	}
	//�e�X�g�v���C�̏ꍇ
	if (pParent_->GetObjectName() == "MapEditScene")
	{
		MapEditScene* pMES = (MapEditScene*)FindObject("MapEditScene");
		if (pPlayer_->GetStageState() == STATE_GOAL)
		{
			pMES->SetIsClear(true);
			//�}�b�v�G�f�B�^�[�\��
			pMES->SetIsDisp(true);
			KillMe();
		}
		if (pPlayer_->GetStageState() == STATE_FAILURE)
		{
			//�}�b�v�G�f�B�^�[�\��
			pMES->SetIsDisp(true);
			KillMe();
		}
	}
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}

void PlayScene::Read()
{
	std::ifstream read;
	std::string openfile = saveFolderName + "saveMath";

	openfile += std::to_string(saveNum_);
	read.open(openfile, std::ios::in);
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
		for (int j = 0; j < ZSIZE; j++)
		{
			read.read((char*)&math_[i][j], sizeof(math_[i][j]));
			//������ł͂Ȃ��f�[�^��ǂ݂���

		}
	}
	read.close();  //�t�@�C�������

	//�Ƃ��Ƃ����[�g
	openfile = saveFolderName + "tgtgRoute";
	openfile += std::to_string(saveNum_);
	read.open(openfile, std::ios::in);
	if (!read) {
		std::cout << "�t�@�C�����J���܂���";
		return;
	}
	
	while (!read.eof())
	{
		TOGETOGEROUTE pTg;
		read.read((char*)&pTg, sizeof(pTg));
		if (pTg.route_.scale_.x < 1)
		{
			tTgtgRoute_.push_back(pTg);
		}
	}
	read.close();  //�t�@�C�������
}

XMFLOAT3 PlayScene::GetPlayerPos()
{
	XMFLOAT3 retMath = XMFLOAT3(0, 0, 0);
	if (pPlayer_ == nullptr)
	{
		return retMath;
	}
	retMath = pPlayer_->GetPosition();
	return retMath;
}

MATHDEDAIL PlayScene::GetPlayerStandMath()
{
	MATHDEDAIL retMath;
	//�Ԃ�MATHDEDAIL�̏����l
	MATHTYPE retMathInit = MATH_FLOOR;
	Transform retTransformInit;
	retMath = { retMathInit, retTransformInit };
	if (pPlayer_ == nullptr || pPlayer_->GetPlayerState() != STATE_IDLE)
	{
		return retMath;
	}
	retMath = pPlayer_->GetStandMath();
	return retMath;
}