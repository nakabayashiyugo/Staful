#pragma once
#include <string>
#include <vector>
#include "Transform.h"
#include "Sprite.h"

namespace Image
{
	void SetTransform(int hImage, Transform transform);
	int Load(std::string filename);
	void Draw(int hImage);
	void Release(int _hImage);
	void AllRelease();

	void FrontHandlesDraw();

	void SetRect(int hImage, int x, int y, int width, int height);
	void ResetRect(int hImage);

	void SetAlpha(int hImage, int alpha);
	void SetColor(int hImage, XMFLOAT3 color);

	void SetFrontDrawHandle(int hImage);

	XMFLOAT3 GetTextureSize(int hImage);
}
