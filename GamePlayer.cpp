#include "GamePlayer.h"

#include "Engine/Image.h"

#include "Scene/MapEditScene.h"
#include "Scene/PlayScene.h"
#include "Scene/SceneTransition.h"

const int saveFileName1 = 1, saveFileName2 = 2;

GamePlayer::GamePlayer(GameObject* parent)
	:GameObject(parent, "GamePlayer")
{
	SceneTransition* pST = (SceneTransition*)this->pParent_;
	playerNum_ = pST->GetPlayerNum();
	this->objectName_ += std::to_string(playerNum_);


	//“Ç‚Ýž‚Þƒtƒ@ƒCƒ‹”Ô†
	if (pST->GetTurnNum() % 2 != 0 && playerNum_ == 0)	saveNum_ = saveFileName2;
	else if (pST->GetTurnNum() % 2 != 0 && playerNum_ == 1) saveNum_ = saveFileName1;
	else if (pST->GetTurnNum() % 2 != 1 && playerNum_ == 0)	saveNum_ = saveFileName2;
	else saveNum_ = saveFileName1;
}

GamePlayer::~GamePlayer()
{
}

void GamePlayer::Initialize()
{
}

void GamePlayer::Update()
{
}

void GamePlayer::Draw()
{
}

void GamePlayer::Release()
{
}

void GamePlayer::MapEdit()
{
	pMES_->Instantiate<MapEditScene>(this);
	pMES_ = (MapEditScene*)FindObject("MapEditScene");
}

void GamePlayer::Challenge()
{
	pPS_->Instantiate<PlayScene>(this);
	pPS_ = (PlayScene*)FindObject("PlayScene");
}

int GamePlayer::GetTurnNum()
{
	SceneTransition* pST = (SceneTransition*)FindObject("SceneTransition");
	return pST->GetTurnNum();
}

void GamePlayer::PlayerNumDraw()
{
	const XMFLOAT3 playerLogoPos = XMFLOAT3(0.7, 0.8, 0);
	Transform player;
	player.position_ = playerLogoPos;

	Image::SetTransform(hPlayer_, player);
	Image::Draw(hPlayer_);
}
