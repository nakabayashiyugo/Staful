#include "Sprite.h"

Sprite::Sprite()
	:vertexNum_(0), pVertexBuffer_(nullptr), indexNum_(0), pIndexBuffer_(nullptr), pConstantBuffer_(nullptr),
	texture_size_(0, 0, 0)
{
}

Sprite::~Sprite()
{
	Release();
}

HRESULT Sprite::Load(std::string filename)
{
	HRESULT hr;
	pTexture_ = new Texture;
	hr = pTexture_->Load(filename);
	texture_size_ = pTexture_->GetSize();
	if (FAILED(hr))
	{
		//エラー処理
		MessageBox(nullptr, "テクスチャのロードに失敗しました", "エラー", MB_OK);
		return hr;
	}
	InitVertexData();
	hr = CreateVertexBuffer();
	if (FAILED(hr))
	{
		//エラー処理
		MessageBox(nullptr, "頂点バッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
	InitIndexData();
	hr = CreateIndexBuffer();
	if (FAILED(hr))
	{
		//エラー処理
		MessageBox(nullptr, "インデックスバッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}
	hr = CreateConstantBuffer();
	if (FAILED(hr))
	{
		//エラー処理
		MessageBox(nullptr, "コンスタントバッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

void Sprite::Draw(Transform& transform, RECT rect, float alpha, XMFLOAT3 color)
{
	Direct3D::SetShader(Direct3D::SHADER_2D);
	transform.Calclation();//トランスフォームを計算

	SetBufferToPipeline();

	PassDataToCB(transform, rect, alpha, color);
}

void Sprite::Release()
{
	SAFE_RELEASE(pTexture_);
	SAFE_DELETE(pTexture_);

	SAFE_RELEASE(pConstantBuffer_);
	SAFE_RELEASE(pIndexBuffer_);
	SAFE_RELEASE(pVertexBuffer_);
}

void Sprite::InitVertexData()
{
	vertices_ = {
	{ XMVectorSet(-1.0f, 1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f) },	// 四角形の頂点（左上）
	{ XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f) }, // 四角形の頂点（右上）
	{ XMVectorSet(1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(1.0f, 1.0f, 0.0f, 0.0f) }, // 四角形の頂点（右下）
	{ XMVectorSet(-1.0f, -1.0f, 0.0f, 0.0f), XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) }  // 四角形の頂点（左下）
	};
	vertexNum_ = vertices_.size();

}

HRESULT Sprite::CreateVertexBuffer()
{
	HRESULT hr;
	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(vertices_[0]) * vertexNum_;
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = &vertices_[0];
	hr = Direct3D::pDevice_->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);
	if (FAILED(hr))
	{
		//エラー処理
		MessageBox(nullptr, "頂点バッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

void Sprite::InitIndexData()
{
	//インデックス情報
	index_ = { 0,1,2, 0,2,3 };
	indexNum_ = index_.size();
}

HRESULT Sprite::CreateIndexBuffer()
{
	HRESULT hr;
	// インデックスバッファを生成する
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(index_);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &index_[0];
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	hr = Direct3D::pDevice_->CreateBuffer(&bd, &InitData, &pIndexBuffer_);
	if (FAILED(hr))
	{
		//エラー処理
		MessageBox(nullptr, "インデックスバッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT Sprite::CreateConstantBuffer()
{
	HRESULT hr;
	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	hr = Direct3D::pDevice_->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
	if (FAILED(hr))
	{
		//エラー処理
		MessageBox(nullptr, "コンスタントバッファの作成に失敗しました", "エラー", MB_OK);
		return hr;
	}

	return S_OK;
}

void Sprite::PassDataToCB(Transform& transform, RECT rect, float alpha, XMFLOAT3 color)
{
	//コンスタントバッファに渡す情報
	CONSTANT_BUFFER cb;
	D3D11_MAPPED_SUBRESOURCE pdata;

	XMMATRIX cut = XMMatrixScaling((float)rect.right, (float)rect.bottom, 1);
	XMMATRIX view = XMMatrixScaling(1.0f / Direct3D::bfr_scrWidth, 1.0f / Direct3D::bfr_scrHeight, 1.0f);

	XMMATRIX world = cut * transform.GetMatScale() * transform.GetMatRotate() * view * transform.GetMatTranslate();
	cb.matW = XMMatrixTranspose(world);

	XMMATRIX mTexTrans = XMMatrixTranslation((float)rect.left / (float)pTexture_->GetSize().x,
		(float)rect.top / (float)pTexture_->GetSize().y, 0.0f);
	XMMATRIX mTexScale = XMMatrixScaling((float)rect.right / (float)pTexture_->GetSize().x,
		(float)rect.bottom / (float)pTexture_->GetSize().y, 1.0f);
	XMMATRIX mTexel = mTexScale * mTexTrans;
	cb.uvTrans = XMMatrixTranspose(mTexel);

	cb.color = XMFLOAT4(color.x, color.y, color.z, alpha);

	Direct3D::pContext_->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る

	ID3D11SamplerState* pSampler = pTexture_->GetSampler();
	Direct3D::pContext_->PSSetSamplers(0, 1, &pSampler);

	ID3D11ShaderResourceView* pSRV = pTexture_->GetSRV();
	Direct3D::pContext_->PSSetShaderResources(0, 1, &pSRV);
	Direct3D::pContext_->Unmap(pConstantBuffer_, 0);	//再開

	Direct3D::pContext_->DrawIndexed(indexNum_, 0, 0);
}

void Sprite::SetBufferToPipeline()
{
	//頂点バッファ
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	Direct3D::pContext_->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext_->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext_->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext_->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

	
}
