#include "Image.h"

namespace Image
{
	struct ImageData
	{
		Sprite* pSprite_;
		Transform transform_;
		std::string fileName_;
		RECT rect_;
		float alpha_;
		XMFLOAT3 color_;
		XMFLOAT3 size_;

		//コンストラクタ
		ImageData() : pSprite_(nullptr)
		{
			fileName_ = "";
			alpha_ = 1.0f;
			pSprite_ = nullptr;
			color_ = XMFLOAT3(1, 1, 1);
		}
	};

	//モデルのポインタを入れておくポインタ
	std::vector<ImageData*> imageList;
}

void Image::SetTransform(int hImage, Transform transform)
{
	imageList[hImage]->transform_ = transform;
}

int Image::Load(std::string filename)
{
	ImageData* pData = new ImageData();
	pData->fileName_ = filename;
	pData->pSprite_ = nullptr;
	for (auto itr = imageList.begin(); itr != imageList.end(); itr++)
	{
		if ((*itr)->fileName_ == filename)
		{
			pData->pSprite_ = (*itr)->pSprite_;
			break;
		}
	}
	if (pData->pSprite_ == nullptr)
	{
		pData->pSprite_ = new Sprite();
		pData->pSprite_->Load(filename);
	}
	pData->size_ = pData->pSprite_->GetTextureSize();
	imageList.push_back(pData);
	int handle = imageList.size() - 1;
	ResetRect(handle);
	return handle;
}

void Image::Draw(int hImage)
{
	imageList[hImage]->pSprite_->
		Draw(imageList[hImage]->transform_, imageList[hImage]->rect_, imageList[hImage]->alpha_, imageList[hImage]->color_);
}

void Image::Release(int hImage)
{
	if (hImage < 0 || hImage >= imageList.size())
	{
		return;
	}

	//同じモデルを他でも使っていないか
	bool isExist = false;
	for (int i = 0; i < imageList.size(); i++)
	{
		//すでに開いている場合
		if (imageList[i] != nullptr && i != hImage && imageList[i]->pSprite_ == imageList[hImage]->pSprite_)
		{
			isExist = true;
			break;
		}
	}

	//使ってなければモデル解放
	if (isExist == false)
	{
		SAFE_DELETE(imageList[hImage]->pSprite_);
	}

	SAFE_DELETE(imageList[hImage]);

}

void Image::AllRelease()
{
	for (int i = 0; i < imageList.size(); i++)
	{
		Release(i);
	}
	imageList.clear();
}

void Image::SetRect(int hImage, int x, int y, int width, int height)
{
	if (hImage < 0 || hImage >= imageList.size())
	{
		return;
	}

	imageList[hImage]->rect_.left = x;
	imageList[hImage]->rect_.top = y;
	imageList[hImage]->rect_.right = width;
	imageList[hImage]->rect_.bottom = height;
}

void Image::ResetRect(int hImage)
{
	if (hImage < 0 || hImage >= imageList.size())
	{
		return;
	}

	XMFLOAT3 size = imageList[hImage]->pSprite_->GetTextureSize();

	imageList[hImage]->rect_.left = 0;
	imageList[hImage]->rect_.top = 0;
	imageList[hImage]->rect_.right = (long)imageList[hImage]->size_.x;
	imageList[hImage]->rect_.bottom = (long)imageList[hImage]->size_.y;
}

void Image::SetAlpha(int hImage, int alpha)
{
	if (hImage < 0 || hImage >= imageList.size())
	{
		return;
	}
	imageList[hImage]->alpha_ = (float)alpha / 255.0f;
}

void Image::SetColor(int hImage, XMFLOAT3 color)
{
	if (hImage < 0 || hImage >= imageList.size())
	{
		return;
	}
	imageList[hImage]->color_ = color;
}

XMFLOAT3 Image::GetTextureSize(int hImage)
{
	return imageList[hImage]->size_;
}
