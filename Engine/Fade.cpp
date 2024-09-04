#include "Fade.h"

#include "SceneManager.h"
#include "Image.h"
#include "Direct3D.h"

namespace FADE
{
	Fade* fade_;
}

void FADE::FadeStart(GameObject* _parent, float _fadeSpeed, FADETYPE _fadeType)
{
	//���łɃt�F�[�h�����݂�����������Ȃ�
	if (fade_ == nullptr)
	{
		fade_ = new Fade(_fadeType, _fadeSpeed);
		fade_->Initialize();
		//��ԔN��̐e�̉��ɍ��
		if (_parent != nullptr)	_parent->GetRootJob()->PushBackChild(fade_);
	}
	fade_->SetFadeType(_fadeType);
}

bool FADE::IsFadeinFinished()
{
	if (fade_ == nullptr)	return false;
	if (fade_->GetFadeType() == TYPE_FADEIN)
	{
		if (fade_->IsFaded())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

bool FADE::IsFadeoutFinished()
{
	if (fade_ == nullptr)	return false;
	if (fade_->GetFadeType() == TYPE_FADEOUT)
	{
		if (fade_->IsFaded())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void FADE::FadeRemove()
{
	if (fade_ == nullptr) return;
	fade_->KillMe();
}


//�^���ÂɂȂ�Ƃ���alpha�l
const int alphaMax = 255 * 2;

Fade::Fade(FADETYPE _fadeType, float _fadeSpeed) 
	:hFade_(-1), fadeSpeed_(_fadeSpeed)
{
	objectName_ = "Fade";
	fadeType_ = _fadeType;
	Initialize();
}

Fade::~Fade()
{
}

void Fade::Initialize()
{
	hFade_ = Image::Load("Assets\\Fade.png");
	assert(hFade_ >= 0);

	XMFLOAT3 texSize = Image::GetTextureSize(hFade_);

	//�t�F�[�h�摜�̃T�C�Y����
	tFade_.scale_ = XMFLOAT3(Direct3D::bfr_scrWidth / texSize.x, Direct3D::bfr_scrHeight / texSize.y, 0);
}

void Fade::Update()
{
	//1�t���[���ő�����alpha�l
	const float alphaUpdate = alphaMax / (fadeSpeed_ * FPS);
	switch (fadeType_)
	{
	case TYPE_FADEIN:
		alpha_ -= alphaUpdate;
		break;
	case TYPE_FADEOUT:
		alpha_ += alphaUpdate;
		break;
	}
}

void Fade::Draw()
{
	Image::SetTransform(hFade_, tFade_);
	Image::SetAlpha(hFade_, alpha_);
	Image::Draw(hFade_);
}

void Fade::Release()
{
}

bool Fade::IsFaded()
{
	switch (fadeType_)
	{
	case TYPE_FADEIN:
		if (alpha_ < 0)
		{
			return true;
		}
		break;
	case TYPE_FADEOUT:
		if (alpha_ >= alphaMax)
		{
			return true;
		}
		break;
	}
	return false;
}

void Fade::SetFadeType(FADETYPE _fadeType)
{
	fadeType_ = _fadeType;
	switch (fadeType_)
	{
	case TYPE_FADEIN:
		alpha_ = alphaMax;
		break;
	case TYPE_FADEOUT:
		alpha_ = 0;
		break;
	}
}
