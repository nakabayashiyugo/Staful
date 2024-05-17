#include "Camera.h"

#include "Easing.h"

#include "../Timer.h"

namespace Camera
{
	//�ϐ�
	XMVECTOR position_;			//�J�����̈ʒu�i���_�j
	XMVECTOR target_;			//����ʒu�i�œ_�j
	XMMATRIX viewMatrix_;		//�r���[�s��
	XMMATRIX projMatrix_;		//�v���W�F�N�V�����s��
	XMMATRIX billBoard_;		//�r���{�[�h�p�s��

	//�U���p
	Shaker* camShaker_;			//�J�����̐U���p
	XMFLOAT3 shakePos_;			//�J�����U���p��position;
	XMFLOAT3 dirCamToTarget_;	//�J�����ƃJ�����̌��Ă�I�u�W�F�N�g�̈ʒu�̋���
	XMFLOAT3 shakeDir_;			//�J�����̐U���̕���
}

//������
void Camera::Initialize()
{
	position_ = XMVectorSet(0, 3, -10, 0);	//�J�����̈ʒu
	target_ = XMVectorSet(0, 0, 0, 0);	//�J�����̏œ_

	//�v���W�F�N�V�����s��
	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)800.0 / (FLOAT)600.0, 0.1f, 100.0f);
	camShaker_ = new Shaker();
	shakeDir_ = XMFLOAT3(1.0f, 1.0f, 1.0f);
}

//�X�V
void Camera::Update()
{
	//�r���[�s��̍쐬
	viewMatrix_ = XMMatrixLookAtLH(position_, target_, XMVectorSet(0, 1, 0, 0));

	//�r���{�[�h�s��
	//�i��ɃJ�����̕��������悤�ɉ�]������s��B�p�[�e�B�N���ł����g��Ȃ��j
	billBoard_ = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), target_ - position_, XMVectorSet(0, 1, 0, 0));
	billBoard_ = XMMatrixInverse(nullptr, billBoard_);
}

//�ʒu��ݒ�
void Camera::SetPosition(XMVECTOR position)
{
	position_ = position;
}
void Camera::SetPosition(XMFLOAT3 position)
{
	SetPosition(XMLoadFloat3(&position));
}

XMVECTOR Camera::GetPosition()
{
	return position_;
}

XMVECTOR Camera::GetRotate()
{
	return XMVECTOR();
}

//�œ_��ݒ�
void Camera::SetTarget(XMVECTOR target)
{
	target_ = target;
}
void Camera::SetTarget(XMFLOAT3 target)
{
	SetTarget(XMLoadFloat3(&target));
}

//�r���[�s����擾
XMMATRIX Camera::GetViewMatrix()
{
	return viewMatrix_;
}

//�v���W�F�N�V�����s����擾
XMMATRIX Camera::GetProjectionMatrix()
{
	return projMatrix_;
}

//�r���{�[�h�p��]�s����擾
XMMATRIX Camera::GetBillboardMatrix() 
{ 
	return billBoard_; 
}

void Camera::ShakeInit(SHAKETYPE _shakeType, float _vibTime, float _vibPower)
{
	XMStoreFloat3(&shakePos_, position_);
	camShaker_->ShakeInit(&shakePos_, _shakeType, _vibTime, _vibPower);
	camShaker_->SetIsShake(true);
	XMStoreFloat3(&dirCamToTarget_, position_ - target_);
}

void Camera::CameraShake()
{
	if (camShaker_->GetIsShake())
	{
		//�J�����U��
		camShaker_->ShakeUpdate();
		float cnt = camShaker_->GetMoveCount();
		SphereLinear(&position_, &position_, )
		//�J�����U������target�̈ʒu
		XMFLOAT3 shakeTarget = XMFLOAT3(shakePos_.x - dirCamToTarget_.x,
			shakePos_.y - dirCamToTarget_.y,
			shakePos_.z - dirCamToTarget_.z);
		target_ = XMLoadFloat3(&shakeTarget);
		

		position_ = XMLoadFloat3(&shakePos_);
	}
	
}

XMVECTOR* Camera::SphereLinear(XMVECTOR* out, XMVECTOR* start, XMVECTOR* end, float t)
{
	XMVECTOR s, e;
	s = XMVector3Normalize(*start);
	e = XMVector3Normalize(*end);

	// 2�x�N�g���Ԃ̊p�x�i�s�p���j
	float angle = acos(XMVectorGetX(XMVector3Dot(s, e)));
	// sin��
	float SinTh = sin(angle);
	// ��ԌW��
	float Ps = sin(angle * (1 - t));
	float Pe = sin(angle * t);

	*out = (Ps * s + Pe * e) / SinTh;

	// �ꉞ���K�����ċ��ʐ��`��Ԃ�
	*out = XMVector3Normalize(*out);

	return out;
}