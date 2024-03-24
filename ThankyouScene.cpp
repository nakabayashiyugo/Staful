#include "ThankyouScene.h"
#include "SceneTransition.h"
#include "Button.h"

#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Direct3D.h"
#include "Engine/SceneManager.h"

ThankyouScene::ThankyouScene(GameObject* parent)
	: GameObject(parent, "ThankyouScene"), hReturnTitle_(-1), hThankyou_(-1),
	mousePos_(0, 0, 0)
{
	pReturnButton_ = (Button*)FindObject("Button");
	pReturnButton_->Instantiate<Button>(this);
}

void ThankyouScene::Initialize()
{
	hReturnTitle_ = Image::Load("Assets\\Logo_ReturnTitle.png");
	assert(hReturnTitle_ >= 0);
	hThankyou_ = Image::Load("Assets\\Logo_Thankyou.png");
	assert(hThankyou_ >= 0);

	pReturnButton_ = (Button*)FindObject("Button");
	pReturnButton_->SetPictNum(hReturnTitle_);
}

void ThankyouScene::Update()
{
	if (pReturnButton_->GetIsReleased())
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TITLE);
	}
}

void ThankyouScene::Draw()
{
	tReturnTitle_.position_.y = -0.1f;
	tReturnTitle_.scale_ = XMFLOAT3(0.5f, 0.5f, 1);
	pReturnButton_->SetTransform(tReturnTitle_);


	tThankyou_.position_.y = 0.5f;
	tThankyou_.scale_ = XMFLOAT3(2, 2, 1);
	Image::SetTransform(hThankyou_, tThankyou_);
	Image::Draw(hThankyou_);

	//Model::Draw(hDice_);
}

void ThankyouScene::Release()
{
}