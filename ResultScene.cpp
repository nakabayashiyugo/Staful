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
	//player１のファイル番号
	const int fileName1 = 1;
	//player２のファイル番号
	const int fileName2 = 2;

	//player１がクリアしたかどうか受け取る
	std::ifstream read;
	std::string openfile = "SaveFile\\saveResult";

	openfile += std::to_string(fileName1);
	//  ファイルを開く
	//  ios::in は読み込み専用  ios::binary はバイナリ形式
	read.open(openfile, std::ios::in);

	//  ファイルが開けなかったときの対策
	if (!read) {
		std::cout << "ファイルが開けません";
		return;
	}
	//受け取る値
	bool recvValue;
	//文字列ではないデータを読みこむ
	read.read((char*)&recvValue, sizeof(recvValue));

	isClear_Player_.push_back(recvValue);

	read.close();  //ファイルを閉じる

	//player２がクリアしたかどうか受け取る
	openfile = "SaveFile\\saveResult";
	openfile += std::to_string(fileName2);
	read.open(openfile, std::ios::in);
	if (!read) {
		std::cout << "ファイルが開けません";
		return;
	}
	read.read((char*)&recvValue, sizeof(recvValue));

	isClear_Player_.push_back(recvValue);

	read.close();  //ファイルを閉じる
}
