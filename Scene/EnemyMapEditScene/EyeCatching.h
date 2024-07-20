#pragma once
#include "../../../Engine/GameObject.h"

class Timer;

class EyeCatching : public GameObject
{
	//��ʂ��Â�����摜�ԍ�
	int hDarken_;
	//�}�b�v���쒆�������摜�ԍ�
	int hMaking_, hMakingPoint_;
	//�}�b�v���쒆�́u.�v�̐�
	int makingPointNum_;

	Timer* makingPointTimer_;
public:
	EyeCatching(GameObject* _parent);

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void EnemyMapEditMakingInit();
	void EnemyMapEditMakingDraw();
};

