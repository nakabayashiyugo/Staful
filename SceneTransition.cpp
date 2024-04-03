#include "SceneTransition.h"
#include "MapEditScene.h"
#include "PlayScene.h"
#include "BetweenScene.h"

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
	hPlayer1_(-1), hPlayer2_(-1), hWin_(-1), hLose_(-1)
{
	//�X�e�[�W�̍ŏ��T�C�Y
	const int stageSizeMin = 5;
	//�X�e�[�W�̍ő�T�C�Y
	const int stageSizeMax = 20;
	XSIZE = (rand() % (stageSizeMax - stageSizeMin)) + stageSizeMin;
	ZSIZE = (rand() % (stageSizeMax - stageSizeMin)) + stageSizeMin;

	Math_Resize(XSIZE, ZSIZE, &math_);

	//���ׂ�Floor�ŏ�����
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
	hWin_ = Image::Load("Assets\\Logo_Win.png");
	assert(hWin_ >= 0);
	hLose_ = Image::Load("Assets\\Logo_Lose.png");
	assert(hLose_ >= 0);
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
		case SCENE_TURNEND:
			ResultWrite();
			pSM->ChangeScene(SCENE_ID_RESULT);
			break;
		default:
			break;
		}
	}
	prevSceneState_ = sceneState_;
}

void SceneTransition::Draw()
{
	Transform player;
	player.position_ = XMFLOAT3(0.7, 0.8, 0);
	const float hpRectx = 112;
	const float hpRecty = 17;
	const float hpRectWidth = 280;
	const float hpRectHeight = 75;
	Image::SetRect(hPlayer1_, hpRectx, hpRecty, hpRectWidth, hpRectHeight);
	Image::SetRect(hPlayer2_, hpRectx, hpRecty, hpRectWidth, hpRectHeight);
	switch (sceneState_)
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
	/*if (isFinished_)
	{
		const int finishTime = 3;
		static int cnt = 0;
		cnt++;
		if (cnt / FPS >= finishTime)
		{
			SceneManager* pSM = (SceneManager*)FindObject("SceneManager");
			pSM->ChangeScene(SCENE_ID_THANK);
		}

		
		player.position_ = XMFLOAT3(-0.3, 0.1, 0);
		Image::SetTransform(hPlayer1_, player);
		player.position_ = XMFLOAT3(0.3, 0.1, 0);
		Image::SetTransform(hPlayer2_, player);
		if (isClear_Player_[0] == true && isClear_Player_[1] == false)
		{
			player.position_ = XMFLOAT3(-0.3, -0.1, 0);
			Image::SetTransform(hWin_, player);
			player.position_ = XMFLOAT3(0.3, -0.1, 0);
			Image::SetTransform(hLose_, player);
		}
		else
		{
			player.position_ = XMFLOAT3(0.3, -0.3, 0);
			Image::SetTransform(hWin_, player);
			player.position_ = XMFLOAT3(-0.3, -0.3, 0);
			Image::SetTransform(hLose_, player);
		}
		Image::Draw(hPlayer1_);
		Image::Draw(hPlayer2_);
		Image::Draw(hWin_);
		Image::Draw(hLose_);
	}*/
}

void SceneTransition::Release()
{
}


void SceneTransition::Write()
{
	std::ofstream write;
	std::string savefile = saveFolderName + "saveMath";
	savefile += std::to_string(saveNum_);

	write.open(savefile, std::ios::out);

	//  �t�@�C�����J���Ȃ������Ƃ��̃G���[�\��
	if (!write) {
		std::cout << "�t�@�C�� " << savefile << " ���J���܂���";
		return;
	}

	for (int i = 0; i < XSIZE; i++) {
		for (int j = 0; j < ZSIZE; j++)
		{
			write.write((char*)&math_[i][j], sizeof(math_[i][j]));
			//������ł͂Ȃ��f�[�^����������
		}
	}

	write.close();  //�t�@�C�������

	//�Ƃ��Ƃ����[�g
	savefile = saveFolderName + "tgtgRoute";
	savefile += std::to_string(saveNum_);
	write.open(savefile, std::ios::out);
	//  �t�@�C�����J���Ȃ������Ƃ��̃G���[�\��
	if (!write) {
		std::cout << "�t�@�C�� " << savefile << " ���J���܂���";
		return;
	}
	for (auto itr = tTgtgRoute_.begin(); itr != tTgtgRoute_.end(); itr++)
	{
		write.write((char*)&itr, sizeof(itr));
	}
	write.close();  //�t�@�C�������
}

void SceneTransition::ResultWrite()
{
	//player�P�̃��U���g��ۑ�
	std::ofstream write;
	std::string savefile = saveFolderName + "saveResult";
	savefile += std::to_string(saveFileName1);

	write.open(savefile, std::ios::out);

	//  �t�@�C�����J���Ȃ������Ƃ��̃G���[�\��
	if (!write) {
		std::cout << "�t�@�C�� " << savefile << " ���J���܂���";
		return;
	}

	write.write((char*)&isClear_Player_[saveFileName1 - 1], sizeof(&isClear_Player_[saveFileName1 - 1]));
	write.close();  //�t�@�C�������

	//player�Q�̃��U���g��ۑ�
	savefile = saveFolderName + "saveResult";
	savefile += std::to_string(saveFileName2);
	write.open(savefile, std::ios::out);
	//  �t�@�C�����J���Ȃ������Ƃ��̃G���[�\��
	if (!write) {
		std::cout << "�t�@�C�� " << savefile << " ���J���܂���";
		return;
	}
	write.write((char*)&isClear_Player_[saveFileName2 - 1], sizeof(&isClear_Player_[saveFileName2 - 1]));
	write.close();  //�t�@�C�������
}
