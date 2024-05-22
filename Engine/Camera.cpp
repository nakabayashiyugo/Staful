#include "Camera.h"

#include "Easing.h"

#include "../Timer.h"

namespace Camera
{
	//変数
	XMVECTOR position_;			//カメラの位置（視点）
	XMVECTOR target_;			//見る位置（焦点）
	XMMATRIX viewMatrix_;		//ビュー行列
	XMMATRIX projMatrix_;		//プロジェクション行列
	XMMATRIX billBoard_;		//ビルボード用行列

	//振動用
	Shaker* camShaker_;			//カメラの振動用
	XMFLOAT3 shakePos_;			//カメラ振動用のposition;
	XMFLOAT3 dirCamToTarget_;	//カメラとカメラの見てるオブジェクトの位置の距離
	XMFLOAT3 shakeDir_;			//カメラの振動の方向
}

//初期化
void Camera::Initialize()
{
	position_ = XMVectorSet(0, 3, -10, 0);	//カメラの位置
	target_ = XMVectorSet(0, 0, 0, 0);	//カメラの焦点

	camShaker_ = new Shaker();
	shakeDir_ = XMFLOAT3(1.0f, 1.0f, 1.0f);

	//プロジェクション行列
	projMatrix_ = XMMatrixPerspectiveFovLH(XM_PIDIV4, (FLOAT)800.0 / (FLOAT)600.0, 0.1f, 100.0f);
	
}

//更新
void Camera::Update()
{
	//ビュー行列の作成
	viewMatrix_ = XMMatrixLookAtLH(position_, target_, XMVectorSet(0, 1, 0, 0));

	//ビルボード行列
	//（常にカメラの方を向くように回転させる行列。パーティクルでしか使わない）
	billBoard_ = XMMatrixLookAtLH(XMVectorSet(0, 0, 0, 0), target_ - position_, XMVectorSet(0, 1, 0, 0));
	billBoard_ = XMMatrixInverse(nullptr, billBoard_);

	//カメラの位置からカメラの見てるターゲットの位置までのベクトル
	XMVECTOR dir = target_ - position_;
	XMStoreFloat3(&dirCamToTarget_, dir);
}

//位置を設定
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

//焦点を設定
void Camera::SetTarget(XMVECTOR target)
{
	target_ = target;
}
void Camera::SetTarget(XMFLOAT3 target)
{
	SetTarget(XMLoadFloat3(&target));
}

//ビュー行列を取得
XMMATRIX Camera::GetViewMatrix()
{
	return viewMatrix_;
}

//プロジェクション行列を取得
XMMATRIX Camera::GetProjectionMatrix()
{
	return projMatrix_;
}

//ビルボード用回転行列を取得
XMMATRIX Camera::GetBillboardMatrix() 
{ 
	return billBoard_; 
}

void Camera::ShakeInit(SHAKETYPE _shakeType, float _vibTime, float _vibPower)
{
	//カメラの振動の方向決め
	switch (_shakeType)
	{
	case TYPE_BESIDE:
		camShaker_->SetShakeType(TYPE_BESIDE);
		break;
	case TYPE_VERTICAL:
		camShaker_->SetShakeType(TYPE_BASIC);
		//上のベクトルの90度回転させたベクトル
		XMVECTOR rotateVec = XMVectorSet(dirCamToTarget_.z, dirCamToTarget_.y, dirCamToTarget_.x, 0);
		//二つのベクトルの外積をとる
		XMVECTOR cross = XMVector3Cross(XMLoadFloat3(&dirCamToTarget_), rotateVec);
		cross = XMVectorSet(0, XMVectorGetY(cross), XMVectorGetZ(cross), 0);
		//このベクトルを振動の軸とする
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
		//カメラ振動
		camShaker_->ShakeUpdate();
		float cnt = camShaker_->GetMoveCount();
		//カメラ振動中のtargetの位置
		XMFLOAT3 shakeTarget = XMFLOAT3(shakePos_.x + dirCamToTarget_.x,
			shakePos_.y + dirCamToTarget_.y,
			shakePos_.z + dirCamToTarget_.z);
		target_ = XMLoadFloat3(&shakeTarget);
		

		position_ = XMLoadFloat3(&shakePos_);
	}
	
}