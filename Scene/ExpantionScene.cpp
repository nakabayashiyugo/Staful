#include "ExpantionScene.h"
#include "SceneTransition.h"

#include "../Button.h"

#include "../Engine/Image.h"
#include "../Engine/Input.h"


ExpantionScene::ExpantionScene(GameObject* parent)
	: GameObject(parent, "ExpantionScene"), hNext_(-1), hExplanNextButton_(-1), hExplanReturn_(-1)
{
	for (int i = 0; i < explanationNum; i++)
	{
		hExplan_[i] = -1;
	}

}

void ExpantionScene::Initialize()
{
}

void ExpantionScene::Update()
{
}

void ExpantionScene::Draw()
{
}

void ExpantionScene::Release()
{
}