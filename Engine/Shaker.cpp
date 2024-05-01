#include "Shaker.h"

#include "Easing.h"

#include "../Timer.h"

//�U���̈ړ��ʂ𐧌�����l
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
	//�U���O�̃I�u�W�F�N�g�̈ʒu
	position_ = _position;
	pTimer_ = new Timer(shakeTime_);
	pEasing_ = new Easing();
	//�I�u�W�F�N�g�̐U���̕���
	shakeDir_ = XMVectorSet(rand(), rand(), rand(), 0);
	//�I�u�W�F�N�g�̐U���̕����ɃI�u�W�F�N�g�̐U���̋������̒�������������
	shakeDir_ = XMVector3Normalize(shakeDir_) * shakePower_ * SHAKEDEC;
}

void Shaker::ShakeUpdate()
{
	pTimer_->Update();
	//camMoveCount�̏����l
	const float moveCountInit = 0;
	//�J�����̓����𐧌䂷��J�E���g
	static float moveCount = moveCountInit;
	//camMoveCount�̖��t���[���������l
	const float moveCountPlus = 0.5f;
	moveCount += moveCountPlus;
	//prevPosition������vector
	XMVECTOR prevPosVec = XMLoadFloat3(position_);
	//retPos�ɒl�����
	float ease = pEasing_->EaseInSine(moveCount);
	XMStoreFloat3(position_, (prevPosVec * shakeDir_) * ease + prevPosVec);

	if (ease <= 0)
	{
		moveCount = moveCountInit;
		//�I�u�W�F�N�g�̐U���̕���
		shakeDir_ = XMVectorSet(rand() % 10, rand() % 10, rand() % 10, 0);
		//�I�u�W�F�N�g�̐U���̕����ɃI�u�W�F�N�g�̐U���̋������̒�������������
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
