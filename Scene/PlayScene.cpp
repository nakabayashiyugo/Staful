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
#include "../GamePlayer.h"

PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"), 
	table_Change_(false), 
	floorHeight_(0), 
	playerHeight_(floorHeight_ + MODELHEIGHT)
{
	MathVolumeRead();
	if (pParent_->GetObjectName() == "MapEditScene")
	{
		pGP_ = (GamePlayer*)this->pParent_->GetParent();
	}
	else
	{
		pGP_ = (GamePlayer*)this->pParent_;
	}
	playerNum_ = pGP_->GetPlayerNum();
	Math_Resize(mathVolume_.x, mathVolume_.z, &math_);
	saveNum_ = pGP_->GetSaveNum();
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
	else
	{
		if (pPlayer_->GetStageState() == STATE_GOAL)
		{
			pGP_->SetIsClear(true);
			pGP_->ChallengeFinished();
			KillMe();
		}
		if (pPlayer_->GetStageState() == STATE_FAILURE)
		{
			pGP_->SetIsClear(false);
			pGP_->ChallengeFinished();
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

void PlayScene::PlayerNumDraw()
{
	//プレイヤー番号表示
	pGP_->PlayerNumDraw();
}

void PlayScene::CallWrite()
{
	this->Write();
}

void PlayScene::CallRead()
{
	this->Read();
}
