#include "ExpantionScene.h"
#include "SceneTransition.h"

#include "../Button.h"

#include "../Engine/Image.h"
#include "../Engine/Input.h"

//ボタンの番号
const int exNextButtonNum = 0;
const int exReturnButtonNum = 1;
const int nextButtonNum = 2;

ExpantionScene::ExpantionScene(GameObject* parent)
	: GameObject(parent, "ExpantionScene"), hNext_(-1), hExplanNextButton_(-1), hExplanReturn_(-1)
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
	tOverview_.position_ = XMFLOAT3(0, 0.7, 0);
	tExplan_[0].position_ = XMFLOAT3(0, 0, 0);
}

void ExpantionScene::Update()
{
}

void ExpantionScene::Draw()
{
	Image::SetTransform(hOverview_, tOverview_);
	Image::Draw(hOverview_);
	Image::SetTransform(hExplan_[0], tExplan_[0]);
	Image::Draw(hExplan_[0]);
}

void ExpantionScene::Release()
{
}

void ExpantionScene::ButtonInit()
{
	pNext_ = Instantiate<Button>(this);

	tNext_.position_ = XMFLOAT3(0.7f, -0.7f, 0);
	//基準値
	const XMFLOAT3 exButtonInit = XMFLOAT3(0.7, 0, 0);
	tExplanNextButton_.position_ = XMFLOAT3(exButtonInit);
	tExplanReturnButton_.position_ = XMFLOAT3(-exButtonInit.x, exButtonInit.y, exButtonInit.z);
	tExplanReturnButton_.rotate_.z = 180.0f;

	pNext_->SetTransform(tNext_);
	pNext_->SetPictNum(hNext_);
	pExplanNextButton_->SetTransform(tExplanNextButton_);
	pExplanNextButton_->SetPictNum(hExplanNextButton_);
	pExplanReturnButton_->SetTransform(tExplanReturnButton_);
	pExplanReturnButton_->SetPictNum(hExplanReturnButton_);


}
