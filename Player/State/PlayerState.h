#pragma once
#include "StateBase.h"
class Player;

//�ҋ@
class PlayerIdleState : public StateBase
{
	Player* pPlayer_;

public:
	PlayerIdleState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

//�ړ�
class PlayerWalkState : public StateBase
{
	Player* pPlayer_;
public:
	PlayerWalkState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

//�W�����v
class PlayerJumpState : public StateBase
{
	Player* pPlayer_;
public:
	PlayerJumpState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

//����
class PlayerFallState : public StateBase
{
	Player* pPlayer_;
public:
	PlayerFallState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

//�R���x�A�ړ�
class PlayerConvMoveState : public StateBase
{
	Player* pPlayer_;
public:
	PlayerConvMoveState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

//����
class PlayerConfState : public StateBase
{
	Player* pPlayer_;
public:
	PlayerConfState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

//���S
class PlayerDeadState : public StateBase
{
	Player* pPlayer_;
public:
	PlayerDeadState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};



