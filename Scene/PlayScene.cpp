#include "PlayScene.h"

#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Image.h"

#include "SceneTransition.h"
#include "MapEditScene.h"

#include "../Timer.h"
#include "../Stage.h"
#include "../Player.h"
#include "../Button.h"

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
	
	cancelButton_->Instantiate<Button>(this);
	cancelButton_ = (Button*)FindObject("Button");
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