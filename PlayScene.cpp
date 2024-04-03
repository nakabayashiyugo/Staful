#include "PlayScene.h"

#include "Engine/Input.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"

#include "SceneTransition.h"
#include "MapEditScene.h"
#include "Timer.h"
#include "Stage.h"
#include "Player.h"

PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), table_Change_(false), saveNum_(2)
{
	pTrans_ = (SceneTransition*)FindObject("SceneTransition");
	XSIZE = (int)pTrans_->GetMathSize_x();
	ZSIZE = (int)pTrans_->GetMathSize_z();

	player_Num_ = pTrans_->GetPlayerNum();
	Math_Resize(XSIZE, ZSIZE, &math_);
	
	saveNum_ = pTrans_->GetSaveNum();

	Read();
}

void PlayScene::Initialize()
{
	pPlayer_->Instantiate<Player>(this);
	pStage_->Instantiate<Stage>(this);
	pStage_ = (Stage*)FindObject("Stage");
	pPlayer_ = (Player*)FindObject("Player");
}

void PlayScene::Update()
{
	if (pParent_->GetObjectName() == "SceneTransition")
	{
		if (pPlayer_->GetStageState() == STATE_GOAL)
		{
			pTrans_->SetNextScene();
			pTrans_->SetIsClear(player_Num_, true);
			KillMe();
		}
		if (pPlayer_->GetStageState() == STATE_FAILURE)
		{
			pTrans_->SetNextScene();
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
			pMES->IsHidden(false);
			KillMe();
		}
		if (pPlayer_->GetStageState() == STATE_FAILURE)
		{
			//�}�b�v�G�f�B�^�[�\��
			pMES->IsHidden(false);
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
	pPlayer_ = (Player*)FindObject("Player");
	return pPlayer_->GetPosition();
}
