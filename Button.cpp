#include "Button.h"
#include "SceneTransition.h"
#include "MapEditScene.h"

#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Direct3D.h"
#include "Engine/SceneManager.h"

Button::Button(GameObject* parent)
	: GameObject(parent, "Button"), isClick_(false),isRelease_(false),
	hPict_(-1)
{
	//親クラスがMapEditSceneなら
	if (pParent_->GetObjectName() == "MapEditScene")
	{
		//自身のオブジェクトネームに番号を付ける
		MapEditScene* pMES = (MapEditScene*)FindObject("MapEditScene");
		this->objectName_ += std::to_string(pMES->GetButtonNum());
	}
}

void Button::Initialize()
{
}

void Button::Update()
{
	mousePos_ = Input::GetMousePosition();


	mousePos_.x = mousePos_.x - (Direct3D::scrWidth / 2);
	mousePos_.y = mousePos_.y - (Direct3D::scrHeight / 2);

	XMFLOAT3 startButtonSize = Image::GetTextureSize(hPict_);

	//ボタンの右の座標
	float SBRight = tPict_.position_.x * (Direct3D::scrWidth / 2) + (startButtonSize.x * tPict_.scale_.x / 2);
	//ボタンの左の座標
	float SBLeft = tPict_.position_.x * (Direct3D::scrWidth / 2) - (startButtonSize.x * tPict_.scale_.x / 2);
	//ボタンの上の座標
	float SBUp = -((tPict_.position_.y * (Direct3D::scrHeight / 2) + (startButtonSize.y * tPict_.scale_.y / 2)));
	//ボタンの下の座標
	float SBDown = -((tPict_.position_.y * (Direct3D::scrHeight / 2) - (startButtonSize.y * tPict_.scale_.y / 2)));

	if (mousePos_.x >= SBLeft && mousePos_.x <= SBRight &&
		mousePos_.y >= SBUp && mousePos_.y <= SBDown)
	{
		if (Input::IsMouseButtonDown(0))
		{
			isClick_ = true;
		}
		if (Input::IsMuoseButtonUp(0))
		{
			isClick_ = false;
			isRelease_ = true;
		}
	}
	else
	{
		std::string resStr = std::to_string((float)mousePos_.x) + ", " + std::to_string(mousePos_.y) + "\n";
		OutputDebugString(resStr.c_str());
	}
}

void Button::Draw()
{
	//ボタンをクリックしたときの色
	const XMFLOAT3 clickedColor = XMFLOAT3(0.5f, 0.5f, 0.5f);
	//ボタンをクリックしてない時の色
	const XMFLOAT3 color = XMFLOAT3(1, 1, 1);

	XMFLOAT3 buttonColor;
	buttonColor = color;
	if (isClick_)
	{
		buttonColor = clickedColor;
	}
	Image::SetColor(hPict_, buttonColor);
	Image::SetTransform(hPict_, tPict_);
	Image::Draw(hPict_);
}

void Button::Release()
{
}

void Button::SetPictNum(int _pict)
{
	this->hPict_ = _pict;
}

void Button::SetAlpha(int _alpha)
{
	Image::SetAlpha(hPict_, _alpha);
}
