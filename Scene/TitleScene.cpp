#include "TitleScene.h"
#include "ExpantionScene.h"

#include "../Button.h"

#include "../Engine/Image.h"
#include "../Engine/Model.h"
#include "../Engine/Input.h"
#include "../Engine/Direct3D.h"
#include "../Engine/SceneManager.h"
#include "../Engine/VFX.h"
#include "../Engine/BillBoard.h"
#include "../Engine/Fade.h"

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), hStartButton_(-1), hStaful_(-1)
{
	
	
}

void TitleScene::Initialize()
{
	//�{�^���̃t�@�C���l�[��
	std::string sbStr = "Button_GameStart.png";
	std::string exStr = "Button_Exit.png";
	//�{�^���̃t�@�C���������Ă�t�H���_
	std::string folderName = "Assets\\Button\\SceneTrans\\";
	sbStr = folderName + sbStr;
	exStr = folderName + exStr;
	//�X�^�[�g�{�^�����[�h
	hStartButton_ = Image::Load(sbStr);
	assert(hStartButton_ >= 0);

	hStaful_ = Image::Load("Assets\\Logo_Staful.png");
	assert(hStaful_ >= 0);

	pStartButton_->Instantiate<Button>(this);
	pStartButton_ = (Button*)FindObject("Button");
	pStartButton_->SetPictNum(hStartButton_);


}

void TitleScene::Update()
{
	//�X�^�[�g�{�^���������ė�������
	if (pStartButton_->GetIsReleased())
	{
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_EXPLANATION);
	}	
}

void TitleScene::Draw()
{
	//�X�^�[�g�{�^���̃g�����X�t�H�[��
	const XMFLOAT3 sbPos = XMFLOAT3(0.0f, -0.1f, 0.0f);
	const XMFLOAT3 sbScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	//Staful�̃��S�̃g�����X�t�H�[��
	const XMFLOAT3 sPos = XMFLOAT3(0.0f, 0.5f, 0.0f);
	const XMFLOAT3 sScale = XMFLOAT3(2.0f, 2.0f, 1.0f);

	tStartButton_.position_ = sbPos;
	tStartButton_.scale_ = sbScale;
	pStartButton_->SetTransform(tStartButton_);

	tStaful_.position_ = sPos;
	tStaful_.scale_ = sScale;


	Image::SetTransform(hStaful_, tStaful_);
	Image::Draw(hStaful_);
}

void TitleScene::Release()
{
}