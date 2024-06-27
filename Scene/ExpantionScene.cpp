#include "ExpantionScene.h"

#include "SceneTransition.h"

#include "../Button.h"

#include "../Engine/Easing.h"
#include "../Engine/Image.h"
#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"

//ボタンの番号
const int exNextButtonNum = 0;
const int exReturnButtonNum = 1;
const int nextButtonNum = 2;

ExpantionScene::ExpantionScene(GameObject* parent)
	: GameObject(parent, "ExpantionScene"), hNext_(-1), hExplanNextButton_(-1), hExplanReturnButton_(-1),
	curExplanNum_(0), isMoving_(false), destPos_(0), prevPos_(0)
{
	for (int i = 0; i < explanationNum; i++)
	{
		hExplan_[i] = -1;
	}

}

void ExpantionScene::Initialize()
{
	//画像のロード
	//画像の入ってるフォルダー
	const std::string folderName = "Assets\\GameExplanation\\";
	std::string next = folderName + "Next.png";
	std::string exNext = folderName + "ExplanNext.png";
	std::string overview = folderName + "GameOverview.png";

	hNext_ = Image::Load(next);
	hExplanNextButton_ = Image::Load(exNext);
	hExplanReturnButton_ = Image::Load(exNext);
	hOverview_ = Image::Load(overview);
	assert(hNext_ >= 0);
	assert(hExplanNextButton_ >= 0);
	assert(hOverview_ >= 0);

	std::string ex[explanationNum];
	for (int i = 0; i < explanationNum; i++)
	{
		ex[i] = folderName + "GameExplanation" + std::to_string(i + 1) + ".png";
		hExplan_[i] = Image::Load(ex[i]);
		assert(hExplan_[i] >= 0);
	}

	//トランスフォームの初期化
	tOverview_.position_ = XMFLOAT3(0, 0.8f, 0);
	tExplan_[0].position_ = XMFLOAT3(0, 0, 0);

	ButtonInit();
}

void ExpantionScene::Update()
{
	if (pNext_->GetIsClicked())
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TRANSITION);
	}
	if (pExplanNextButton_->GetIsClicked())
	{
		pExplanNextButton_->SetIsClicked(false);
		prevPos_ = (int)curExplanNum_;
		destPos_ = curExplanNum_ + 1;
		isMoving_ = true;
	}
	if (pExplanReturnButton_->GetIsClicked())
	{
		pExplanReturnButton_->SetIsClicked(false);
		prevPos_ = (int)curExplanNum_;
		destPos_ = curExplanNum_ - 1;
		isMoving_ = true;
	}
	if (curExplanNum_ >= explanationNum - 1)
	{
		pExplanNextButton_->SetIsCanPush(false);
	}
	else
	{
		pExplanNextButton_->SetIsCanPush(true);
	}
	if (curExplanNum_ <= 0)
	{
		pExplanReturnButton_->SetIsCanPush(false);
	}
	else
	{
		pExplanReturnButton_->SetIsCanPush(true);
	}
	if (isMoving_)
	{
		ExplanMove();
	}
}

void ExpantionScene::Draw()
{
	Image::SetTransform(hOverview_, tOverview_);
	Image::Draw(hOverview_);
	for(int i = 0; i < explanationNum; i++)
	{
		Image::SetTransform(hExplan_[i], tExplan_[i]);
		Image::Draw(hExplan_[i]);
	}
	
}

void ExpantionScene::Release()
{
}

void ExpantionScene::ButtonInit()
{
	//了解ボタン
	buttonNum_ = nextButtonNum;
	pNext_->Instantiate<Button>(this);
	pNext_ = (Button*)FindObject("Button" + std::to_string(buttonNum_));
	//position
	tNext_.position_ = XMFLOAT3(0.7f, -0.75f, 0);
	//scale
	tNext_.scale_ = XMFLOAT3(0.5f, 0.5f, 0);
	pNext_->SetTransform(tNext_);
	pNext_->SetPictNum(hNext_);

	//基準位置
	const XMFLOAT3 exButtonPosInit = XMFLOAT3(0.9f, 0, 0);
	//基準サイズ
	const XMFLOAT3 exButtonScaleInit = XMFLOAT3(0.2f, 0.2f, 0);

	//次ボタン
	buttonNum_ = exNextButtonNum;
	pExplanNextButton_->Instantiate<Button>(this);
	pExplanNextButton_ = (Button*)FindObject("Button" + std::to_string(buttonNum_));
	//position
	tExplanNextButton_.position_ = XMFLOAT3(exButtonPosInit);
	//scale
	tExplanNextButton_.scale_ = XMFLOAT3(exButtonScaleInit);
	pExplanNextButton_->SetTransform(tExplanNextButton_);
	pExplanNextButton_->SetPictNum(hExplanNextButton_);

	//戻るボタン
	buttonNum_ = exReturnButtonNum;
	pExplanReturnButton_->Instantiate<Button>(this);
	pExplanReturnButton_ = (Button*)FindObject("Button" + std::to_string(buttonNum_));
	//position
	tExplanReturnButton_.position_ = XMFLOAT3(-exButtonPosInit.x, exButtonPosInit.y, exButtonPosInit.z);
	//rotate
	tExplanReturnButton_.rotate_.z = 180.0f;
	//scale
	tExplanReturnButton_.scale_ = XMFLOAT3(exButtonScaleInit);
	pExplanReturnButton_->SetTransform(tExplanReturnButton_);
	pExplanReturnButton_->SetPictNum(hExplanReturnButton_);
}

void ExpantionScene::ExplanPositioning()
{
	//curExplanNum_が範囲外の時のエラー処理
	if (curExplanNum_ < 0)
	{
		curExplanNum_ = 0;
	}
	else if (curExplanNum_ >= explanationNum)
	{
		curExplanNum_ = explanationNum - 1;
	}
	//ゲームの説明画像の間隔
	const float exSpace = 2.0f;
	for (int i = 0; i < explanationNum; i++)
	{
		tExplan_[i].position_.x = (float)(i - curExplanNum_) * exSpace;
	}
}

void ExpantionScene::ExplanMove()
{
	Easing* pEasing = new Easing();
	//初期値
	const float moveCntInit = 0.0f;
	//毎フレーム足される値
	const float moveCntUpdate = 0.1f;
	static float moveCnt = moveCntInit;
	moveCnt += moveCntUpdate;
	float ease = pEasing->EaseInSine(moveCnt);

	curExplanNum_ = (float)prevPos_ + ((float)destPos_ - prevPos_) * ease;
	if (ease >= 1)
	{
		curExplanNum_ = destPos_;
		moveCnt = moveCntInit;
		isMoving_ = false;
	}
	ExplanPositioning();
}
