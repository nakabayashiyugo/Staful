#include "ThankyouScene.h"
#include "SceneTransition.h"

#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Direct3D.h"
#include "Engine/SceneManager.h"

ThankyouScene::ThankyouScene(GameObject* parent)
	: GameObject(parent, "ThankyouScene"), hReturnTitle_(-1), hThankyou_(-1),
	mousePos_(0, 0, 0)
{
}

void ThankyouScene::Initialize()
{
	hReturnTitle_ = Image::Load("Assets\\Logo_ReturnTitle.png");
	assert(hReturnTitle_ >= 0);
	hThankyou_ = Image::Load("Assets\\Logo_Thankyou.png");
	assert(hThankyou_ >= 0);
}

void ThankyouScene::Update()
{
	mousePos_ = Input::GetMousePosition();

	mousePos_.x = mousePos_.x - (Direct3D::scrWidth / 2);
	mousePos_.y = mousePos_.y - (Direct3D::scrHeight / 2);

	float RTRight = tReturnTitle_.position_.x * (Direct3D::scrWidth / 2) + (Direct3D::scrWidth * tReturnTitle_.scale_.x / 2);

	float RTLeft = tReturnTitle_.position_.x * (Direct3D::scrWidth / 2) - (Direct3D::scrWidth * tReturnTitle_.scale_.x / 2);

	float RTUp = (tReturnTitle_.position_.y * (Direct3D::scrHeight / 2) + (Direct3D::scrHeight * tReturnTitle_.scale_.y / 2)) * -1;

	float RTDown = (tReturnTitle_.position_.y * (Direct3D::scrHeight / 2) - (Direct3D::scrHeight * tReturnTitle_.scale_.y / 2)) * -1;

	if (mousePos_.x >= RTLeft && mousePos_.x <= RTRight &&
		mousePos_.y >= RTUp && mousePos_.y <= RTDown)
	{
		if (Input::IsMuoseButtonUp(0))
		{
			SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
			pSceneManager->ChangeScene(SCENE_ID_TITLE);
		}
	}
	else
	{
		std::string resStr = std::to_string((float)mousePos_.x) + ", " + std::to_string(mousePos_.y) + "\n";
		OutputDebugString(resStr.c_str());
	}
}

void ThankyouScene::Draw()
{
	tReturnTitle_.position_.y = -0.1f;
	tReturnTitle_.scale_ = XMFLOAT3(0.5f, 0.5f, 1);
	tThankyou_.position_.y = 0.5f;
	tThankyou_.scale_ = XMFLOAT3(2, 2, 1);

	Image::SetTransform(hReturnTitle_, tReturnTitle_);
	Image::SetTransform(hThankyou_, tThankyou_);
	Image::Draw(hReturnTitle_);
	Image::Draw(hThankyou_);

	//Model::Draw(hDice_);
}

void ThankyouScene::Release()
{
}