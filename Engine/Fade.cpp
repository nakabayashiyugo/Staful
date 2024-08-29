#include "Fade.h"

#include "SceneManager.h"
#include "Image.h"
#include "Direct3D.h"

namespace FADE
{
	std::vector<Fade*> fades_;
}

int FADE::FadeStart(GameObject* _parent, float _fadeSpeed, FADETYPE _fadeType)
{
	//親オブジェクトの子供にすでにフェードが存在したらもう作らない
	if (_parent->FindChildObject("Fade") != nullptr)
	{
		return -1;
	}
	Fade* fade = new Fade(_fadeType, _fadeSpeed);
	fade->Initialize();
	if(_parent != nullptr)	_parent->PushBackChild(fade);
	fades_.push_back(fade);
	return fades_.size() - 1;
}

bool FADE::FadeEnd(int _handle)
{
	if (_handle < 0 && _handle >= fades_.size()) return false;
	if (fades_[_handle]->IsFaded())
	{
		return true;
	}
	return false;
}

int FADE::FadeSerch(Fade* _fade)
{
	for (int i = 0; i < fades_.size(); i++)
	{
		if (_fade == fades_[i])
		{
			return i;
		}
	}
	return -1;
}


//真っ暗になるときのalpha値
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

	//フェード画像のサイズ調整
	tFade_.scale_ = XMFLOAT3(Direct3D::bfr_scrWidth / texSize.x, Direct3D::bfr_scrHeight / texSize.y, 0);
}

void Fade::Update()
{
	//1フレームで増えるalpha値
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
			KillMe();
			return true;
		}
		break;
	case TYPE_FADEOUT:
		if (alpha_ >= alphaMax)
		{
			KillMe();
			return true;
		}
		break;
	}
	return false;
}
