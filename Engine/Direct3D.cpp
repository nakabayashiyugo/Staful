#include "Direct3D.h"
#include <cassert>
#include <DirectXMath.h>
#include <vector>

#include "Image.h"

//変数
namespace Direct3D
{
	ID3D11Device* pDevice_ = nullptr;		//デバイス
	ID3D11DeviceContext* pContext_ = nullptr;		//デバイスコンテキスト
	IDXGISwapChain* pSwapChain_ = nullptr;		//スワップチェイン
	ID3D11RenderTargetView* pRenderTargetView_ = nullptr;	//レンダーターゲットビュー
    ID3D11Texture2D* pDepthStencil;			//深度ステンシル
    ID3D11DepthStencilView* pDepthStencilView;		//深度ステンシルビュー
    ID3D11DepthStencilState* pDepthStencilState[BLEND_MAX];

    D3D11_VIEWPORT vp;

    ID3D11BlendState* pBlendState[BLEND_MAX];

    struct SHADER_BUNDLE
    {
        ID3D11VertexShader* pVertexShader_ = nullptr;	//頂点シェーダー
        ID3D11PixelShader* pPixelShader_ = nullptr;		//ピクセルシェーダー
        ID3D11RasterizerState* pRasterizerState_ = nullptr;	//ラスタライザー
        ID3D11InputLayout* pVertexLayout_ = nullptr;	//頂点インプットレイアウト
    };
    SHADER_BUNDLE shaderBundle[SHADER_MAX];

    int aft_scrWidth, aft_scrHeight;
    int bfr_scrWidth, bfr_scrHeight;
}

//初期化
HRESULT Direct3D::Initialize(int winW, int winH, HWND hWnd)
{

    aft_scrWidth = winW;
    aft_scrHeight = winH;
    bfr_scrWidth = WINDOW_WIDTH;
    bfr_scrHeight = WINDOW_HEIGHT;

    ///////////////////////////いろいろ準備するための設定///////////////////////////////
    //いろいろな設定項目をまとめた構造体
    DXGI_SWAP_CHAIN_DESC scDesc;

    //とりあえず全部0
    ZeroMemory(&scDesc, sizeof(scDesc));

    //描画先のフォーマット
    scDesc.BufferDesc.Width = winW;		//画面幅
    scDesc.BufferDesc.Height = winH;	//画面高さ
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 何色使えるか

    //FPS（1/60秒に1回）
    scDesc.BufferDesc.RefreshRate.Numerator = 60;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;

    //その他
    scDesc.Windowed = TRUE;			//ウィンドウモードかフルスクリーンか
    scDesc.OutputWindow = hWnd;		//ウィンドウハンドル
    scDesc.BufferCount = 1;			//バックバッファの枚数
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//バックバッファの使い道＝画面に描画するために
    scDesc.SampleDesc.Count = 1;		//MSAA（アンチエイリアス）の設定
    scDesc.SampleDesc.Quality = 0;		//　〃

    ////////////////上記設定をもとにデバイス、コンテキスト、スワップチェインを作成////////////////////////
    D3D_FEATURE_LEVEL level;
    D3D11CreateDeviceAndSwapChain(
        nullptr,				// どのビデオアダプタを使用するか？既定ならばnullptrで
        D3D_DRIVER_TYPE_HARDWARE,		// ドライバのタイプを渡す。ふつうはHARDWARE
        nullptr,				// 上記をD3D_DRIVER_TYPE_SOFTWAREに設定しないかぎりnullptr
        0,					// 何らかのフラグを指定する。（デバッグ時はD3D11_CREATE_DEVICE_DEBUG？）
        nullptr,				// デバイス、コンテキストのレベルを設定。nullptrにしとけばOK
        0,					// 上の引数でレベルを何個指定したか
        D3D11_SDK_VERSION,			// SDKのバージョン。必ずこの値
        &scDesc,				// 上でいろいろ設定した構造体
        &pSwapChain_,				// 無事完成したSwapChainのアドレスが返ってくる
        &pDevice_,				// 無事完成したDeviceアドレスが返ってくる
        &level,					// 無事完成したDevice、Contextのレベルが返ってくる
        &pContext_);				// 無事完成したContextのアドレスが返ってくる

    ///////////////////////////レンダーターゲットビュー作成///////////////////////////////
    //スワップチェーンからバックバッファを取得（バックバッファ ＝ レンダーターゲット）
    ID3D11Texture2D* pBackBuffer;
    pSwapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    HRESULT hr;
    //レンダーターゲットビューを作成
    hr = pDevice_->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView_);
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "レンダーターゲットビューの作成に失敗しました", "エラー", MB_OK);
        return hr;
    }
    
    //一時的にバックバッファを取得しただけなので解放
    SAFE_RELEASE(pBackBuffer);

    ///////////////////////////ビューポート（描画範囲）設定///////////////////////////////
    //レンダリング結果を表示する範囲
    
    vp.Width = (float)winW;	//幅
    vp.Height = (float)winH;//高さ
    vp.MinDepth = 0.0f;	//手前
    vp.MaxDepth = 1.0f;	//奥
    vp.TopLeftX = 0;	//左
    vp.TopLeftY = 0;	//上


    //深度ステンシルビューの作成
    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = winW;
    descDepth.Height = winH;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    pDevice_->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
    pDevice_->CreateDepthStencilView(pDepthStencil, NULL, &pDepthStencilView);

    //深度テストを行う深度ステンシルステートの作成
    {
        //デフォルト
        D3D11_DEPTH_STENCIL_DESC desc = {};
        desc.DepthEnable = true;
        desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        desc.StencilEnable = true;
        pDevice_->CreateDepthStencilState(&desc, &pDepthStencilState[BLEND_DEFAULT]);
        pContext_->OMSetDepthStencilState(pDepthStencilState[BLEND_DEFAULT], 0);

        //加算合成用（書き込みなし）
        desc.StencilEnable = false;
        pDevice_->CreateDepthStencilState(&desc, &pDepthStencilState[BLEND_ADD]);
    }

    //ブレンドステート
    {
        //通常
        D3D11_BLEND_DESC BlendDesc;
        ZeroMemory(&BlendDesc, sizeof(BlendDesc));
        BlendDesc.AlphaToCoverageEnable = FALSE;
        BlendDesc.IndependentBlendEnable = FALSE;
        BlendDesc.RenderTarget[0].BlendEnable = TRUE;

        BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

        BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        pDevice_->CreateBlendState(&BlendDesc, &pBlendState[BLEND_DEFAULT]);
        float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
        pContext_->OMSetBlendState(pBlendState[BLEND_DEFAULT], blendFactor, 0xffffffff);

        //加算合成（重なるほど光って見える効果）
        BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
        pDevice_->CreateBlendState(&BlendDesc, &pBlendState[BLEND_ADD]);
    }
    
    //データを画面に描画するための一通りの設定（パイプライン）
    pContext_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // データの入力種類を指定
    pContext_->OMSetRenderTargets(1, &pRenderTargetView_, pDepthStencilView);            // 描画先を設定
    pContext_->RSSetViewports(1, &vp);

    //シェーダー準備
    hr = InitShader();
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "シェーダーの呼び出しに失敗しました", "エラー", MB_OK);
        return hr;
    }
    return S_OK;
}

