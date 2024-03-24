#include "Button.h"
#include "SceneTransition.h"

#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Direct3D.h"
#include "Engine/SceneManager.h"

Button::Button(GameObject* parent)
	: GameObject(parent, "Button"), isClick_(false),isRelease_(false),
	hPict_(-1)
{
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

	float SBRight = tPict_.position_.x * (Direct3D::scrWidth / 2) + (startButtonSize.x * tPict_.scale_.x / 2);

	float SBLeft = tPict_.position_.x * (Direct3D::scrWidth / 2) - (startButtonSize.x * tPict_.scale_.x / 2);

	float SBUp = (tPict_.position_.y * (Direct3D::scrHeight / 2) + (startButtonSize.y * tPict_.scale_.y / 2)) * -1;

	float SBDown = (tPict_.position_.y * (Direct3D::scrHeight / 2) - (startButtonSize.y * tPict_.scale_.y / 2)) * -1;

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
	const XMFLOAT3 clickedColor = XMFLOAT3(0.5f, 0.5f, 0.5f);
	const XMFLOAT3 color = XMFLOAT3(1, 1, 1);
	Image::SetColor(hPict_, color);
	if (isClick_)
	{
		Image::SetColor(hPict_, clickedColor);
	}

	Image::SetTransform(hPict_, tPict_);
	Image::Draw(hPict_);
}

void Button::Release()
{
}