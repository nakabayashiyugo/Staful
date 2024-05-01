#include "Shaker.h"

#include "Easing.h"

#include "../Timer.h"

//振動の移動量を制限する値
const float SHAKEDEC = 0.01f;

Shaker::Shaker()
{	
	shakeTime_ = 0;
	shakePower_ = 0;			
	isShake_ = false;
	prevIsShake_ = false;

}

Shaker::~Shaker()
{
	delete pTimer_;
	delete pEasing_;
}

void Shaker::ShakeInit(XMFLOAT3 *_position, float _vibTime, float _vibPower)
{
	shakeTime_ = _vibTime;
	shakePower_ = _vibPower;
	//振動前のオブジェクトの位置
	position_ = _position;
	pTimer_ = new Timer(shakeTime_);
	pEasing_ = new Easing();
	//オブジェクトの振動の方向
	shakeDir_ = XMVectorSet(rand(), rand(), rand(), 0);
	//オブジェクトの振動の方向にオブジェクトの振動の強さ分の長さを持たせる
	shakeDir_ = XMVector3Normalize(shakeDir_) * shakePower_ * SHAKEDEC;
}

void Shaker::ShakeUpdate()
{
	pTimer_->Update();
	//camMoveCountの初期値
	const float moveCountInit = 0;
	//カメラの動きを制御するカウント
	static float moveCount = moveCountInit;
	//camMoveCountの毎フレーム足される値
	const float moveCountPlus = 0.5f;
	moveCount += moveCountPlus;
	//prevPositionを入れるvector
	XMVECTOR prevPosVec = XMLoadFloat3(position_);
	//retPosに値入れる
	float ease = pEasing_->EaseInSine(moveCount);
	XMStoreFloat3(position_, (prevPosVec * shakeDir_) * ease + prevPosVec);

	if (ease <= 0)
	{
		moveCount = moveCountInit;
		//オブジェクトの振動の方向
		shakeDir_ = XMVectorSet(rand() % 10, rand() % 10, rand() % 10, 0);
		//オブジェクトの振動の方向にオブジェクトの振動の強さ分の長さを持たせる
		shakeDir_ = XMVector3Normalize(shakeDir_) * shakePower_ * SHAKEDEC;
	}
	if (pTimer_->isTimeUpped())
	{
		isShake_ = false;
		return;
	}
}

void Shaker::SetVibElem(float _vibTime, float _vibPower)
{
	shakeTime_ = _vibTime;
	shakePower_ = _vibPower;
}

void Shaker::SetIsShake(bool _isShake)
{
	isShake_ = _isShake;
}