void Direct3D::SetBlendMode(BLEND_MODE blendMode)
{
    //加算合成
    float blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
    pContext_->OMSetBlendState(pBlendState[blendMode], blendFactor, 0xffffffff);

    //Zバッファへの書き込み
    pContext_->OMSetDepthStencilState(pDepthStencilState[blendMode], 0);
}

//描画開始
void Direct3D::BeginDraw()
{
    //背景の色
    float clearColor[4] = { 0.8f, 1.0f, 0.8f, 1.0f };//R,G,B,A
    pContext_->ClearRenderTargetView(pRenderTargetView_, clearColor);

    Camera::Update();

    //深度バッファクリア
    pContext_->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//描画終了
void Direct3D::EndDraw()
{
    Image::FrontHandlesDraw();

    //スワップ（バックバッファを表に表示する）
    pSwapChain_->Present(0, 0);
}

void Direct3D::SetShader(SHADER_TYPE type)
{
    //それぞれをデバイスコンテキストにセット
    pContext_->VSSetShader(shaderBundle[type].pVertexShader_, NULL, 0);	//頂点シェーダー
    pContext_->PSSetShader(shaderBundle[type].pPixelShader_, NULL, 0);	//ピクセルシェーダー
    pContext_->IASetInputLayout(shaderBundle[type].pVertexLayout_);	//頂点インプットレイアウト
    pContext_->RSSetState(shaderBundle[type].pRasterizerState_);		//ラスタライザー
}

//解放処理
void Direct3D::Release()
{
    for (int i = 0; i < SHADER_MAX; i++)
    {
        SAFE_RELEASE(shaderBundle[i].pRasterizerState_);
        SAFE_RELEASE(shaderBundle[i].pVertexLayout_);
        SAFE_RELEASE(shaderBundle[i].pPixelShader_);
        SAFE_RELEASE(shaderBundle[i].pVertexShader_);
    }
    

    SAFE_RELEASE(pRenderTargetView_);
    SAFE_RELEASE(pSwapChain_);
    SAFE_RELEASE(pContext_);
    SAFE_RELEASE(pDevice_);
}

//シェーダー準備
HRESULT Direct3D::InitShader()
{
    HRESULT hr;

    hr = InitShader3D();
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "3Dシェーダの作成に失敗しました", "エラー", MB_OK);
        return hr;
    }

    hr = InitShader2D();
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "2Dシェーダの作成に失敗しました", "エラー", MB_OK);
        return hr;
    }

    hr = InitShaderBillBoard();
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "BillBoardシェーダの作成に失敗しました", "エラー", MB_OK);
    }

    return S_OK;
}

