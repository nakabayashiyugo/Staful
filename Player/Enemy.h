#pragma once
#include "Player.h"
#include "../StageOrigin.h"

class Enemy : public Player
{
private:

public:
	Enemy(GameObject* _parent);
	~Enemy();

	void PlayerOperation() override;

//AI�ɂ���(��)
private:
	//���[�g�m�[�h
	RootNode* rootNode_;

	//�}�X���Ƃ̊댯�x
	std::vector<int> risks_;

public:
	//�q���ǉ�����
	void AddChildNode();
	//�ړ��������߂�
	void SelectMoveDir();
};

