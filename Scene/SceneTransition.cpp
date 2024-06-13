#include "SceneTransition.h"

#include "MapEditScene.h"
#include "PlayScene.h"
#include "BetweenScene.h"
#include "ResultScene.h"

#include "../GamePlayer.h"

#include "../Engine/SceneManager.h"
#include "../Engine/Image.h"
#include "../Engine/Input.h"

SceneTransition::SceneTransition(GameObject* parent)
	: GameObject(parent, "SceneTransition"), 
	sceneState_(SCENE_MAPEDIT1), 
	prevSceneState_(SCENE_TURNEND),
	isClear_Player_{ false, false },
	hPlayer1_(-1), hPlayer2_(-1)
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
}

void SceneTransition::Update()
{
	if (sceneState_ != prevSceneState_)
	{
		switch (sceneState_)
		{
		case SCENE_MAPEDIT1:
			turnNum_++;
			gPlayer_[0]->MapEdit();
			break;
		case SCENE_MAPEDIT2:
			gPlayer_[1]->MapEdit();
			break;
		case SCENE_STAGE1:
			gPlayer_[0]->Challenge();
			break;
		case SCENE_STAGE2:
			gPlayer_[1]->Challenge();
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
	gPlayer_[_playerNum]->MapEdit();
}

void SceneTransition::CallChallenge(int _playerNum)
{
	gPlayer_[_playerNum]->Challenge();
}
