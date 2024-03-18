#include "BetweenScene.h"
#include "SceneTransition.h"

#include "Engine/Image.h"
#include "Engine/SceneManager.h"

const float PIE = 3.141592;
const int FPS = 60;
const int moveUpdate = 5;

BetweenScene::BetweenScene(GameObject* parent)
	: GameObject(parent, "BetweenScene"), 
	hPlayer1_(-1), hPlayer2_(-1),
	hLogoEmptyCourse_(-1), hLogoCourse1_(-1), hLogoCourse2_(-1),
	logoPlayerMoveCount_(0), logoCourseMoveCount_(0)
{
}

void BetweenScene::Initialize()
{
	hPlayer1_ = Image::Load("Assets\\Logo_Player1.png");
	assert(hPlayer1_ >= 0);
	hPlayer2_ = Image::Load("Assets\\Logo_Player2.png");
	assert(hPlayer2_ >= 0);
	hLogoEmptyCourse_ = Image::Load("Assets\\Logo_EmptyCourse.png");
	assert(hLogoEmptyCourse_ >= 0);
	hLogoCourse1_ = Image::Load("Assets\\Logo_Course1.png");
	assert(hLogoCourse1_ >= 0);
	hLogoCourse2_ = Image::Load("Assets\\Logo_Course2.png");
	assert(hLogoCourse2_ >= 0);

	
	tLogoStandard_.position_ = XMFLOAT3(0.5f, 0.3f, 0);
	tLogoStandard_.scale_ = XMFLOAT3(0.5f, 0.5f, 1);

	tPlayer1_.position_ = XMFLOAT3(tLogoStandard_.position_.x, -tLogoStandard_.position_.y, tLogoStandard_.position_.z);
	tPlayer1_.scale_ = tLogoStandard_.scale_;
	tPlayer2_.position_ = XMFLOAT3(-tLogoStandard_.position_.x, -tLogoStandard_.position_.y, tLogoStandard_.position_.z);
	tPlayer2_.scale_ = tLogoStandard_.scale_;
	tLogoCourse1_.position_ = XMFLOAT3(tLogoStandard_.position_.x, tLogoStandard_.position_.y, tLogoStandard_.position_.z);
	tLogoCourse1_.scale_ = tLogoStandard_.scale_;
	tLogoCourse2_.position_ = XMFLOAT3(-tLogoStandard_.position_.x, tLogoStandard_.position_.y, tLogoStandard_.position_.z);
	tLogoCourse2_.scale_ = tLogoStandard_.scale_;
}

void BetweenScene::Update()
{
	SceneTransition* pST = (SceneTransition*)FindObject("SceneTransition");
	SCENESTATE curScene = (SCENESTATE)pST->GetSceneState();
	switch (curScene)
	{
	case SCENE_BETWEEN1:
		Player1MapEditUpdate();
		break;
	case SCENE_BETWEEN2:
		Player2MapEditUpdate();
		break;
	case SCENE_BETWEEN3:
		Player1PlayUpdate();
		break;
	case SCENE_BETWEEN4:
		Player1PlayUpdate();
		break;
	}
}

void BetweenScene::Draw()
{
	Image::SetTransform(hPlayer1_, tPlayer1_);
	Image::Draw(hPlayer1_);
	Image::SetTransform(hPlayer2_, tPlayer2_);
	Image::Draw(hPlayer2_);
	Image::SetTransform(hLogoEmptyCourse_, tLogoCourse1_);
	Image::Draw(hLogoEmptyCourse_);
	Image::SetTransform(hLogoEmptyCourse_, tLogoCourse2_);
	Image::Draw(hLogoEmptyCourse_);
}

void BetweenScene::Release()
{
	KillMe();
}

void BetweenScene::Player1MapEditUpdate()
{
	logoPlayerMoveCount_ += moveUpdate;
	tPlayer1_.position_.y = -tLogoStandard_.position_.y + sin(logoPlayerMoveCount_ * PIE / 180) / 10;

	if (logoPlayerMoveCount_ / moveUpdate >= FPS)
	{
		logoCourseMoveCount_ += 0.01f;
		float ease = EaseInSine(logoCourseMoveCount_);
		if (ease <= 1)
		{
			tLogoCourse1_.position_.x = tLogoStandard_.position_.x * (1 - ease);
		}
	}
}

void BetweenScene::Player2MapEditUpdate()
{
}

void BetweenScene::Player1PlayUpdate()
{
}

void BetweenScene::Player2PlayUpdate()
{
}

float BetweenScene::EaseInSine(float pos)
{
	return 1 - cos((pos * PIE) / 2);
}
