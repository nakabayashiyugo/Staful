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

	//�����蔻��
	void OnCollision(GameObject* pTarget) override;

//AI�ɂ���(��)
private:
	//���[�g�m�[�h
	RootNode* rootNode_;

	//�}�X���Ƃ̊댯�x
	std::vector<int> risks_;
	//Astar�ɓn���}�X���Ƃ̃R�X�g
	std::vector<std::vector<int>> costs_;

public:
	//rootNode_�Ɏq����ǉ�����
	void AddNode();
	//�ړ��������߂�
	void SelectMoveDir();

	int GetCost(int x, int y);
	void SetCost(int _cost, int x, int y);
};

