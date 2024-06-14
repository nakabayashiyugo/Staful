#include "SceneTransition.h"

#include "MapEditScene.h"
#include "PlayScene.h"
#include "BetweenScene.h"
#include "ResultScene.h"

#include "../GamePlayer.h"

#include "../Engine/SceneManager.h"
#include "../Engine/Image.h"
#include "../Engine/Input.h"
#include "../Engine/Audio.h"

SceneTransition::SceneTransition(GameObject* parent)
	: GameObject(parent, "SceneTransition"), 
	sceneState_(SCENE_MAPEDIT1), 
	prevSceneState_(SCENE_TURNEND),
	isClear_Player_{ false, false },
	hPlayer1_(-1), hPlayer2_(-1),
	hMapEdit_(-1), hChallenge_(-1)
{

	hPlayer1_ = Image::Load("Assets\\Logo_Player1.png");
	assert(hPlayer1_ >= 0);
	hPlayer2_ = Image::Load("Assets\\Logo_Player2.png");
	assert(hPlayer2_ >= 0);

	for (playerNum_ = 0; playerNum_ < PLAYERNUMMAX; playerNum_++)
	{
		gPlayer_[playerNum_]->Instantiate<GamePlayer>(this);
		std::string str = "GamePlayer" + std::to_string(playerNum_);
		gPlayer_[playerNum_] = (GamePlayer*)FindChildObject(str);
	}
	gPlayer_[0]->SetPlayerPictNum(hPlayer1_);
	gPlayer_[1]->SetPlayerPictNum(hPlayer2_);
}

void SceneTransition::Initialize()
{
	sceneState_ = SCENESTATE(0);

	//音楽ロード
	//音楽が入ってるフォルダ名
	const std::string folderName = "Music\\Assets\\Audio\\";
	//音楽のファイル名
	const std::string fileNameBase = "Audio_";
	std::string mapEditName = folderName + fileNameBase + "MapEdit.wav";
	std::string challengeName = folderName + fileNameBase + "Challenge.wav";
	
	hMapEdit_ = Audio::Load(mapEditName, true);
	assert(hMapEdit_ >= 0);
	hChallenge_ = Audio::Load(challengeName, true);
	assert(hChallenge_ >= 0);
}

void SceneTransition::Update()
{
	Audio::Play(hChallenge_, 0.5f);
	if (sceneState_ != prevSceneState_)
	{
		switch (sceneState_)
		{
		case SCENE_MAPEDIT1:
			turnNum_++;
		case SCENE_MAPEDIT2:
		case SCENE_STAGE1:
		case SCENE_STAGE2:
			Instantiate<BetweenScene>(this);
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

void SceneTransition::CallMapEdit(int _playerNum)
{
	playerNum_ = _playerNum;
	gPlayer_[playerNum_]->MapEdit();
}

void SceneTransition::CallChallenge(int _playerNum)
{
	playerNum_ = _playerNum;
	gPlayer_[playerNum_]->Challenge();
}
