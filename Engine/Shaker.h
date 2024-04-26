#pragma once
#include "Transform.h"

class Timer;
class Easing;

class Shaker
{
private:
	//カメラの振動に関する変数
	XMFLOAT3 *position_;		//振動する前の位置
	XMVECTOR shakeDir_;			//振動する方向
	float shakeTime_;			//振動する時間
	float shakePower_;			//振動の強さ
	int shakeNum_;				//振動する回数
	bool isShake_, prevIsShake_;//振動するかどうか
	Timer* pTimer_;
	Easing* pEasing_;

public:
	Shaker();
	~Shaker();

	//カメラの振動
	//引数1 : 振動させたいオブジェクトのポジションのポインタ
	//引数2 : 振動の時間
	//引数3 : 振動の強さ
	void ShakeInit(XMFLOAT3 *_position, float _vibTime, float _vibPower);
	void ShakeUpdate();

	void SetVibElem(float _vibTime, float _vibPower);

	//振動するかどうかのセッター
	void SetIsShake(bool _isShake);
	//振動するかどうかのゲッター
	bool GetIsShake() { return isShake_; }
};

