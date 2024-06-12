//#include <cmath>
//
//#include "BetweenScene.h"
//#include "SceneTransition.h"
//
//#include "../Engine/Image.h"
//#include "../Engine/SceneManager.h"
//#include "../Engine/Easing.h"
//
//const float PIE = 3.141592;
//const int moveUpdate = 5;
//
//BetweenScene::BetweenScene(GameObject* parent)
//	: GameObject(parent, "BetweenScene"), 
//	hPlayer1_(-1), hPlayer2_(-1),
//	hLogoEmptyCourse_(-1), hLogoCourse1_(-1), hLogoCourse2_(-1),
//	hLogoMapCreate_(-1), hLogoPlay_(-1),
//	logoPlayerMoveCount_(0), logoCourseMoveCount_(0), logoState_(COURSELOGOSTATE(0)),
//	curScene_(SCENESTATE(0))
//{
//	pST_ = (SceneTransition*)FindObject("SceneTransition");
//	pEasing_ = new Easing();
//}
//
//void BetweenScene::Initialize()
//{
//	hPlayer1_ = Image::Load("Assets\\Logo_Player1.png");
//	assert(hPlayer1_ >= 0);
//	hPlayer2_ = Image::Load("Assets\\Logo_Player2.png");
//	assert(hPlayer2_ >= 0);
//
//	hLogoCourse1_ = Image::Load("Assets\\Logo_CourseA.png");
//	assert(hLogoCourse1_ >= 0);
//	hLogoCourse2_ = Image::Load("Assets\\Logo_CourseB.png");
//	assert(hLogoCourse2_ >= 0);
//	if (pST_->GetTurnNum() % 2 == 0)
//	{
//		int tmp = hLogoCourse1_;
//		hLogoCourse1_ = hLogoCourse2_;
//		hLogoCourse2_ = tmp;
//	}
//	if (pST_->GetSceneState() == SCENE_BETWEEN4)
//	{
//		int tmp = hLogoCourse1_;
//		hLogoCourse1_ = hLogoCourse2_;
//		hLogoCourse2_ = tmp;
//	}
//	hLogoMapCreate_ = Image::Load("Assets\\Logo_MapCreate.png");
//	assert(hLogoMapCreate_ >= 0);
//	hLogoPlay_ = Image::Load("Assets\\Logo_Let'sPlay.png");
//	assert(hLogoPlay_ >= 0);
//
//	tLogoStandard_.position_ = XMFLOAT3(0.5f, 0.3f, 0);
//	tLogoStandard_.scale_ = XMFLOAT3(0.5f, 0.5f, 1);
//
//	tPlayer1_.position_ = XMFLOAT3(tLogoStandard_.position_.x, -tLogoStandard_.position_.y, tLogoStandard_.position_.z);
//	tPlayer1_.scale_ = tLogoStandard_.scale_;
//	tPlayer2_.position_ = XMFLOAT3(-tLogoStandard_.position_.x, -tLogoStandard_.position_.y, tLogoStandard_.position_.z);
//	tPlayer2_.scale_ = tLogoStandard_.scale_;
//	tLogoCourse1_.position_ = XMFLOAT3(tLogoStandard_.position_.x, tLogoStandard_.position_.y, tLogoStandard_.position_.z);
//	tLogoCourse1_.scale_ = tLogoStandard_.scale_;
//	tLogoCourse2_.position_ = XMFLOAT3(-tLogoStandard_.position_.x, tLogoStandard_.position_.y, tLogoStandard_.position_.z);
//	tLogoCourse2_.scale_ = tLogoStandard_.scale_;
//
//	tLogoMapCreate_.position_.y = -0.55f;
//}
//
//void BetweenScene::Update()
//{
//	curScene_ = (SCENESTATE)pST_->GetSceneState();
//	switch (curScene_)
//	{
//	case SCENE_BETWEEN1:
//		Player1MapEditUpdate();
//		break;
//	case SCENE_BETWEEN2:
//		Player2MapEditUpdate();
//		break;
//	case SCENE_BETWEEN3:
//		Player1PlayUpdate();
//		break;
//	case SCENE_BETWEEN4:
//		Player2PlayUpdate();
//		break;
//	}
//}
//
//void BetweenScene::Draw()
//{
//	Image::SetTransform(hLogoCourse1_, tLogoCourse1_);
//	Image::Draw(hLogoCourse1_);
//	Image::SetTransform(hLogoCourse2_, tLogoCourse2_);
//	Image::Draw(hLogoCourse2_);
//
//
//	Image::SetTransform(hPlayer1_, tPlayer1_);
//	Image::Draw(hPlayer1_);
//	Image::SetTransform(hPlayer2_, tPlayer2_);
//	Image::Draw(hPlayer2_);
//	
//	if (curScene_ < SCENE_BETWEEN3)
//	{
//		Image::SetTransform(hLogoMapCreate_, tLogoMapCreate_);
//		Image::Draw(hLogoMapCreate_);
//	}
//	else if (logoState_ == STATE_MOVE2)
//	{
//		Image::SetTransform(hLogoPlay_, tLogoMapCreate_);
//		Image::Draw(hLogoPlay_);
//	}
//}
//
//void BetweenScene::Release()
//{
//}
//
//void BetweenScene::Player1MapEditUpdate()
//{
//	logoPlayerMoveCount_ += moveUpdate;
//	tPlayer1_.position_.y = -tLogoStandard_.position_.y + sin(logoPlayerMoveCount_ * PIE / 180) / 10;
//
//	float ease = 0;
//
//	const float decMove = 5;
//
//	switch (logoState_)
//	{
//	case STATE_WAIT:
//		if (logoPlayerMoveCount_ / moveUpdate >= FPS)
//		{
//			logoState_ = STATE_MOVE1;
//		}
//		break;
//	case STATE_MOVE1:
//		logoCourseMoveCount_ += 0.01f;
//		ease = pEasing_->EaseInSine(logoCourseMoveCount_);
//		tLogoCourse1_.position_.x = tLogoStandard_.position_.x * (1 - ease);
//		if (ease > 1)
//		{
//			logoState_ = STATE_MOVE2;
//			logoCourseMoveCount_ = 0;
//		}
//		break;
//	case STATE_MOVE2:
//		logoCourseMoveCount_ += 0.01f;
//		ease = pEasing_->EaseInOutBack(logoCourseMoveCount_);
//		tLogoCourse1_.position_.y += ease / decMove;
//		if (ease > 1)
//		{
//			pST_->SetNextScene();
//			KillMe();
//		}
//		break;
//	}
//}
//
//void BetweenScene::Player2MapEditUpdate()
//{
//	logoPlayerMoveCount_ += moveUpdate;
//	tPlayer2_.position_.y = -tLogoStandard_.position_.y + sin(logoPlayerMoveCount_ * PIE / 180) / 10;
//
//	float ease = 0;
//
//	const float decMove = 5;
//
//	switch (logoState_)
//	{
//	case STATE_WAIT:
//		if (logoPlayerMoveCount_ / moveUpdate >= FPS)
//		{
//			logoState_ = STATE_MOVE1;
//		}
//		break;
//	case STATE_MOVE1:
//		logoCourseMoveCount_ += 0.01f;
//		ease = pEasing_->EaseInSine(logoCourseMoveCount_);
//		tLogoCourse2_.position_.x = -tLogoStandard_.position_.x * (1 - ease);
//		if (ease > 1)
//		{
//			logoState_ = STATE_MOVE2;
//			logoCourseMoveCount_ = 0;
//		}
//		break;
//	case STATE_MOVE2:
//		logoCourseMoveCount_ += 0.01f;
//		ease = pEasing_->EaseInOutBack(logoCourseMoveCount_);
//		tLogoCourse2_.position_.y += ease / decMove;
//		if (ease > 1)
//		{
//			pST_->SetNextScene();
//			KillMe();
//		}
//		break;
//	}
//}
//
//void BetweenScene::Player1PlayUpdate()
//{
//	float decMove = 5;
//	float ease = 0;
//	switch (logoState_)
//	{
//	case STATE_WAIT:
//		logoCourseMoveCount_ += 0.01f;
//		ease = pEasing_->EaseInOutBack(logoCourseMoveCount_);
//		tLogoCourse1_.position_.y += ease / decMove;
//		tLogoCourse2_.position_.y += ease / decMove;
//		if (logoCourseMoveCount_ > 1)
//		{
//			logoState_ = STATE_MOVE1;
//			logoCourseMoveCount_ = 0;
//			float tmp = tLogoCourse1_.position_.x;
//			tLogoCourse1_.position_.x = tLogoCourse2_.position_.x;
//			tLogoCourse2_.position_.x = tmp;
//		}
//		break;
//	case STATE_MOVE1:
//		logoCourseMoveCount_ += 0.01f;
//		ease = pEasing_->EaseInOutBack(logoCourseMoveCount_);
//		tLogoCourse1_.position_.y = (1 - ease) + 0.3f;
//		tLogoCourse2_.position_.y = (1 - ease) + 0.3f;
//		if (logoCourseMoveCount_ > 1)
//		{
//			logoState_ = STATE_MOVE2;
//		}
//		break;
//	case STATE_MOVE2:
//		logoPlayerMoveCount_ += moveUpdate;
//		tPlayer1_.position_.y = -tLogoStandard_.position_.y + sin(logoPlayerMoveCount_ * PIE / 180) / 10;
//		if (logoPlayerMoveCount_ / moveUpdate >= FPS * 2)
//		{
//			pST_->SetNextScene();
//			KillMe();
//		}
//		break;
//	}
//}
//
//void BetweenScene::Player2PlayUpdate()
//{
//	logoState_ = STATE_MOVE2;
//	logoPlayerMoveCount_ += moveUpdate;
//	tPlayer2_.position_.y = -tLogoStandard_.position_.y + sin(logoPlayerMoveCount_ * PIE / 180) / 10;
//	if (logoPlayerMoveCount_ / moveUpdate >= FPS * 2)
//	{
//		pST_->SetNextScene();
//		KillMe();
//	}
//}
