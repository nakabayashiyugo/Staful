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

	camShaker_ = new Shaker();
	shakeDir_ = XMFLOAT3(1.0f, 1.0f, 1.0f);

	//�v���W�F�N�V�����s��
	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)800.0 / (FLOAT)600.0, 0.1f, 100.0f);
	
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

	//�J�����̈ʒu����J�����̌��Ă�^�[�Q�b�g�̈ʒu�܂ł̃x�N�g��
	XMVECTOR dir = target_ - position_;
	XMStoreFloat3(&dirCamToTarget_, dir);
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
	//�J�����̐U���̕�������
	switch (_shakeType)
	{
	case TYPE_BESIDE:
		camShaker_->SetShakeType(TYPE_BESIDE);
		break;
	case TYPE_VERTICAL:
		camShaker_->SetShakeType(TYPE_BASIC);
		//��̃x�N�g����90�x��]�������x�N�g��
		XMVECTOR rotateVec = XMVectorSet(dirCamToTarget_.z, dirCamToTarget_.y, dirCamToTarget_.x, 0);
		//��̃x�N�g���̊O�ς��Ƃ�
		XMVECTOR cross = XMVector3Cross(XMLoadFloat3(&dirCamToTarget_), rotateVec);
		cross = XMVectorSet(0, XMVectorGetY(cross), XMVectorGetZ(cross), 0);
		//���̃x�N�g����U���̎��Ƃ���
		camShaker_->SetShaft(cross);
	}

	XMStoreFloat3(&shakePos_, position_);
	camShaker_->ShakeInit(&shakePos_, _vibTime, _vibPower);
	camShaker_->SetIsShake(true);
	
}

void Camera::CameraShake()
{
	if (camShaker_->GetIsShake())
	{
		//�J�����U��
		camShaker_->ShakeUpdate();
		float cnt = camShaker_->GetMoveCount();
		//�J�����U������target�̈ʒu
		XMFLOAT3 shakeTarget = XMFLOAT3(shakePos_.x + dirCamToTarget_.x,
			shakePos_.y + dirCamToTarget_.y,
			shakePos_.z + dirCamToTarget_.z);
		target_ = XMLoadFloat3(&shakeTarget);
		

		position_ = XMLoadFloat3(&shakePos_);
	}
	
}