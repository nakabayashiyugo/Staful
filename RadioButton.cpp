#include "RadioButton.h"

#include "Button.h"

RadioButton::RadioButton(int _btn[], int _num)
	:newLineNum_(0), 
	interval_(0, 0),
	posBase_(0, 0, 0),
	buttonSize_(1, 1, 1)
{
	//画像のセット
	radioButton_.resize(_num);
	for (int i = 0; i < radioButton_.size(); i++)
	{
		radioButton_[i]->SetPictNum(_btn[_num]);
	}
}

RadioButton::~RadioButton()
{
}

int RadioButton::GetPushButtonHandle()
{
	int ret = 0;
	for (int i = 0; i < radioButton_.size(); i++)
	{
		if (radioButton_[i]->GetIsClicked())
		{
			ret = i;
			break;
		}
	}

	return ret;
}

void RadioButton::ButtonPosAssign()
{
	for (int i = 0; i < radioButton_.size(); i++)
	{
		//ポジション計算
		//マス選択ボタンの位置
		XMFLOAT3 mbPos;
		//間隔
		const XMFLOAT3 mbInterval = XMFLOAT3((i % newLineNum_) * interval_.x,
			-(i / newLineNum_) * interval_.y, 0);
		mbPos.x = (float)(i % newLineNum_) * buttonSize_.x + posBase_.x + mbInterval.x;
		mbPos.y = -(float)(i / newLineNum_) * buttonSize_.y + posBase_.y + mbInterval.y;
		mbPos.z = posBase_.z;

		Transform t;
		t.position_ = mbPos;
		t.scale_ = buttonSize_;


		radioButton_[i]->SetTransform(t);
	}
}
