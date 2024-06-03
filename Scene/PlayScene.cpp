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
	//テストプレイの場合
	if (pParent_->GetObjectName() == "MapEditScene")
	{
		MapEditScene* pMES = (MapEditScene*)FindObject("MapEditScene");
		if (pPlayer_->GetStageState() == STATE_GOAL)
		{
			pMES->SetIsClear(true);
			//マップエディター表示
			pMES->SetIsDisp(true);
			KillMe();
		}
		if (pPlayer_->GetStageState() == STATE_FAILURE)
		{
			//マップエディター表示
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
	//  ファイルを開く
	//  ios::in は読み込み専用  ios::binary はバイナリ形式

	if (!read) {
		std::cout << "ファイルが開けません";
		return;
	}
	//  ファイルが開けなかったときの対策

	//ファイルの最後まで続ける
	for (int i = 0; i < XSIZE; i++)
	{
		for (int j = 0; j < ZSIZE; j++)
		{
			read.read((char*)&math_[i][j], sizeof(math_[i][j]));
			//文字列ではないデータを読みこむ

		}
	}
	read.close();  //ファイルを閉じる

	//とげとげルート
	openfile = saveFolderName + "tgtgRoute";
	openfile += std::to_string(saveNum_);
	read.open(openfile, std::ios::in);
	if (!read) {
		std::cout << "ファイルが開けません";
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
	read.close();  //ファイルを閉じる
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
	//返すMATHDEDAILの初期値
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