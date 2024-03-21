#pragma once
#include "Engine/GameObject.h"

class ThankyouScene
	:public GameObject
{
	int hReturnTitle_, hThankyou_;

	Transform tReturnTitle_, tThankyou_;
	XMFLOAT3 mousePos_;
public:
	ThankyouScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;
};