#include "PlayScene.h"

#include "../Engine/Input.h"
#include "../Engine/SceneManager.h"
#include "../Engine/Image.h"
#include "../Engine/Audio.h"

#include "SceneTransition.h"
#include "MapEditScene/MapEditScene.h"

#include "../Timer.h"
#include "../Stage.h"
#include "../Button.h"
#include "../GamePlayer.h"
#include "../ButtonManager.h"

#include "../Player/Player.h"
#include "../Player/Enemy.h"

PlayScene::PlayScene(GameObject* parent)
	: GameObject(parent, "PlayScene"),
	table_Change_(false),
	floorHeight_(0),
	playerHeight_(floorHeight_ + MODELHEIGHT),
	hAudio_Music_(-1)
{
	MathVolumeRead();
	if (pParent_->GetObjectName() == "MapEditScene")
	{
		pGP_ = (GamePlayer*)this->pParent_->GetParent();
	}
	else
	{
		pGP_ = (GamePlayer*)this->pParent_;
		AudioInit();
	}
	playerNum_ = pGP_->GetPlayerNum();
	Math_Resize(mathVolume_.x, mathVolume_.z, &math_);
	saveNum_ = pGP_->GetSaveNum();
	Read();
}

PlayScene::~PlayScene()
{
	ButtonManager::RemoveButton(hCancel_);
}

void PlayScene::Initialize()
{
	pStage_ = (Stage*)Instantiate<Stage>(this);
	//自分がEnemyだったら
	if (pGP_->GetIsEnemy())
	{
		pPlayer_ = (Enemy*)Instantiate<Enemy>(this);
	}
	else
	{
		pPlayer_ = (Player*)Instantiate<Player>(this);
	}

	//テストプレイの場合
	if (pParent_->GetObjectName() == "MapEditScene")
	{
		//中止ボタン
		std::string buttonCancel = "Button_Cancel.png";
		//中止ボタンのロード
		std::string folderName = "Assets\\Button\\";
		buttonCancel = folderName + buttonCancel;
		int cancelNum = Image::Load(buttonCancel);
		assert(cancelNum >= 0);

		//中止ボタンのトランスフォーム
		hCancel_ = ButtonManager::AddButton("cancelButton", (GameObject*)this);
		cancelButton_ = ButtonManager::GetButton(hCancel_);
		const XMFLOAT3 cbPos = XMFLOAT3(-0.8f, -0.7f, 0);
		const XMFLOAT3 cbScale = XMFLOAT3(0.3f, 0.3f, 1);
		Transform ccbTransform;
		ccbTransform.position_ = cbPos;
		ccbTransform.scale_ = cbScale;
		cancelButton_->SetTransform(ccbTransform);
		cancelButton_->SetPictNum(cancelNum);
	}
}

void PlayScene::Update()
{
	//テストプレイの場合
	if (pParent_->GetObjectName() == "MapEditScene")
	{
		MapEditScene* pMES = (MapEditScene*)FindObject("MapEditScene");
		if (pPlayer_->GetStageState() == STATE_GOAL)
		{
			pMES->SetIsClear(true);
			//マップエディター表示
			pMES->SetIsDisp(true);
			KillMe();
		}
		if (pPlayer_->GetStageState() == STATE_FAILURE)
		{
			//マップエディター表示
			pMES->SetIsDisp(true);
			KillMe();
		}

		//中止ボタンが押されたら
		if (cancelButton_->OnClick())
		{
			pMES->CancelButtonPush();
		}
	}
	else
	{
		Audio::Play(hAudio_Music_, 0.5f);
		if (pPlayer_->GetStageState() == STATE_GOAL)
		{
			pGP_->SetIsClear(true);
			pGP_->ChallengeFinished();
			Audio::Stop(hAudio_Music_);
			KillMe();
		}
		if (pPlayer_->GetStageState() == STATE_FAILURE)
		{
			pGP_->SetIsClear(false);
			pGP_->ChallengeFinished();
			Audio::Stop(hAudio_Music_);
			KillMe();
		}
	}
}

void PlayScene::Draw()
{
}

void PlayScene::Release()
{
}


XMFLOAT3 PlayScene::GetPlayerPos()
{
	XMFLOAT3 retMath = XMFLOAT3(0, 0, 0);
	if (pPlayer_ == nullptr)
	{
		return retMath;
	}
	retMath = pPlayer_->GetPosition();
	return retMath;
}

MATHDEDAIL PlayScene::GetPlayerStandMath()
{
	MATHDEDAIL retMath;
	//返すMATHDEDAILの初期値
	MATHTYPE retMathInit = MATH_DELETE;
	Transform retTransformInit;
	retMath = { retMathInit, retTransformInit };
	if (pPlayer_ == nullptr || pPlayer_->GetPlayerState() != STATE_IDLE)
	{
		return retMath;
	}
	retMath = pPlayer_->GetStandMath();
	return retMath;
}

void PlayScene::PlayerNumDraw()
{
	//プレイヤー番号表示
	pGP_->PlayerNumDraw();
}

void PlayScene::CallWrite()
{
	this->Write();
}

void PlayScene::CallRead()
{
	this->Read();
}

void PlayScene::AudioInit()
{
	//音楽が入ってるフォルダ名
	const std::string folderName = "Assets\\Audio\\";
	//音楽
	const std::string musicFolder = "Music\\";
	//音楽ロード
	std::string music = folderName + musicFolder + "Audio_Challenge.wav";
	hAudio_Music_ = Audio::Load(music, true);
}
