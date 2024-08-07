#include "GamePlayer.h"

#include "Engine/Image.h"

#include "Scene/MapEditScene/MapEditScene.h"
#include "Scene/EnemyMapEditScene/EnemyMapEditScene.h"
#include "Scene/PlayScene.h"
#include "Scene/SceneTransition.h"

const int saveFileName1 = 1, saveFileName2 = 2;

GamePlayer::GamePlayer(GameObject* parent)
	:GameObject(parent, "GamePlayer"),
	isEnemy_(false),
	isClear_(false)
{
	pST_ = (SceneTransition*)this->pParent_;
	playerNum_ = pST_->GetPlayerNum();
	this->objectName_ += std::to_string(playerNum_);

	pMES_ = nullptr;
	pPS_ = nullptr;
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
	//読み込むファイル番号
	if (pST_->GetTurnNum() % 2 != 0 && playerNum_ == 0)	saveNum_ = saveFileName2;
	else if (pST_->GetTurnNum() % 2 != 0 && playerNum_ == 1) saveNum_ = saveFileName1;
	else if (pST_->GetTurnNum() % 2 != 1 && playerNum_ == 0)	saveNum_ = saveFileName1;
	else saveNum_ = saveFileName2;
	if (isEnemy_)
	{
		pMES_->Instantiate<EnemyMapEditScene>(this);
		pMES_ = (EnemyMapEditScene*)FindObject("EnemyMapEditScene");
	}
	else
	{
		pMES_->Instantiate<MapEditScene>(this);
		pMES_ = (MapEditScene*)FindObject("MapEditScene");
	}
}

void GamePlayer::Challenge()
{
	//読み込むファイル番号入れ替え
	if (saveNum_ == saveFileName1)	saveNum_ = saveFileName2;
	else if (saveNum_ == saveFileName2)	saveNum_ = saveFileName1;
	pPS_->Instantiate<PlayScene>(this);
	pPS_ = (PlayScene*)FindObject("PlayScene");
}

void GamePlayer::MapEditFinished()
{
	pST_->SetNextScene();
	pMES_->KillMe();
}

void GamePlayer::ChallengeFinished()
{
	pST_->SetNextScene();
	pPS_->KillMe();
	ResultWrite(isClear_);
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

void GamePlayer::ResultWrite(bool _isClear)
{
	std::ofstream write;
	std::string savefile = "SaveFile\\result";
	savefile += std::to_string(playerNum_);

	write.open(savefile, std::ios::out);

	//  ファイルが開けなかったときのエラー表示
	if (!write) {
		std::cout << "ファイル " << savefile << " が開けません";
		return;
	}

	write.write((char*)&_isClear, sizeof(_isClear));
	//文字列ではないデータをかきこむ

	write.close();  //ファイルを閉じる
}
