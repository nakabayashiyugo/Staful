#include "EyeCatching.h"
#include "../../../Timer.h"

//マップ制作中の表示が変わる時間
const float makingDrawChangeTime = 0.3f;
//マップ制作中の「.」の間隔
const float makingPointInterval = 0.05f;
//マップ制作中の「.」の最大数
const int makingPointNumMax = 3;

EyeCatching::EyeCatching(GameObject* _parent)
	:GameObject(_parent, "EyeCatching"),
	hDarken_(-1),
	hMaking_(-1), 
	hMakingPoint_(-1),
	makingPointNum_(0)
{
}

void EyeCatching::Initialize()
{
	EnemyMapEditMakingInit();
	makingPointTimer_ = new Timer(makingDrawChangeTime);
}

void EyeCatching::Update()
{
	makingPointTimer_->Update();
}

void EyeCatching::Draw()
{
	if (makingPointTimer_->isTimeUpped())
	{
		makingPointTimer_->TimeReset();
		makingPointNum_++;
		if (makingPointNum_ > makingPointNumMax)
		{
			makingPointNum_ = 0;
		}

	}
	EnemyMapEditMakingDraw();
}

void EyeCatching::Release()
{
}

void EyeCatching::EnemyMapEditMakingInit()
{
	std::string folderName = "Assets\\EnemyMapEdit\\";
	//マップ制作中に写す画像のロード
	hDarken_ = Image::Load(folderName + "EnemyMapEdit.png");
	assert(hDarken_ >= 0);
	//「マップ制作中」
	hMaking_ = Image::Load(folderName + "EnemyMapEdit_Making.png");
	assert(hMaking_ >= 0);
	hMakingPoint_ = Image::Load(folderName + "EnemyMapEdit_Making_Point.png");
	assert(hMakingPoint_ >= 0);
}

void EyeCatching::EnemyMapEditMakingDraw()
{
	//マップ製作中に写す画像のサイズ
	XMFLOAT3 texSize = Image::GetTextureSize(hDarken_);
	//マップ製作中に写す画像のトランスフォーム
	Transform tPict;
	tPict.scale_ = XMFLOAT3(Direct3D::bfr_scrWidth / texSize.y,
		Direct3D::bfr_scrHeight / texSize.x, 1);
	Image::SetTransform(hDarken_, tPict);
	//一番前に描画する
	Image::Draw(hDarken_);


	//「マップ制作中」のテクスチャサイズ
	XMFLOAT3 makTexSize = Image::GetTextureSize(hMaking_);

	Transform tMaking;
	std::vector<Transform> tMakPoint(makingPointNum_);
	if (!tMakPoint.empty())
	{
		Transform pointBase;
		pointBase.position_ = XMFLOAT3(makTexSize.x / Direct3D::bfr_scrWidth * tMaking.scale_.x - makingPointInterval
			, 0, 0);
		pointBase.scale_ = tMaking.scale_;
		for (int i = 0; i < tMakPoint.size(); i++)
		{
			tMakPoint[i] = pointBase;
			tMakPoint[i].position_.x += makingPointInterval * i;
		}
	}


	Image::SetTransform(hMaking_, tMaking);
	Image::Draw(hMaking_);
	for (int i = 0; i < tMakPoint.size(); i++)
	{
		Image::SetTransform(hMakingPoint_, tMakPoint[i]);
		Image::Draw(hMakingPoint_);
	}
}
