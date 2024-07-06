#pragma once
#include <vector>

#include "Engine/GameObject.h"

class Button;

class RadioButton
{
	std::vector<Button*> radioButton_;
	//ボタン何個で改行するか
	int newLineNum_;
	//ボタンの感覚
	XMFLOAT2 interval_;
	//基準となる左上のボタンの位置(今のところ左上から横書きしかできない)
	XMFLOAT3 posBase_;
	//ボタンの大きさ
	XMFLOAT3 buttonSize_;

public:
	//引数１：親オブジェクト
	//引数２：画像番号が入った配列のポインタ
	//引数３：引数２の配列のサイズ
	RadioButton(int _btn[], int _num);
	~RadioButton();

	int GetPushButtonHandle();

	//ボタンにポジションをセット
	void ButtonPosAssign();

//セッター・ゲッター類
public:
	void SetNewLineNum(int _num) { newLineNum_ = _num; }
	void SetInterval(XMFLOAT2 _interval) { interval_ = _interval; }
	void SetPosBase(XMFLOAT3 _pos) { posBase_ = _pos; }
	void SetButtonSize(XMFLOAT3 _size) { buttonSize_ = _size; }

};

