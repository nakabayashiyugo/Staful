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
	//それぞれのプレイヤーがクリアしたかどうか受取り
	for (int i = 0; i < PLAYERNUMMAX; i++)
	{
		isClear_Player_[i] = pST_->GetIsClear(i);
	}
	//ボタンの画像ロード
	std::string ntFileName = "Button_NextTurn.png";
	std::string rtFileName = "Button_ReturnTitle.png";
	//ゲームが終了したら
	if (isClear_Player_[0] != isClear_Player_[1])
	{
		ButtonInit(rtFileName);
	}
	//続行だったら
	else
	{
		ButtonInit(ntFileName);
	}
}

void ResultScene::Update()
{
	if (pButton_->GetIsClicked())
	{
		//ゲームが終了したら
		if (isClear_Player_[0] != isClear_Player_[1])
		{
			SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
			pSM->ChangeScene(SCENE_ID_TITLE);
		}
		//続行だったら
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
	//ボタンの画像が入ってるフォルダ名
	std::string folderName = "Assets\\Button\\SceneTrans\\";
	std::string fileName = folderName + _fileName;
	hButton_ = Image::Load(fileName);
	assert(hButton_ >= 0);

	pButton_->Instantiate<Button>(this);
	pButton_ = (Button*)FindObject("Button");
	pButton_->SetPictNum(hButton_);

	//ボタンの位置
	const XMFLOAT3 bPos = XMFLOAT3(0, -0.5, 0);
	//ボタンの大きさ
	const XMFLOAT3 bScale = XMFLOAT3(0.5f, 0.5f, 0.5f);
	Transform bTransform;
	bTransform.position_ = bPos;
	bTransform.scale_ = bScale;
	pButton_->SetTransform(bTransform);
}

