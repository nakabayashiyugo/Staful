#include "Engine/GameObject.h"
#include <vector>

class Button;

#pragma once
namespace ButtonManager
{
	//ボタンの追加
	int AddButton(std::string _btnName, GameObject* _parent);
	//ボタンの削除
	void RemoveButton(int _handle);
	//ボタンの全削除
	void RemoveAllButton();

	//ボタンの取得
	Button* GetButton(int _handle);

	//画像の設定
	void SetPict(int _handle, int _imageHandle);
	//トランスフォームの設定
	void SetTransform(int _handle, Transform _transform);
};

