#pragma once
#include "StateBase.h"
class Player;

//待機
class PlayerIdleState : public StateBase
{
	Player* pPlayer_;

public:
	PlayerIdleState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

//移動
class PlayerWalkState : public StateBase
{
	Player* pPlayer_;
public:
	PlayerWalkState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

//ジャンプ
class PlayerJumpState : public StateBase
{
	Player* pPlayer_;
public:
	PlayerJumpState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

//落下
class PlayerFallState : public StateBase
{
	Player* pPlayer_;
public:
	PlayerFallState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

//コンベア移動
class PlayerConvMoveState : public StateBase
{
	Player* pPlayer_;
public:
	PlayerConvMoveState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

//混乱
class PlayerConfState : public StateBase
{
	Player* pPlayer_;
public:
	PlayerConfState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};

//死亡
class PlayerDeadState : public StateBase
{
	Player* pPlayer_;
public:
	PlayerDeadState(StateManager* _pStateManager);
	void EnterState() override;
	void UpdateState() override;
	void ExitState() override;
};



