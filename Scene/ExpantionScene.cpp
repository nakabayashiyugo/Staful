#include "ExpantionScene.h"

#include "SceneTransition.h"

#include "../ButtonManager.h"
#include "../Button.h"

#include "../Engine/Easing.h"
#include "../Engine/Image.h"
#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Fade.h"

//ボタンの番号
const int exNextButtonNum = 0;
const int exReturnButtonNum = 1;
const int nextButtonNum = 2;

ExpantionScene::ExpantionScene(GameObject* parent)
	: GameObject(parent, "ExpantionScene"), hNext_(-1), hExplanNextButton_(-1), hExplanBackButton_(-1),
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
	hExplanBackButton_ = Image::Load(exNext);
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
	ExplanPositioning();
}

void ExpantionScene::Update()
{
	//了解ボタンが押されたら
	if (ButtonManager::GetButton(nextBtnHandle_)->OnClick())
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_TRANSITION);
	}
	//右のボタンが押されたら
	if (ButtonManager::GetButton(explanNextBtnHandle_)->OnClick())
	{
		prevPos_ = (int)curExplanNum_;
		destPos_ = curExplanNum_ + 1;
		isMoving_ = true;
	}
	//左のボタンが押されたら
	if (ButtonManager::GetButton(explanBackBtnHandle_)->OnClick())
	{
		prevPos_ = (int)curExplanNum_;
		destPos_ = curExplanNum_ - 1;
		isMoving_ = true;
	}
	if (curExplanNum_ >= explanationNum - 1)
	{
		ButtonManager::GetButton(explanNextBtnHandle_)->SetIsCanPush(false);
	}
	else
	{
		ButtonManager::GetButton(explanNextBtnHandle_)->SetIsCanPush(true);
	}
	if (curExplanNum_ <= 0)
	{
		ButtonManager::GetButton(explanBackBtnHandle_)->SetIsCanPush(false);
	}
	else
	{
		ButtonManager::GetButton(explanBackBtnHandle_)->SetIsCanPush(true);
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
	nextBtnHandle_ = ButtonManager::AddButton("nextButton", (GameObject*)this);
	//position
	tNext_.position_ = XMFLOAT3(0.7f, -0.75f, 0);
	//scale
	tNext_.scale_ = XMFLOAT3(0.5f, 0.5f, 0);
	ButtonManager::SetTransform(nextBtnHandle_, tNext_);
	ButtonManager::SetPict(nextBtnHandle_, hNext_);

	//基準位置
	const XMFLOAT3 exButtonPosInit = XMFLOAT3(0.9f, 0, 0);
	//基準サイズ
	const XMFLOAT3 exButtonScaleInit = XMFLOAT3(0.2f, 0.2f, 0);

	//次ボタン
	explanNextBtnHandle_ = ButtonManager::AddButton("explanNextButton", (GameObject*)this);
	//position
	tExplanNextButton_.position_ = exButtonPosInit;
	//scale
	tExplanNextButton_.scale_ = exButtonScaleInit;
	ButtonManager::SetTransform(explanNextBtnHandle_, tExplanNextButton_);
	ButtonManager::SetPict(explanNextBtnHandle_, hExplanNextButton_);

	//戻るボタン
	explanBackBtnHandle_ = ButtonManager::AddButton("explanBackButton", (GameObject*)this);
	//position
	tExplanBackButton_.position_ = XMFLOAT3(-exButtonPosInit.x, exButtonPosInit.y, exButtonPosInit.z);
	//rotate
	tExplanBackButton_.rotate_.z = 180.0f;
	//scale
	tExplanBackButton_.scale_ = XMFLOAT3(exButtonScaleInit);
	ButtonManager::SetTransform(explanBackBtnHandle_, tExplanBackButton_);
	ButtonManager::SetPict(explanBackBtnHandle_, hExplanBackButton_);
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
