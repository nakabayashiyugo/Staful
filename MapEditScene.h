#pragma once
#include "StageOrigin.h"

class Text;
class Button;

class MapEditScene
	:public GameObject, StageOrigin
{
	int hPict_[MATH_MAX];
	int hTgtgRoute_;

	//マスの縦方向の量
	int YSIZE;

	//選ばれてるマスの種類
	int mathtype_;
	//書き込む・読みだすファイル
	int saveNum_;
	//置ける障害物の数
	int mathChangeNumLimit_;
	//おいてる障害物の数
	int mathChangeNum_;

	XMFLOAT3 texture_size_;

	//元々のマスの情報
	std::vector<std::vector<MATHDEDAIL>> math_origin_;
	//true : 選択されたコンベアマスが回る
	//false : コンベアマスは回らない
	std::vector<std::vector<bool>> isConvRot_;

	Text* pText_;

	//画面上のマスの種類を選択するボタン
	Button* pMathButton_[MATH_MAX];
public:
	MapEditScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

	void Write();
	void Read();

	bool isMathChangeNumLimit();

	//マス選択ボタンの初期化
	void MathButtonInit(XMFLOAT3 _imageSize);
};