HRESULT Direct3D::InitShader3D()
{
    HRESULT hr;
    // 頂点シェーダの作成（コンパイル）
    ID3DBlob* pCompileVS = nullptr;
    D3DCompileFromFile(L"Shader/Simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
    hr = pDevice_->CreateVertexShader(pCompileVS->GetBufferPointer(), 
        pCompileVS->GetBufferSize(), NULL, &shaderBundle[SHADER_3D].pVertexShader_);
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "頂点バッファのコンパイルに失敗しました", "エラー", MB_OK);
        return hr;
    }

    //頂点インプットレイアウト
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//位置
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::XMVECTOR) , D3D11_INPUT_PER_VERTEX_DATA, 0 },//UV座標
        { "NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(DirectX::XMVECTOR) * 2 ,	D3D11_INPUT_PER_VERTEX_DATA, 0 },//法線
    };
    hr = pDevice_->CreateInputLayout(layout, 3, pCompileVS->GetBufferPointer(), 
        pCompileVS->GetBufferSize(), &shaderBundle[SHADER_3D].pVertexLayout_);
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "頂点インプットレイアウトの設定に失敗しました", "エラー", MB_OK);
        return hr;
    }
    
    SAFE_RELEASE(pCompileVS);

    // ピクセルシェーダの作成（コンパイル）
    ID3DBlob* pCompilePS = nullptr;
    D3DCompileFromFile(L"Shader/Simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
    hr = pDevice_->CreatePixelShader(pCompilePS->GetBufferPointer(), 
        pCompilePS->GetBufferSize(), NULL, &shaderBundle[SHADER_3D].pPixelShader_);
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "ピクセルシェーダのコンパイルに失敗しました", "エラー", MB_OK);
        return hr;
    }
    SAFE_RELEASE(pCompilePS);

    //ラスタライザ作成
    D3D11_RASTERIZER_DESC rdc = {};
    rdc.CullMode = D3D11_CULL_BACK;
    rdc.FillMode = D3D11_FILL_SOLID;
    rdc.FrontCounterClockwise = FALSE;
    hr = pDevice_->CreateRasterizerState(&rdc, &shaderBundle[SHADER_3D].pRasterizerState_);
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "ラスタライザの作成に失敗しました", "エラー", MB_OK);
        return hr;
    }

    return S_OK;

}

HRESULT Direct3D::InitShader2D()
{
    HRESULT hr;

    //2D用
    // 頂点シェーダの作成（コンパイル）
    ID3DBlob* pCompileVS = nullptr;
    D3DCompileFromFile(L"Shader/Simple2D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
    hr = pDevice_->CreateVertexShader(pCompileVS->GetBufferPointer(), 
        pCompileVS->GetBufferSize(), NULL, &shaderBundle[SHADER_2D].pVertexShader_);
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "頂点バッファのコンパイルに失敗しました", "エラー", MB_OK);
        return hr;
    }
    
    //頂点インプットレイアウト
    std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },	//位置
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::XMVECTOR) , D3D11_INPUT_PER_VERTEX_DATA, 0 },//UV座標
    };
    //hr = pDevice_->CreateInputLayout(layout, sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout_);
    hr = pDevice_->CreateInputLayout(layout.data(), layout.size(), pCompileVS->GetBufferPointer(), 
        pCompileVS->GetBufferSize(), &shaderBundle[SHADER_2D].pVertexLayout_);
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "頂点インプットレイアウトの設定に失敗しました", "エラー", MB_OK);
        return hr;
    }
    
    SAFE_RELEASE(pCompileVS);
    
    // ピクセルシェーダの作成（コンパイル）
    ID3DBlob* pCompilePS = nullptr;
    D3DCompileFromFile(L"Shader/Simple2D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
    hr = pDevice_->CreatePixelShader(pCompilePS->GetBufferPointer(), 
        pCompilePS->GetBufferSize(), NULL, &shaderBundle[SHADER_2D].pPixelShader_);
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "ピクセルシェーダのコンパイルに失敗しました", "エラー", MB_OK);
        return hr;
    }
    SAFE_RELEASE(pCompilePS);
    
    //ラスタライザ作成
    D3D11_RASTERIZER_DESC rdc = {};
    rdc.CullMode = D3D11_CULL_BACK;
    rdc.FillMode = D3D11_FILL_SOLID;
    rdc.FrontCounterClockwise = FALSE;
    hr = pDevice_->CreateRasterizerState(&rdc, &shaderBundle[SHADER_2D].pRasterizerState_);
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "ラスタライザの作成に失敗しました", "エラー", MB_OK);
        return hr;
    }

    //それぞれをデバイスコンテキストにセット
    pContext_->VSSetShader(shaderBundle[SHADER_2D].pVertexShader_, NULL, 0);	//頂点シェーダー
    pContext_->PSSetShader(shaderBundle[SHADER_2D].pPixelShader_, NULL, 0);	//ピクセルシェーダー
    pContext_->IASetInputLayout(shaderBundle[SHADER_2D].pVertexLayout_);	//頂点インプットレイアウト
    pContext_->RSSetState(shaderBundle[SHADER_2D].pRasterizerState_);		//ラスタライザー

    return S_OK;
}

