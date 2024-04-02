#include "Button.h"
#include "SceneTransition.h"
#include "MapEditScene.h"

#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Direct3D.h"
#include "Engine/SceneManager.h"

Button::Button(GameObject* parent)
	: GameObject(parent, "Button"), isClick_(false),isRelease_(false),
	hPict_(-1), isCanPush_(true), isCursorMatched_(false)
{
	//�e�N���X��MapEditScene�Ȃ�
	if (pParent_->GetObjectName() == "MapEditScene")
	{
		//���g�̃I�u�W�F�N�g�l�[���ɔԍ���t����
		MapEditScene* pMES = (MapEditScene*)FindObject("MapEditScene");
		this->objectName_ += std::to_string(pMES->GetButtonNum());
	}
}

void Button::Initialize()
{
}

void Button::Update()
{
	if (isCanPush_)
	{
		mousePos_ = Input::GetMousePosition();

		mousePos_.x = mousePos_.x - (Direct3D::scrWidth / 2);
		mousePos_.y = mousePos_.y - (Direct3D::scrHeight / 2);

		XMFLOAT3 startButtonSize = Image::GetTextureSize(hPict_);

		//�{�^���̉E�̍��W
		float SBRight = tPict_.position_.x * (Direct3D::scrWidth / 2) + (startButtonSize.x * tPict_.scale_.x / 2);
		//�{�^���̍��̍��W
		float SBLeft = tPict_.position_.x * (Direct3D::scrWidth / 2) - (startButtonSize.x * tPict_.scale_.x / 2);
		//�{�^���̏�̍��W
		float SBUp = -((tPict_.position_.y * (Direct3D::scrHeight / 2) + (startButtonSize.y * tPict_.scale_.y / 2)));
		//�{�^���̉��̍��W
		float SBDown = -((tPict_.position_.y * (Direct3D::scrHeight / 2) - (startButtonSize.y * tPict_.scale_.y / 2)));

		if (mousePos_.x >= SBLeft && mousePos_.x <= SBRight &&
			mousePos_.y >= SBUp && mousePos_.y <= SBDown)
		{
			isCursorMatched_ = true;
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
			isCursorMatched_ = false;
		}
	}
}

void Button::Draw()
{
	//�{�^�����N���b�N�����Ƃ��̐F
	const XMFLOAT3 clickedColor = XMFLOAT3(0.5f, 0.5f, 0.5f);
	//�{�^�����N���b�N���ĂȂ����̐F
	const XMFLOAT3 color = XMFLOAT3(1, 1, 1);
	//���̃{�^���������Ȃ����̐F
	const XMFLOAT3 notPush = XMFLOAT3(0.2f, 0.2f, 0.2f);
	//�{�^���ɃJ�[�\�������������̐F
	const XMFLOAT3 cursorMatchColor = XMFLOAT3(0.7f, 0.7f, 0.7f);

	XMFLOAT3 buttonColor;
	buttonColor = color;
	if (isCursorMatched_)
	{
		buttonColor = cursorMatchColor;
	}
	if (isClick_)
	{
		buttonColor = clickedColor;
	}
	if (!isCanPush_)
	{
		buttonColor = notPush;
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
