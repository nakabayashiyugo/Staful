#include "Fade.h"

#include "SceneManager.h"
#include "Image.h"
#include "Direct3D.h"

//�^���ÂɂȂ�Ƃ���alpha�l
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

	//�t�F�[�h�摜�̃T�C�Y����
	tFade_.scale_ = XMFLOAT3(Direct3D::bfr_scrWidth / texSize.x, Direct3D::bfr_scrHeight / texSize.y, 0);
}

void Fade::Update()
{
	//1�t���[���ő�����alpha�l
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