HRESULT Direct3D::InitShaderBillBoard()
{
    DWORD vectorSize = sizeof(XMFLOAT3);

    HRESULT hr;

    // 頂点シェーダの作成（コンパイル）
    ID3DBlob* pCompileVS = NULL;
    D3DCompileFromFile(L"Shader/BillBoard.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
    hr = pDevice_->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaderBundle[SHADER_BILLBOARD].pVertexShader_);
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "頂点シェーダの作成に失敗しました", "エラー", MB_OK);
        return hr;
    }

    // ピクセルシェーダの作成（コンパイル）
    ID3DBlob* pCompilePS = NULL;
    D3DCompileFromFile(L"Shader/BillBoard.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
    hr = pDevice_->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderBundle[SHADER_BILLBOARD].pPixelShader_);
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "ピクセルシェーダの作成に失敗しました", "エラー", MB_OK);
        return hr;
    }

    // 頂点レイアウトの作成（1頂点の情報が何のデータをどんな順番で持っているか）
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, vectorSize * 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, vectorSize * 1, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    hr = pDevice_->CreateInputLayout(layout, 2, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderBundle[SHADER_BILLBOARD].pVertexLayout_);
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "頂点レイアウトの作成に失敗しました", "エラー", MB_OK);
        return hr;
    }

    //シェーダーが無事作成できたので、コンパイルしたやつはいらない
    pCompileVS->Release();
    pCompilePS->Release();

    //ラスタライザ作成
    D3D11_RASTERIZER_DESC rdc = {};
    rdc.CullMode = D3D11_CULL_NONE;
    rdc.FillMode = D3D11_FILL_SOLID;
    rdc.FrontCounterClockwise = TRUE;
    hr = pDevice_->CreateRasterizerState(&rdc, &shaderBundle[SHADER_BILLBOARD].pRasterizerState_);
    if (FAILED(hr))
    {
        //失敗したときの処理
        MessageBox(nullptr, "ラスタライザの作成に失敗しました", "エラー", MB_OK);
        return hr;
    }


    //それぞれをデバイスコンテキストにセット
    pContext_->VSSetShader(shaderBundle[SHADER_BILLBOARD].pVertexShader_, NULL, 0);	//頂点シェーダー
    pContext_->PSSetShader(shaderBundle[SHADER_BILLBOARD].pPixelShader_, NULL, 0);	//ピクセルシェーダー
    pContext_->IASetInputLayout(shaderBundle[SHADER_BILLBOARD].pVertexLayout_);	//頂点インプットレイアウト
    pContext_->RSSetState(shaderBundle[SHADER_BILLBOARD].pRasterizerState_);		//ラスタライザー

    return S_OK;
}

//Zバッファへの書き込みON/OFF
void Direct3D::SetDepthBafferWriteEnable(bool isWrite)
{
    //ON
    if (isWrite)
    {
        //Zバッファ（デプスステンシルを指定する）
        pContext_->OMSetRenderTargets(1, &pRenderTargetView_, pDepthStencilView);
    }

    //OFF
    else
    {
        pContext_->OMSetRenderTargets(1, &pRenderTargetView_, nullptr);
    }
}
