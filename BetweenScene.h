#pragma once
#include "Engine/GameObject.h"

class SceneTransition;

enum COURSELOGOSTATE
{
	STATE_WAIT = 0,
	STATE_MOVE1,
	STATE_MOVE2,
}; 

class BetweenScene
	:public GameObject
{
	float logoPlayerMoveCount_;
	float logoCourseMoveCount_;

	int hPlayer_;
	int hPlayer1_, hPlayer2_;
	int hLogoEmptyCourse_, hLogoCourse1_, hLogoCourse2_;
	int hLogoMapCreate_;

	Transform tLogoStandard_;
	Transform tPlayer1_, tPlayer2_;
	Transform tLogoCourse1_, tLogoCourse2_;
	Transform tLogoMapCreate_;

	COURSELOGOSTATE logoState_;

	SceneTransition* pST_;
public:
	BetweenScene(GameObject* parent);

	//èâä˙âª
	void Initialize() override;

	//çXêV
	void Update() override;

	//ï`âÊ
	void Draw() override;

	//äJï˙
	void Release() override;

	void Player1MapEditUpdate();
	void Player2MapEditUpdate();
	void Player1PlayUpdate();
	void Player2PlayUpdate();

	float EaseInSine(float pos);
	float EaseInOutBack(float pos);
};