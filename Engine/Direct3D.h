#pragma once
//インクルード
#include <d3dcompiler.h>
#include <d3d11.h>
#include "Camera.h"

//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

const int WINDOW_WIDTH = 1200;  //ウィンドウの幅
const int WINDOW_HEIGHT = 800; //ウィンドウの高さ

namespace Direct3D
{
	enum SHADER_TYPE
	{
		SHADER_2D,
		SHADER_3D,
		SHADER_BILLBOARD,
		SHADER_MAX,
	};

	extern ID3D11Device* pDevice_;				//デバイス
	extern ID3D11DeviceContext* pContext_;		//デバイスコンテキスト
	extern int aft_scrWidth, aft_scrHeight;		//変更後のウィンドウサイズ
	extern int bfr_scrWidth, bfr_scrHeight;		//変更前のウィンドウサイズ

	//■ブレンドモード
	enum BLEND_MODE
	{
		BLEND_DEFAULT, BLEND_ADD, BLEND_MAX
	};

	//初期化
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//シェーダー準備
	HRESULT InitShader();
	HRESULT InitShader3D();
	HRESULT InitShader2D();
	HRESULT InitShaderBillBoard();

	void SetShader(SHADER_TYPE type);

	//ブレンドモードの変更
	//引数：blendMode	BLEND_DEFAULT	通常
	//					BLEND_ADD		加算合成（パーティクル用）
	void SetBlendMode(BLEND_MODE blendMode);

	//描画開始
	void BeginDraw();

	//描画終了
	void EndDraw();

	//解放
	void Release();
};

