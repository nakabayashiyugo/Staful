#pragma once
#include "../Engine/GameObject.h"

class SceneTransition;
class Easing;

enum COURSELOGOSTATE
{
	STATE_WAIT = 0,
	STATE_MOVE1,
	STATE_MOVE2,
}; 

class BetweenScene
	:public GameObject
{
	//各ロゴの動きを制御するカウント
	float logoPlayerMoveCount_;
	float logoCourseMoveCount_;

	//各ロゴの画像番号
	int hPlayer_;
	int hPlayer1_, hPlayer2_;
	int hLogoEmptyCourse_, hLogoCourse1_, hLogoCourse2_;
	int hLogoMapCreate_, hLogoPlay_;

	//各ロゴのトランスフォーム
	Transform tLogoStandard_;
	Transform tPlayer1_, tPlayer2_;
	Transform tLogoCourse1_, tLogoCourse2_;
	Transform tLogoMapCreate_;

	COURSELOGOSTATE logoState_;

	SceneTransition* pST_;

	int curScene_;

	//イージングを使えるようにするクラス
	Easing* pEasing_;
public:
	BetweenScene(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void Player1MapEditUpdate();
	void Player2MapEditUpdate();
	void Player1PlayUpdate();
	void Player2PlayUpdate();

	//次に呼ぶ関数
	void NextCallFunction();

	//このシーンが終わった時
	void Finished();
};