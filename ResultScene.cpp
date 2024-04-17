#include "ResultScene.h"
#include "Button.h"
#include "SceneTransition.h"

#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Direct3D.h"
#include "Engine/SceneManager.h"

ResultScene::ResultScene(GameObject* parent)
	: GameObject(parent, "ResultScene")
{
	pST_ = (SceneTransition*)FindObject("SceneTransition");
}

void ResultScene::Initialize()
{
	isClear_Player_.resize(PLAYERNUMMAX);
	//���ꂼ��̃v���C���[���N���A�������ǂ�������
	for (int i = 0; i < PLAYERNUMMAX; i++)
	{
		isClear_Player_[i] = pST_->GetIsClear(i);
	}
	//�{�^���̉摜���[�h
	std::string ntFileName = "Button_NextTurn.png";
	std::string rtFileName = "Button_ReturnTitle.png";
	//�Q�[�����I��������
	if (isClear_Player_[0] != isClear_Player_[1])
	{
		ButtonInit(rtFileName);
	}
	//���s��������
	else
	{
		ButtonInit(ntFileName);
	}
}

void ResultScene::Update()
{
	if (pButton_->GetIsClicked())
	{
		//�Q�[�����I��������
		if (isClear_Player_[0] != isClear_Player_[1])
		{
			SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
			pSM->ChangeScene(SCENE_ID_TITLE);
		}
		//���s��������
		else
		{
			pST_->SetNextScene();
		}
		KillMe();
	}
}

void ResultScene::Draw()
{
}

void ResultScene::Release()
{
}

void ResultScene::ButtonInit(std::string _fileName)
{
	//�{�^���̉摜�������Ă�t�H���_��
	std::string folderName = "Assets\\Button\\SceneTrans\\";
	std::string fileName = folderName + _fileName;
	hButton_ = Image::Load(fileName);
	assert(hButton_ >= 0);

	pButton_->Instantiate<Button>(this);
	pButton_ = (Button*)FindObject("Button");
	pButton_->SetPictNum(hButton_);

	//�{�^���̈ʒu
	const XMFLOAT3 bPos = XMFLOAT3(0, -0.5, 0);
	//�{�^���̑傫��
	const XMFLOAT3 bScale = XMFLOAT3(0.5f, 0.5f, 0.5f);
	Transform bTransform;
	bTransform.position_ = bPos;
	bTransform.scale_ = bScale;
	pButton_->SetTransform(bTransform);
}

