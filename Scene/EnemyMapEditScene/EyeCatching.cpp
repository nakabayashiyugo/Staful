#include "EyeCatching.h"
#include "../../../Timer.h"

//�}�b�v���쒆�̕\�����ς�鎞��
const float makingDrawChangeTime = 0.3f;
//�}�b�v���쒆�́u.�v�̊Ԋu
const float makingPointInterval = 0.05f;
//�}�b�v���쒆�́u.�v�̍ő吔
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
	//�}�b�v���쒆�Ɏʂ��摜�̃��[�h
	hDarken_ = Image::Load(folderName + "EnemyMapEdit.png");
	assert(hDarken_ >= 0);
	//�u�}�b�v���쒆�v
	hMaking_ = Image::Load(folderName + "EnemyMapEdit_Making.png");
	assert(hMaking_ >= 0);
	hMakingPoint_ = Image::Load(folderName + "EnemyMapEdit_Making_Point.png");
	assert(hMakingPoint_ >= 0);
}

void EyeCatching::EnemyMapEditMakingDraw()
{
	//�}�b�v���쒆�Ɏʂ��摜�̃T�C�Y
	XMFLOAT3 texSize = Image::GetTextureSize(hDarken_);
	//�}�b�v���쒆�Ɏʂ��摜�̃g�����X�t�H�[��
	Transform tPict;
	tPict.scale_ = XMFLOAT3(Direct3D::bfr_scrWidth / texSize.y,
		Direct3D::bfr_scrHeight / texSize.x, 1);
	Image::SetTransform(hDarken_, tPict);
	//��ԑO�ɕ`�悷��
	Image::Draw(hDarken_);


	//�u�}�b�v���쒆�v�̃e�N�X�`���T�C�Y
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
