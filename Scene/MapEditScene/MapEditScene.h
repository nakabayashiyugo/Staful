#pragma once
#include "../../StageOrigin.h"

class Text;
class Button;
class GamePlayer;
class MathManager;
class PlayScene;

class MapEditScene
	:public GameObject, public StageOrigin
{
protected:
	int hPict_[MATH_MAX];
	
	int hTgtgRoute_;

	//元々のマスの情報
	std::vector<std::vector<MATHDEDAIL>> math_origin_;
	//true : 選択されたコンベアマスが回る
	//false : コンベアマスは回らない
	std::vector<std::vector<bool>> isConvRot_;

	GamePlayer* pGP_;

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

//コスト管理について
private:
	//それぞれのマスのコスト
	std::vector<int> costs_;
	Text* pText_;
public:
	void CostDraw();


//マスの選択について
private:
	//選ばれてるマスの種類
	int mathtype_;
	//カーソルが置かれてるマスの位置
	XMFLOAT3 selectMath_;

	//右クリックしたときのマウスの位置
	XMFLOAT2 rightClickPos_;
	//右クリックを話した時のマウスの位置
	XMFLOAT2 rightReleasePos_;
	//左クリックしたときのマウスの位置
	XMFLOAT2 leftClickPos_;

	MathManager* table_;
public:
	//マスの初期化
	void MathInit();
	//マスの表示
	virtual void MathDraw();

	//左クリックしたときの処理
	void LeftClicked();
	//右クリックしたときの処理
	void RightClicked();
	//右クリック離した時の処理
	void RightReleased();

	//とげとげルートに新しくできたとげとげの情報を入れる
	void TgtgPushBack(XMFLOAT3 _mousePos);

	//カーソルが置かれてるマスの位置の値いれる
	virtual void SelectMathSet();
	//マスタイプ選択
	virtual void SelectMathType();

	//クリックしたマスを選択してるマスに変える
	virtual void ChangeSelectMath();

	int GetMathType() { return mathtype_; }
	void SetMathType(int _type) { mathtype_ = _type; }
	XMFLOAT3 GetSelectMath() { return selectMath_; }
	void SetSelectMath(XMFLOAT3 _select) { selectMath_ = _select; }
	MathManager* GetTable() { return table_; }


//ボタンについて
private:
	//画面上のマスの種類を選択するボタン
	Button* pMathButton_[MATH_MAX];
	//完了ボタン
	Button* pCompleteButton_;
	//テストプレイボタン
	Button* pTestplayButton_;
	//中止ボタン
	Button* pCancelButton_;
	//テクスチャのサイズ
	XMFLOAT3 texture_size_;

	//テストプレイにつかうPlayScene
	PlayScene* testPS_;
public:
	//マス選択ボタンの初期化
	void ButtonInit();
	//それぞれのボタンが押された時の処理
	void ButtonUpdate();
	//マス選択ボタン以外のボタンが押されたとき
	void OtherButtonPush();

	void CompButtonPush();
	void TestButtonPush();
	void CancelButtonPush();

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


//とげとげについて
private:
	//とげとげマスを押した位置
	XMFLOAT3 tgtgRouteMathDown_;
	//とげとげマスを押してドラッグして話した位置
	XMFLOAT3 tgtgRouteMathUp_;
public:
	//とげとげルートの設定
	void TogetogeRouteSet();
	//とげとげマスが別のマスに変わった時の要素の削除
	void TogetogeElemDelete();


private:
	//音楽のハンドル
	int hAudio_Music_;
	//マスを置いたときになるSEのハンドル
	int hSE_PutMath_;
	//コンベアを回転させたときのSEのハンドル
	int hSE_ConvRot_;
	
public:
	void AudioInit();


//書き込み・読み込み
private:

public:
	void Write() override;
	void Read() override;
};