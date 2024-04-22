#include "SceneTransition.h"
#include "MapEditScene.h"
#include "PlayScene.h"
#include "BetweenScene.h"
#include "ResultScene.h"

#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Input.h"

const int FPS = 60;
const int saveFileName1 = 1, saveFileName2 = 2;

SceneTransition::SceneTransition(GameObject* parent)
	: GameObject(parent, "SceneTransition"), 
	sceneState_(SCENE_MAPEDIT1), prevSceneState_(SCENE_TURNEND),
	turnNum_(0), player_Num_(0), saveNum_(saveFileName1),
	isClear_Player_{ false, false },
	hPlayer1_(-1), hPlayer2_(-1)
{
	//ステージの最小サイズ
	const int stageSizeMin = 5;
	//ステージの最大サイズ
	const int stageSizeMax = 18;
	XSIZE = (rand() % (stageSizeMax - stageSizeMin)) + stageSizeMin;
	ZSIZE = (rand() % (stageSizeMax - stageSizeMin)) + stageSizeMin;

	Math_Resize(XSIZE, ZSIZE, &math_);

	//すべてFloorで初期化
	for (int x = 0; x < XSIZE; x++)
	{
		for (int y = 0; y < ZSIZE; y++)
		{
			math_[x][y].mathType_ = MATH_FLOOR;
		}
	}
}

void SceneTransition::Initialize()
{
	Write();
	sceneState_ = SCENE_MAPEDIT2;
	saveNum_ = saveFileName2;
	Write();

	sceneState_ = SCENESTATE(0);

	hPlayer1_ = Image::Load("Assets\\Logo_Player1.png");
	assert(hPlayer1_ >= 0);
	hPlayer2_ = Image::Load("Assets\\Logo_Player2.png");
	assert(hPlayer2_ >= 0);
}

void SceneTransition::Update()
{
	SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
	if (sceneState_ != prevSceneState_)
	{
		switch (sceneState_)
		{
		case SCENE_BETWEEN1:
			turnNum_++;
			if (turnNum_ % 2 == 0)	saveNum_ = saveFileName2;
			else saveNum_ = saveFileName1;
			Instantiate<BetweenScene>(this);
			break;
		case SCENE_MAPEDIT1:Instantiate<MapEditScene>(this); break;
		case SCENE_BETWEEN2:
			if (turnNum_ % 2 == 0)	saveNum_ = saveFileName1;
			else saveNum_ = saveFileName2;
			Instantiate<BetweenScene>(this);
			break;
		case SCENE_MAPEDIT2:Instantiate<MapEditScene>(this); break;
		case SCENE_BETWEEN3:Instantiate<BetweenScene>(this); break;
		case SCENE_STAGE1:
			if (turnNum_ % 2 == 0)	saveNum_ = saveFileName1;
			else saveNum_ = saveFileName2;
			player_Num_ = 0;
			pPS_[player_Num_]->Instantiate<PlayScene>(this);
			pPS_[player_Num_] = (PlayScene*)FindObject("PlayScene");
			break;
		case SCENE_BETWEEN4:Instantiate<BetweenScene>(this); break;
		case SCENE_STAGE2:
			if (turnNum_ % 2 == 0)	saveNum_ = saveFileName2;
			else saveNum_ = saveFileName1;
			player_Num_ = 1;
			pPS_[player_Num_]->Instantiate<PlayScene>(this);
			pPS_[player_Num_] = (PlayScene*)FindObject("PlayScene");
			break;
		case SCENE_RESULT:
			Instantiate<ResultScene>(this);
			break;
		case SCENE_TURNEND:
			sceneState_ = SCENESTATE(0);
			return;
		default:
			break;
		}
	}
	prevSceneState_ = sceneState_;
}

void SceneTransition::Draw()
{
}

void SceneTransition::Release()
{
}

void SceneTransition::PlayerNumDraw()
{
	pST = (SceneTransition*)FindObject("SceneTransition");
	Transform player;
	player.position_ = XMFLOAT3(0.7, 0.8, 0);
	switch (pST->GetSceneState())
	{
	case SCENE_MAPEDIT1:
	case SCENE_STAGE1:
		Image::SetTransform(hPlayer1_, player);
		Image::Draw(hPlayer1_);
		break;
	case SCENE_MAPEDIT2:
	case SCENE_STAGE2:
		Image::SetTransform(hPlayer2_, player);
		Image::Draw(hPlayer2_);
		break;
	}
}


void SceneTransition::Write()
{
	std::ofstream write;
	std::string savefile = saveFolderName + "saveMath";
	savefile += std::to_string(saveNum_);

	write.open(savefile, std::ios::out);

	//  ファイルが開けなかったときのエラー表示
	if (!write) {
		std::cout << "ファイル " << savefile << " が開けません";
		return;
	}

	for (int i = 0; i < XSIZE; i++) {
		for (int j = 0; j < ZSIZE; j++)
		{
			write.write((char*)&math_[i][j], sizeof(math_[i][j]));
			//文字列ではないデータをかきこむ
		}
	}

	write.close();  //ファイルを閉じる

	//とげとげルート
	savefile = saveFolderName + "tgtgRoute";
	savefile += std::to_string(saveNum_);
	write.open(savefile, std::ios::out);
	//  ファイルが開けなかったときのエラー表示
	if (!write) {
		std::cout << "ファイル " << savefile << " が開けません";
		return;
	}
	for (auto itr = tTgtgRoute_.begin(); itr != tTgtgRoute_.end(); itr++)
	{
		write.write((char*)&itr, sizeof(itr));
	}
	write.close();  //ファイルを閉じる
}

void SceneTransition::ResultWrite()
{
	//player１のリザルトを保存
	std::ofstream write;
	std::string savefile = saveFolderName + "saveResult";
	savefile += std::to_string(saveFileName1);

	write.open(savefile, std::ios::out);

	//  ファイルが開けなかったときのエラー表示
	if (!write) {
		std::cout << "ファイル " << savefile << " が開けません";
		return;
	}

	write.write((char*)&isClear_Player_[saveFileName1 - 1], sizeof(&isClear_Player_[saveFileName1 - 1]));
	write.close();  //ファイルを閉じる

	//player２のリザルトを保存
	savefile = saveFolderName + "saveResult";
	savefile += std::to_string(saveFileName2);
	write.open(savefile, std::ios::out);
	//  ファイルが開けなかったときのエラー表示
	if (!write) {
		std::cout << "ファイル " << savefile << " が開けません";
		return;
	}
	write.write((char*)&isClear_Player_[saveFileName2 - 1], sizeof(&isClear_Player_[saveFileName2 - 1]));
	write.close();  //ファイルを閉じる
}
