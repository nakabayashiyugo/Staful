#include "ResultScene.h"

#include "SceneTransition.h"

#include "../Button.h"

#include "../Engine/Image.h"
#include "../Engine/Input.h"
#include "../Engine/Direct3D.h"
#include "../Engine/SceneManager.h"

ResultScene::ResultScene(GameObject* parent)
	: GameObject(parent, "ResultScene")
{
	pST_ = (SceneTransition*)FindObject("SceneTransition");
}

void ResultScene::Initialize()
{
	isClear_.resize(PLAYERNUMMAX);
	//それぞれのプレイヤーがクリアしたかどうか受取り
	ResultRead();
	//ボタンの画像ロード
	std::string ntFileName = "Button_NextTurn.png";
	std::string rtFileName = "Button_ReturnTitle.png";
	//ゲームが終了したら
	if (isClear_[0] != isClear_[1])
	{
		ButtonInit(rtFileName);
	}
	//続行だったら
	else
	{
		ButtonInit(ntFileName);
	}
	PictInit();
}

void ResultScene::Update()
{
	if (pButton_->GetIsClicked())
	{
		//ゲームが終了したら
		if (isClear_[0] != isClear_[1])
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
	PictDraw();
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
	const XMFLOAT3 bPos = XMFLOAT3(0, -0.7, 0);
	//ボタンの大きさ
	const XMFLOAT3 bScale = XMFLOAT3(0.5f, 0.5f, 0.5f);
	Transform bTransform;
	bTransform.position_ = bPos;
	bTransform.scale_ = bScale;
	pButton_->SetTransform(bTransform);
}

void ResultScene::PictInit()
{
	//ロードするファイルの名前
	std::string player1 = "Logo_Player1.png";
	std::string player2 = "Logo_Player2.png";
	std::string win = "Logo_Win.png";
	std::string lose = "Logo_Lose.png";
	std::string draw = "Logo_Draw.png";
	//ファイルが入ってるフォルダ名
	std::string folderName = "Assets\\";
	//フォルダ名たす
	player1 = folderName + player1;
	player2 = folderName + player2;
	win = folderName + win;
	lose = folderName + lose;
	draw = folderName + draw;
	//ロード
	hPlayer1_ = Image::Load(player1);
	hPlayer2_ = Image::Load(player2);
	hWin_ = Image::Load(win);
	hLose_ = Image::Load(lose);
	hDraw_ = Image::Load(draw);
	assert(hPlayer1_ >= 0);
	assert(hPlayer2_ >= 0);
	assert(hWin_ >= 0);
	assert(hLose_ >= 0);
	assert(hDraw_ >= 0);
}
void ResultScene::PictDraw()
{
	//画像を表示する位置
	Transform tPict;
	const XMFLOAT3 p1Pos = XMFLOAT3(-0.5f, 0.3f, 0);
	const XMFLOAT3 p2Pos = XMFLOAT3(0.5f, 0.3f, 0);
	const XMFLOAT3 wPos = XMFLOAT3(0.5f, 0, 0);
	const XMFLOAT3 lPos = XMFLOAT3(0.5f, 0, 0);
	const XMFLOAT3 dPos = XMFLOAT3(0, -0.3f, 0);

	//プレイヤー1ロゴの設定
	tPict.position_ = p1Pos;
	Image::SetTransform(hPlayer1_, tPict);
	Image::Draw(hPlayer1_);

	//プレイヤー２ロゴの設定
	tPict.position_ = p2Pos;
	Image::SetTransform(hPlayer2_, tPict);
	Image::Draw(hPlayer2_);

	//勝利、敗北、引き分けロゴの設定
	if (isClear_[0] && !isClear_[1])
	{
		tPict.position_ = XMFLOAT3(p1Pos.x, wPos.y, wPos.z);
		Image::SetTransform(hWin_, tPict);
		tPict.position_ = XMFLOAT3(p2Pos.x, lPos.y, lPos.z);
		Image::SetTransform(hLose_, tPict);
		Image::Draw(hWin_);
		Image::Draw(hLose_);
	}
	else if (!isClear_[0] && isClear_[1])
	{
		tPict.position_ = XMFLOAT3(p2Pos.x, wPos.y, wPos.z);
		Image::SetTransform(hWin_, tPict);
		tPict.position_ = XMFLOAT3(p1Pos.x, lPos.y, lPos.z);
		Image::SetTransform(hLose_, tPict);
		Image::Draw(hWin_);
		Image::Draw(hLose_);
	}
	else
	{
		tPict.position_ = dPos;
		Image::SetTransform(hDraw_, tPict);
		Image::Draw(hDraw_);
	}
}

void ResultScene::ResultRead()
{
	for (int i = 0; i < PLAYERNUMMAX; i++)
	{
		std::ifstream read;
		std::string openfile = "SaveFile\\result";
		openfile += std::to_string(i);

		read.open(openfile, std::ios::in);
		//  ファイルを開く
		//  ios::in は読み込み専用  ios::binary はバイナリ形式

		if (!read) {
			std::cout << "ファイルが開けません";
			return;
		}
		//  ファイルが開けなかったときの対策
		bool isClear;
		read.read((char*)&isClear, sizeof(isClear));
		isClear_[i] = isClear;
		//文字列ではないデータを読みこむ
		read.close();  //ファイルを閉じる

	}
}

