#include "Fade.h"

#include "SceneManager.h"
#include "Image.h"
#include "Direct3D.h"

//真っ暗になるときのalpha値
const int alphaMax = 255 * 2;

Fade::Fade(FADETYPE _fadeType) :hFade_(-1), fadeSpeed_(0.5f)
{
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
	alpha_ += alphaUpdate;
	if (alpha_ >= alphaMax)
	{
		alpha_ = alphaMax;
		isFinished_ = true;
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
