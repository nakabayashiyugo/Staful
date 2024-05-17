#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>

#include "Shaker.h"

using namespace DirectX;

//-----------------------------------------------------------
//�J����
//-----------------------------------------------------------
namespace Camera
{
	//�������i�v���W�F�N�V�����s��쐬�j
	void Initialize();

	//�X�V�i�r���[�s��쐬�j
	void Update();

	//���_�i�J�����̈ʒu�j��ݒ�
	void SetPosition(XMVECTOR position);
	void SetPosition(XMFLOAT3 position);

	XMVECTOR GetPosition();
	XMVECTOR GetRotate();

	//�œ_�i����ʒu�j��ݒ�
	void SetTarget(XMVECTOR target);
	void SetTarget(XMFLOAT3 target);

	//�r���[�s����擾
	XMMATRIX GetViewMatrix();

	//�v���W�F�N�V�����s����擾
	XMMATRIX GetProjectionMatrix();

	//�r���{�[�h�p��]�s����擾
	XMMATRIX GetBillboardMatrix();

	//�J�����̐U��
	//�U���̏�����
	void ShakeInit(SHAKETYPE _shakeType, float _vibTime, float _vibPower);
	void CameraShake();

	// ���ʐ��`��Ԋ֐�
	// out   : ��ԃx�N�g���i�o�́j
	// start : �J�n�x�N�g��
	// end : �I���x�N�g��
	// t : ��Ԓl�i0�`1�j
	XMVECTOR* SphereLinear(XMVECTOR* out, XMVECTOR* start, XMVECTOR* end, float t);
};