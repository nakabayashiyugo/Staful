#include "SceneManager.h"
#include "../Scene/ResultScene.h"
#include "../Scene/TitleScene.h"
#include "../Scene/PlayScene.h"
#include "../Scene/MapEditScene/MapEditScene.h"
#include "../Scene/SceneTransition.h"
#include "../Scene/ExpantionScene.h"
#include "../Scene/MapEditScene/MapEditScene.h"

#include "Model.h"

SceneManager::SceneManager(GameObject* parent)
	: GameObject(parent, "SceneManager")
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Initialize()
{
	currentSceneID_ = SCENE_ID_TITLE;
	nextSceneID_ = currentSceneID_;
	Instantiate<TitleScene>(this);
}

void SceneManager::Update()
{
	if (currentSceneID_ != nextSceneID_)
	{
		//そのシーンのオブジェクトを全削除
		auto scene = childList_.begin();
		(*scene)->ReleaseSub();
		SAFE_DELETE(*scene);
		childList_.clear();

		Model::Release();
		//後片付け終了

		switch (nextSceneID_)
		{
		case SCENE_ID_PLAY:
			Instantiate<PlayScene>(this);
			break;
		case SCENE_ID_EDIT:
			Instantiate<MapEditScene>(this);
			break;
		case SCENE_ID_TRANSITION:
			Instantiate<SceneTransition>(this);
			break;
		case SCENE_ID_TITLE:
			Instantiate<TitleScene>(this);
			break;
		case SCENE_ID_EXPLANATION:
			Instantiate<ExpantionScene>(this);
			break;
		case SCENE_ID_RESULT:
			Instantiate<ResultScene>(this);
			break;
		}
		currentSceneID_ = nextSceneID_;
	}
}

void SceneManager::Draw()
{
}

void SceneManager::Release()
{
}

void SceneManager::ChangeScene(SCENE_ID _next)
{
	nextSceneID_ = _next;
}
