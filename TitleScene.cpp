#include "TitleScene.h"
#include "SceneTransition.h"
#include "Button.h"

#include "Engine/Image.h"
#include "Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Direct3D.h"
#include "Engine/SceneManager.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hStartButton_(-1), hStaful_(-1)
{
	pStartButton_ = (Button*)FindObject("Button");
	pStartButton_->Instantiate<Button>(this);
	
}

void TitleScene::Initialize()
{
	hStartButton_ = Image::Load("Assets\\Logo_StartButton.png");
	assert(hStartButton_ >= 0);
	hStaful_ = Image::Load("Assets\\Logo_Staful.png");
	assert(hStaful_ >= 0);

	pStartButton_ = (Button*)FindObject("Button");
	pStartButton_->SetPictNum(hStartButton_);
}

void TitleScene::Update()
{
	if (pStartButton_->GetIsReleased())
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TRANSITION);
	}
			
}

void TitleScene::Draw()
{
	tStartButton_.position_.y = -0.1f;
	//tStartButton_.scale_ = XMFLOAT3(0.5f, 0.5f, 1);
	pStartButton_->SetTransform(tStartButton_);

	tStaful_.position_.y = 0.5f;
	tStaful_.scale_ = XMFLOAT3(2, 2, 1);

	Image::SetTransform(hStaful_, tStaful_);
	Image::Draw(hStaful_);
}

void TitleScene::Release()
{
}