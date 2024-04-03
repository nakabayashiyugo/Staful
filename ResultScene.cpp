#include "ResultScene.h"
#include "Button.h"

#include "Engine/Image.h"
#include "Engine/Input.h"
#include "Engine/Direct3D.h"
#include "Engine/SceneManager.h"

ResultScene::ResultScene(GameObject* parent)
	: GameObject(parent, "ResultScene")
{
}

void ResultScene::Initialize()
{
	ResultRead();
}

void ResultScene::Update()
{
}

void ResultScene::Draw()
{
}

void ResultScene::Release()
{
}

void ResultScene::ResultRead()
{
	//player�P�̃t�@�C���ԍ�
	const int fileName1 = 1;
	//player�Q�̃t�@�C���ԍ�
	const int fileName2 = 2;

	//player�P���N���A�������ǂ����󂯎��
	std::ifstream read;
	std::string openfile = "SaveFile\\saveResult";

	openfile += std::to_string(fileName1);
	//  �t�@�C�����J��
	//  ios::in �͓ǂݍ��ݐ�p  ios::binary �̓o�C�i���`��
	read.open(openfile, std::ios::in);

	//  �t�@�C�����J���Ȃ������Ƃ��̑΍�
	if (!read) {
		std::cout << "�t�@�C�����J���܂���";
		return;
	}
	//�󂯎��l
	bool recvValue;
	//������ł͂Ȃ��f�[�^��ǂ݂���
	read.read((char*)&recvValue, sizeof(recvValue));

	isClear_Player_.push_back(recvValue);

	read.close();  //�t�@�C�������

	//player�Q���N���A�������ǂ����󂯎��
	openfile = "SaveFile\\saveResult";
	openfile += std::to_string(fileName2);
	read.open(openfile, std::ios::in);
	if (!read) {
		std::cout << "�t�@�C�����J���܂���";
		return;
	}
	read.read((char*)&recvValue, sizeof(recvValue));

	isClear_Player_.push_back(recvValue);

	read.close();  //�t�@�C�������
}
