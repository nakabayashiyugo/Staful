#include "Engine/GameObject.h"
#include <vector>


class Button;
class RadioButton;

#pragma once
namespace ButtonManager
{
	//ノーマルボタン
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

	////ラジオボタン
	////ボタンの追加
	//int AddRadioButton(std::string _btnName, int btn[], int _num, GameObject* _parent);
	////ボタンの削除
	//void RemoveRadioButton(int _handle);
	////ボタンの全削除
	//void RemoveAllRadioButton();
	////ボタンの取得
	//RadioButton* GetRadioButton(int _handle);

};

