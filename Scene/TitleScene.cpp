#include "TitleScene.h"
#include "ExpantionScene.h"

#include "../ButtonManager.h"
#include "../MathInit.h"
#include "../Button.h"


#include "../Engine/Image.h"
#include "../Engine/Model.h"
#include "../Engine/Input.h"
#include "../Engine/Direct3D.h"
#include "../Engine/SceneManager.h"
#include "../Engine/VFX.h"
#include "../Engine/BillBoard.h"
#include "../Engine/Fade.h"

//��l�v���C�A��l�v���C�̎��̏������ރn���h��
const int soloHandle = 0;
const int twoHandle = 1;

TitleScene::TitleScene(GameObject* parent)
	: GameObject(parent, "TitleScene"), 
	hSoloPlayButton_(-1), 
	hTwoPlayButton_(-1), 
	hStaful_(-1),
	soloPlayHandle_(-1),
	twoPlayHandle_(-1)
{
	
	
}

void TitleScene::Initialize()
{
	//�{�^���̃t�@�C���l�[��
	std::string spStr = "Button_SoloPlay.png";
	std::string tpStr = "Button_TwoPlay.png";
	//�{�^���̃t�@�C���������Ă�t�H���_
	std::string folderName = "Assets\\Button\\SceneTrans\\";
	spStr = folderName + spStr;
	tpStr = folderName + tpStr;
	//�\���v���C�{�^�����[�h
	hSoloPlayButton_ = Image::Load(spStr);
	assert(hSoloPlayButton_ >= 0);
	//��l�v���C�{�^�����[�h
	hTwoPlayButton_ = Image::Load(tpStr);
	assert(hTwoPlayButton_ >= 0);

	hStaful_ = Image::Load("Assets\\Logo_Staful.png");
	assert(hStaful_ >= 0);

	soloPlayHandle_ = ButtonManager::AddButton("soloPlayButton", this);
	ButtonManager::SetPict(soloPlayHandle_, hSoloPlayButton_);
	twoPlayHandle_ = ButtonManager::AddButton("twoPlayButton", this);
	ButtonManager::SetPict(twoPlayHandle_, hTwoPlayButton_);
}


void TitleScene::Update()
{
	static int fadeHandle = -1;
	//�\���v���C�{�^���������ė�������
	if (ButtonManager::GetButton(soloPlayHandle_)->OnClick())
	{
		fadeHandle = FADE::FadeStart(this, 1.0f, TYPE_FADEOUT);
		GameStateWrite(soloHandle);
	}
	//��l�v���C�{�^���������ė�������
	else if (ButtonManager::GetButton(twoPlayHandle_)->OnClick())
	{
		fadeHandle = FADE::FadeStart(this, 1.0f, TYPE_FADEOUT);
		GameStateWrite(twoHandle);
	}
	//�t�F�[�h���I�������
	if (FADE::FadeEnd(fadeHandle))
	{
		MathInit* mathInit = new MathInit();
		SceneManager* pSceneManager = (SceneManager*)FindObject("SceneManager");
		pSceneManager->ChangeScene(SCENE_ID_EXPLANATION);
		delete mathInit;
	}
}

void TitleScene::Draw()
{
	//�{�^���̃g�����X�t�H�[��
	const XMFLOAT3 sbPos = XMFLOAT3(0.3f, -0.1f, 0.0f);
	const XMFLOAT3 sbScale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	//Staful�̃��S�̃g�����X�t�H�[��
	const XMFLOAT3 sPos = XMFLOAT3(0.0f, 0.5f, 0.0f);
	const XMFLOAT3 sScale = XMFLOAT3(2.0f, 2.0f, 1.0f);

	tSoloPlayButton_.position_ = sbPos;
	tSoloPlayButton_.scale_ = sbScale;
	ButtonManager::SetTransform(soloPlayHandle_, tSoloPlayButton_);

	tTwoPlayButton_.position_ = XMFLOAT3(-sbPos.x, sbPos.y, sbPos.z);
	tTwoPlayButton_.scale_ = sbScale;
	ButtonManager::SetTransform(twoPlayHandle_, tTwoPlayButton_);

	tStaful_.position_ = sPos;
	tStaful_.scale_ = sScale;
	Image::SetTransform(hStaful_, tStaful_);
	Image::Draw(hStaful_);
}

void TitleScene::Release()
{
}

void TitleScene::GameStateWrite(int _state)
{
	std::ofstream write;
	//�}�X��񏑂�����
	std::string savefile = "SaveFile\\gameState";
	write.open(savefile, std::ios::out);
	//  �t�@�C�����J���Ȃ������Ƃ��̃G���[�\��
	if (!write) {
		std::cout << "�t�@�C�� " << savefile << " ���J���܂���";
		return;
	}
	write.write((char*)&_state, sizeof(_state));

	write.close();  //�t�@�C�������
}
