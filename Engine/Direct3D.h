#pragma once
//�C���N���[�h
#include <d3dcompiler.h>
#include <d3d11.h>
#include "Camera.h"

//�����J
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define SAFE_DELETE(p) if(p != nullptr){ delete p; p = nullptr;}
#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

const int WINDOW_WIDTH = 1200;  //�E�B���h�E�̕�
const int WINDOW_HEIGHT = 800; //�E�B���h�E�̍���

namespace Direct3D
{
	enum SHADER_TYPE
	{
		SHADER_2D,
		SHADER_3D,
		SHADER_BILLBOARD,
		SHADER_MAX,
	};

	extern ID3D11Device* pDevice_;				//�f�o�C�X
	extern ID3D11DeviceContext* pContext_;		//�f�o�C�X�R���e�L�X�g
	extern int aft_scrWidth, aft_scrHeight;		//�ύX��̃E�B���h�E�T�C�Y
	extern int bfr_scrWidth, bfr_scrHeight;		//�ύX�O�̃E�B���h�E�T�C�Y

	//���u�����h���[�h
	enum BLEND_MODE
	{
		BLEND_DEFAULT, BLEND_ADD, BLEND_MAX
	};

	//������
	HRESULT Initialize(int winW, int winH, HWND hWnd);

	//�V�F�[�_�[����
	HRESULT InitShader();
	HRESULT InitShader3D();
	HRESULT InitShader2D();
	HRESULT InitShaderBillBoard();

	void SetShader(SHADER_TYPE type);

	//�u�����h���[�h�̕ύX
	//�����FblendMode	BLEND_DEFAULT	�ʏ�
	//					BLEND_ADD		���Z�����i�p�[�e�B�N���p�j
	void SetBlendMode(BLEND_MODE blendMode);

	//�`��J�n
	void BeginDraw();

	//�`��I��
	void EndDraw();

	//���
	void Release();
};

