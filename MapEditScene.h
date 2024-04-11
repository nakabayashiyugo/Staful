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


	//元々のマスの情報
	std::vector<std::vector<MATHDEDAIL>> math_origin_;
	//true : 選択されたコンベアマスが回る
	//false : コンベアマスは回らない
	std::vector<std::vector<bool>> isConvRot_;

	//テストプレイクリアしたかどうか
	//true : クリア
	//false : 未クリア
	bool isClear_;
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

//コスト管理について
private:
	//置ける障害物の数
	int mathChangeNumLimit_;
	//おいてる障害物の数
	int mathChangeNum_;
	//コストを表示するトランスフォーム
	std::string tCostText_;
	Text* pText_;
public:
	bool isMathChangeNumLimit();


//ファイルの書き込み・読み出しについて
private:
	//書き込む・読みだすファイル
	int saveNum_;
public:
	void Write();
	void Read();


//マスの選択について
private:
	//選ばれてるマスの種類
	int mathtype_;
public:
	//マスタイプ選択
	void SelectMathType();
	//クリックしたマスを選択してるマスに変える
	void ChangeSelectMath(XMFLOAT3 _selectMath);


//ボタンについて
private:
	//画面上のマスの種類を選択するボタン
	Button* pMathButton_[MATH_MAX];
	Button* pCompleteButton_;
	Button* pTestplayButton_;
	//ボタンの番号
	int buttonNum_;
	//テクスチャのサイズ
	XMFLOAT3 texture_size_;
public:
	//マス選択ボタンの初期化
	void ButtonInit(XMFLOAT3 _imageSize);

	int GetButtonNum() { return buttonNum_; }


//マスの説明について
private:
	//マスの説明の画像番号
	int hExpantion_[MATH_MAX];
	//マスの説明の位置
	Transform tExpantion_;
public:		
	//マスの説明の表示の初期化
	void ExpantionInit();
	//マスの説明の表示のDraw
	void ExpantionDraw();

	
	//マップエディターを表示するかしないか
	//引数 false : 表示
	//　　 true : 非表示
	void IsHidden(bool _isHidden);

	void SetIsClear(bool _isClear) { isClear_ = _isClear; }
};

