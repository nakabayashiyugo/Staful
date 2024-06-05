#include <cmath>
#include "Engine/Model.h"
#include "Engine/SphereCollider.h"
#include "Togetoge.h"
#include "Stage.h"


const float PIE = 3.141592;

Togetoge::Togetoge(GameObject* parent) :
	GameObject(parent, "Togetoge"), hModel_(-1), initPos_(0, 0, 0), destPos_(0, 0, 0),
	moveCount_(0.0f)
{
	Stage* pStage = (Stage*)FindObject("Stage");
	initPos_.x = pStage->GetTogetogeInitPos().x;
	initPos_.y = 1;
	initPos_.z = pStage->GetTogetogeInitPos().y;

	destPos_.x = pStage->GetTogetogeDestPos(initPos_).x;
	destPos_.y = 1;
	destPos_.z = pStage->GetTogetogeDestPos(initPos_).z;
}

void Togetoge::Initialize()
{
	hModel_ = Model::Load("Assets\\Block\\Block_Togetoge.fbx");
	assert(hModel_ >= 0);

	transform_.position_ = initPos_;
}

void Togetoge::Update()
{
	SphereCollider* pSC = new SphereCollider(0.4);
	this->AddCollider(pSC);

	moveCount_ += 0.5f;
	transform_.position_.x = initPos_.x + (destPos_.x - initPos_.x) * abs(sin(moveCount_ * PIE / 180));
	transform_.position_.z = initPos_.z + (destPos_.z - initPos_.z) * abs(sin(moveCount_ * PIE / 180));
}

void Togetoge::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Togetoge::Release()
{
}
