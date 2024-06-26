#pragma once
#include "Transform.h"

class Timer;
class Easing;

enum SHAKETYPE
{
	TYPE_VERTICAL = 0,	//縦揺れ
	TYPE_BESIDE,		//横揺れ
	TYPE_RANDOM,		//ランダム揺れ
	TYPE_BASIC,		//指定なし
	TYPE_MAX,
};

class Shaker
{
private:
	//カメラの振動に関する変数
	XMFLOAT3 *position_;		//振動するオブジェクトの位置
	XMFLOAT3 prevPosition_;		//振動する前の位置
	XMVECTOR shakeDir_;			//振動する方向
	float shakeTime_;			//振動する時間
	float shakePower_;			//振動の強さ
	bool isShake_, prevIsShake_;//振動するかどうか
	float moveCount_;			//振動の補間値
	XMVECTOR shaft_;			//振動の軸
	Timer* pTimer_;
	Easing* pEasing_;

	SHAKETYPE shakeType_;

public:
	Shaker();
	~Shaker();

	//カメラの振動
	//引数1 : 振動させたいオブジェクトのポジションのポインタ
	//引数2 : 振動の時間
	//引数3 : 振動の強さ
	void ShakeInit(XMFLOAT3 *_position, float _vibTime, float _vibPower);
	void ShakeUpdate();

	//移動方向を決める
	void ShakeDirAssign();

	void SetVibElem(float _vibTime, float _vibPower);

	//振動するかどうかのセッター
	void SetIsShake(bool _isShake);
	//振動するかどうかのゲッター
	bool GetIsShake() { return isShake_; }

	//補間値のゲッター
	float GetMoveCount() { return moveCount_; }

	//振動の軸のセッター
	void SetShaft(XMVECTOR _shaft) { shaft_ = _shaft; }
	void SetShaft(XMFLOAT3 _shaft) { shaft_ = XMLoadFloat3(&_shaft); }

	//振動の種類のセッター
	void SetShakeType(SHAKETYPE _type) { shakeType_ = _type; }
};

