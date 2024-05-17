#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>

#include "Shaker.h"

using namespace DirectX;

//-----------------------------------------------------------
//カメラ
//-----------------------------------------------------------
namespace Camera
{
	//初期化（プロジェクション行列作成）
	void Initialize();

	//更新（ビュー行列作成）
	void Update();

	//視点（カメラの位置）を設定
	void SetPosition(XMVECTOR position);
	void SetPosition(XMFLOAT3 position);

	XMVECTOR GetPosition();
	XMVECTOR GetRotate();

	//焦点（見る位置）を設定
	void SetTarget(XMVECTOR target);
	void SetTarget(XMFLOAT3 target);

	//ビュー行列を取得
	XMMATRIX GetViewMatrix();

	//プロジェクション行列を取得
	XMMATRIX GetProjectionMatrix();

	//ビルボード用回転行列を取得
	XMMATRIX GetBillboardMatrix();

	//カメラの振動
	//振動の初期化
	void ShakeInit(SHAKETYPE _shakeType, float _vibTime, float _vibPower);
	void CameraShake();

	// 球面線形補間関数
	// out   : 補間ベクトル（出力）
	// start : 開始ベクトル
	// end : 終了ベクトル
	// t : 補間値（0～1）
	XMVECTOR* SphereLinear(XMVECTOR* out, XMVECTOR* start, XMVECTOR* end, float t);
};