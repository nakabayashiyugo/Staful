#include "Shaker.h"

#include <time.h>

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

void Shaker::ShakeInit(XMFLOAT3 *_position, SHAKETYPE _shakeType, float _vibTime, float _vibPower)
{
	shakeTime_ = _vibTime;
	shakePower_ = _vibPower;
	shakeType_ = _shakeType;
	//振動前のオブジェクトの位置
	position_ = _position;
	prevPosition_ = *position_;
	pTimer_ = new Timer(shakeTime_);
	pEasing_ = new Easing();
	ShakeDirAssign();
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
	XMVECTOR prevPosVec = XMLoadFloat3(&prevPosition_);
	//retPosに値入れる
	float ease = pEasing_->EaseInSine(moveCount);
	XMStoreFloat3(position_, (prevPosVec * shakeDir_) * ease + prevPosVec);

	if (ease <= 0)
	{
		moveCount = moveCountInit;
		ShakeDirAssign();
	}
	if (pTimer_->isTimeUpped())
	{
		isShake_ = false;
		return;
	}
}

void Shaker::ShakeDirAssign()
{
	srand(time(NULL));
	
	switch (shakeType_)
	{
	case TYPE_VERTICAL:
		//オブジェクトの振動の方向
		shakeDir_ = XMVectorSet(0, rand() - RAND_MAX / 2, 0, 0);
		break;
	case TYPE_BESIDE:
		//オブジェクトの振動の方向
		shakeDir_ = XMVectorSet(rand() - RAND_MAX / 2, 0, 0, 0);
		break;
	case TYPE_RANDOM:
		//オブジェクトの振動の方向
		shakeDir_ = XMVectorSet(rand() - RAND_MAX / 2, rand() - RAND_MAX / 2, rand() - RAND_MAX / 2, 0);
		break;
	}
	//オブジェクトの振動の方向にオブジェクトの振動の強さ分の長さを持たせる
	shakeDir_ = XMVector3Normalize(shakeDir_) * shakePower_ * SHAKEDEC;
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
