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
	int costLimit_;
	//おいてる障害物の数
	int curCost_;
	//それぞれのマスのコスト
	std::vector<int> costs_;
	//コストを表示する位置
	XMFLOAT3 costTextPos_;
	Text* pText_;
public:
	bool costManagement();


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
	//マスの初期化
	void MathInit();
	//マスの表示
	void MathDraw();
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
	void ButtonInit();

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


//テストプレイについて
private:
	//テストプレイクリアしたかどうか
	//true : クリア
	//false : 未クリア
	bool isClear_;
	//テストプレイができるかどうか
	bool canTest_;

	//マップエディターを表示するか
	//true : 表示
	//false : 非表示
	bool isDisp_;
public:
	//マップエディターを表示するかしないか
	//引数 false : 表示
	//　　 true : 非表示
	void IsDisplay(bool _isDisp);
	void SetIsDisp(bool _isDisp) { isDisp_ = _isDisp; }
	//テストプレイができるかどうか
	void CheckCanTest();
	//クリアしたかどうかのセッター
	void SetIsClear(bool _isClear) { isClear_ = _isClear; }
};

