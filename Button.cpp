#include "Button.h"
#include "Scene/SceneTransition.h"
#include "Scene/MapEditScene.h"
#include "Scene/ExpantionScene.h"

#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Direct3D.h"
#include "Engine/SceneManager.h"
#include "Engine/Audio.h"

Button::Button(GameObject* parent)
	: GameObject(parent, "Button"), isClick_(false), isRelease_(false),
	hPict_(-1), isCanPush_(true), isCursorMatched_(false), isDisp_(true),
	hSE_(-1)
{
	//�e�N���X��MapEditScene�Ȃ�
	if (pParent_->GetObjectName() == "MapEditScene")
	{
		//���g�̃I�u�W�F�N�g�l�[���ɔԍ���t����
		MapEditScene* pMES = (MapEditScene*)FindObject("MapEditScene");
		this->objectName_ += std::to_string(pMES->GetButtonNum());
	}
	if (pParent_->GetObjectName() == "ExpantionScene")
	{
		ExpantionScene* pES = (ExpantionScene*)FindObject("ExpantionScene");
		this->objectName_ += std::to_string(pES->GetButtonNum());
	}
}

Button::Button(std::string _btnName, GameObject* parent)
	: GameObject(parent, "Button"), isClick_(false), isRelease_(false),
	hPict_(-1), isCanPush_(true), isCursorMatched_(false), isDisp_(true),
	hSE_(-1)
{
	this->objectName_ = _btnName;
}

void Button::Initialize()
{
	//���y�������Ă�t�H���_��
	const std::string folderName = "Assets\\Audio\\";
	//SE
	const std::string SEFolder = "SE\\";
	//SE���[�h
	std::string SEButton = folderName + SEFolder + "SE_PushButton.wav";

	hSE_ = Audio::Load(SEButton, false);
}

void Button::Update()
{
	if (isCanPush_)
	{
		mousePos_ = Input::GetMousePosition();

		mousePos_.x = mousePos_.x - (Direct3D::bfr_scrWidth / 2);
		mousePos_.y = mousePos_.y - (Direct3D::bfr_scrHeight / 2);

		std::string resStr = std::to_string((float)mousePos_.x) + ", " + std::to_string(mousePos_.y) + "\n";
		OutputDebugString(resStr.c_str());

		XMFLOAT3 startButtonSize = Image::GetTextureSize(hPict_);

		//�{�^���̈ʒu��y��0.0f�̂Ƃ��̃J�[�\���Ƃ̌덷
		float cursorErrorInit = 9.0f;
		//�{�^���̈ʒu��y�ƃJ�[�\���Ƃ̌덷
		float cursorError = cursorErrorInit + (-tPict_.position_.y * 10);

		//�{�^���̉E�̍��W
		float SBRight = tPict_.position_.x * (Direct3D::bfr_scrWidth / 2) + (startButtonSize.x * tPict_.scale_.x / 2);
		//�{�^���̍��̍��W
		float SBLeft = tPict_.position_.x * (Direct3D::bfr_scrWidth / 2) - (startButtonSize.x * tPict_.scale_.x / 2);
		//�{�^���̏�̍��W
		float SBUp = -((tPict_.position_.y * (Direct3D::bfr_scrHeight / 2) + (startButtonSize.y * tPict_.scale_.y / 2))) + cursorError;
		//�{�^���̉��̍��W
		float SBDown = SBUp + (startButtonSize.y * tPict_.scale_.y);

		if (mousePos_.x >= SBLeft && mousePos_.x <= SBRight &&
			mousePos_.y >= SBUp && mousePos_.y <= SBDown)
		{
			isCursorMatched_ = true;
			if (Input::IsMouseButtonDown(0))
			{
				Audio::Play(hSE_, 0.5f);
				isClick_ = true;
			}
			if (Input::IsMuoseButtonUp(0))
			{
				Audio::Stop(hSE_);
				isClick_ = false;
				isRelease_ = true;
			}
		}
		else
		{
			isCursorMatched_ = false;
			isClick_ = false;
		}
	}
	else
	{
		isCursorMatched_ = false;
		isClick_ = false;
	}
}

void Button::Draw()
{
	if (isDisp_)
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
