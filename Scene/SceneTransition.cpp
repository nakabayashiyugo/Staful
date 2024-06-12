#include "SceneTransition.h"
#include "MapEditScene.h"
#include "PlayScene.h"
#include "BetweenScene.h"
#include "ResultScene.h"

#include "../Engine/SceneManager.h"
#include "../Engine/Image.h"
#include "../Engine/Input.h"

const int saveFileName1 = 1, saveFileName2 = 2;

SceneTransition::SceneTransition(GameObject* parent)
	: GameObject(parent, "SceneTransition"), 
	sceneState_(SCENE_MAPEDIT1), prevSceneState_(SCENE_TURNEND),
	turnNum_(0), player_Num_(0), saveNum_(saveFileName1),
	isClear_Player_{ false, false },
	hPlayer1_(-1), hPlayer2_(-1)
{
}

void SceneTransition::Initialize()
{
	sceneState_ = SCENESTATE(0);

	hPlayer1_ = Image::Load("Assets\\Logo_Player1.png");
	assert(hPlayer1_ >= 0);
	hPlayer2_ = Image::Load("Assets\\Logo_Player2.png");
	assert(hPlayer2_ >= 0);
}

void SceneTransition::Update()
{
	SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
	if (sceneState_ != prevSceneState_)
	{
		switch (sceneState_)
		{
		case SCENE_MAPEDIT1:
			turnNum_++;
			if (turnNum_ % 2 == 0)	saveNum_ = saveFileName2;
			else saveNum_ = saveFileName1;
			Instantiate<MapEditScene>(this); 
			break;
		case SCENE_MAPEDIT2:
			if (turnNum_ % 2 == 0)	saveNum_ = saveFileName1;
			else saveNum_ = saveFileName2;
			Instantiate<MapEditScene>(this); 
			break;
		case SCENE_STAGE1:
			if (turnNum_ % 2 == 0)	saveNum_ = saveFileName1;
			else saveNum_ = saveFileName2;
			player_Num_ = 0;
			pPS_[player_Num_]->Instantiate<PlayScene>(this);
			pPS_[player_Num_] = (PlayScene*)FindObject("PlayScene");
			break;
		case SCENE_STAGE2:
			if (turnNum_ % 2 == 0)	saveNum_ = saveFileName2;
			else saveNum_ = saveFileName1;
			player_Num_ = 1;
			pPS_[player_Num_]->Instantiate<PlayScene>(this);
			pPS_[player_Num_] = (PlayScene*)FindObject("PlayScene");
			break;
		case SCENE_RESULT:
			Instantiate<ResultScene>(this);
			break;
		case SCENE_TURNEND:
			sceneState_ = SCENESTATE(0);
			return;
		default:
			break;
		}
	}
	prevSceneState_ = sceneState_;
}

void SceneTransition::Draw()
{
}

void SceneTransition::Release()
{
}

void SceneTransition::PlayerNumDraw()
{
	Transform player;
	player.position_ = XMFLOAT3(0.7, 0.8, 0);
	switch (sceneState_)
	{
	case SCENE_MAPEDIT1:
	case SCENE_STAGE1:
		Image::SetTransform(hPlayer1_, player);
		Image::Draw(hPlayer1_);
		break;
	case SCENE_MAPEDIT2:
	case SCENE_STAGE2:
		Image::SetTransform(hPlayer2_, player);
		Image::Draw(hPlayer2_);
		break;
	}
}

void SceneTransition::ResultWrite()
{
	//player１のリザルトを保存
	std::ofstream write;
	std::string savefile = saveFolderName + "saveResult";
	savefile += std::to_string(saveFileName1);

	write.open(savefile, std::ios::out);

	//  ファイルが開けなかったときのエラー表示
	if (!write) {
		std::cout << "ファイル " << savefile << " が開けません";
		return;
	}

	write.write((char*)&isClear_Player_[saveFileName1 - 1], sizeof(&isClear_Player_[saveFileName1 - 1]));
	write.close();  //ファイルを閉じる

	//player２のリザルトを保存
	savefile = saveFolderName + "saveResult";
	savefile += std::to_string(saveFileName2);
	write.open(savefile, std::ios::out);
	//  ファイルが開けなかったときのエラー表示
	if (!write) {
		std::cout << "ファイル " << savefile << " が開けません";
		return;
	}
	write.write((char*)&isClear_Player_[saveFileName2 - 1], sizeof(&isClear_Player_[saveFileName2 - 1]));
	write.close();  //ファイルを閉じる
}
