#pragma once
#include "StageOrigin.h"

class Text;

class MapEditScene
	:public GameObject, StageOrigin
{
	int hPict_[MATH_MAX + 1];
	int hTgtgRoute_;


	int YSIZE;
	int mathtype_;
	int saveNum_;
	int mathChangeNumLimit_;
	int mathChangeNum_;

	XMFLOAT3 texture_size_;

	std::vector<std::vector<MATHDEDAIL>> math_origin_;
	std::vector<std::vector<bool>> isConvRot_;

	Text* pText_;
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
};

