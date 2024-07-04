#include "PlayerState.h"

#include "Player.h"
#include "StateManager.h"

PlayerIdleState::PlayerIdleState(StateManager* _pStateManager)
	:StateBase(_pStateManager)
{
	pPlayer_ = (Player*)(pStateManager_->GetGameObject());
}

void PlayerIdleState::EnterState()
{
}

void PlayerIdleState::UpdateState()
{
	pPlayer_->IdleUpdate();
	switch (pPlayer_->GetPlayerState())
	{
	case STATE_WALK:
		pStateManager_->ChangeState("STATE_WALK");
		break;
	case STATE_JUMP:
		pStateManager_->ChangeState("STATE_JUMP");
		break;
	case STATE_FALL:
		pStateManager_->ChangeState("STATE_FALL");
		break;
	case STATE_CONVMOVE:
		pStateManager_->ChangeState("STATE_CONVMOVE");
		break;
	case STATE_CONF:
		pStateManager_->ChangeState("STATE_CONF");
		break;
	case STATE_DEAD:
		pStateManager_->ChangeState("STATE_DEAD");
		break;
	}
}

void PlayerIdleState::ExitState()
{
}

PlayerWalkState::PlayerWalkState(StateManager* _pStateManager)
	:StateBase(_pStateManager)
{
	pPlayer_ = (Player*)(pStateManager_->GetGameObject());
}

void PlayerWalkState::EnterState()
{
}

void PlayerWalkState::UpdateState()
{
	pPlayer_->WalkUpdate();
	switch (pPlayer_->GetPlayerState())
	{
	case STATE_IDLE:
		pStateManager_->ChangeState("STATE_IDLE");
		break;
	case STATE_DEAD:
		pStateManager_->ChangeState("STATE_DEAD");
		break;
	}
}

void PlayerWalkState::ExitState()
{
}

PlayerJumpState::PlayerJumpState(StateManager* _pStateManager)
	:StateBase(_pStateManager)
{
	pPlayer_ = (Player*)(pStateManager_->GetGameObject());
}

void PlayerJumpState::EnterState()
{
}

void PlayerJumpState::UpdateState()
{
	pPlayer_->JumpUpdate();
	switch (pPlayer_->GetPlayerState())
	{
	case STATE_IDLE:
		pStateManager_->ChangeState("STATE_IDLE");
		break;
	case STATE_DEAD:
		pStateManager_->ChangeState("STATE_DEAD");
		break;
	}
}

void PlayerJumpState::ExitState()
{
}

PlayerFallState::PlayerFallState(StateManager* _pStateManager)
	:StateBase(_pStateManager)
{
	pPlayer_ = (Player*)(pStateManager_->GetGameObject());
}

void PlayerFallState::EnterState()
{
}

void PlayerFallState::UpdateState()
{
	pPlayer_->FallUpdate();
	switch (pPlayer_->GetPlayerState())
	{
	case STATE_IDLE:
		pStateManager_->ChangeState("STATE_IDLE");
		break;
	case STATE_DEAD:
		pStateManager_->ChangeState("STATE_DEAD");
		break;
	}
}

void PlayerFallState::ExitState()
{
}

PlayerConvMoveState::PlayerConvMoveState(StateManager* _pStateManager)
	:StateBase(_pStateManager)
{
	pPlayer_ = (Player*)(pStateManager_->GetGameObject());
}

void PlayerConvMoveState::EnterState()
{
}

void PlayerConvMoveState::UpdateState()
{
	pPlayer_->ConvMoveUpdate();
	switch (pPlayer_->GetPlayerState())
	{
	case STATE_IDLE:
		pStateManager_->ChangeState("STATE_IDLE");
		break;
	case STATE_DEAD:
		pStateManager_->ChangeState("STATE_DEAD");
		break;
	}
}

void PlayerConvMoveState::ExitState()
{
}

PlayerConfState::PlayerConfState(StateManager* _pStateManager)
	:StateBase(_pStateManager)
{
	pPlayer_ = (Player*)(pStateManager_->GetGameObject());
}

void PlayerConfState::EnterState()
{
}

void PlayerConfState::UpdateState()
{
	pPlayer_->ConfUpdate();
	switch (pPlayer_->GetPlayerState())
	{
	case STATE_IDLE:
		pStateManager_->ChangeState("STATE_IDLE");
		break;
	case STATE_DEAD:
		pStateManager_->ChangeState("STATE_DEAD");
		break;
	}
}

void PlayerConfState::ExitState()
{
}

PlayerDeadState::PlayerDeadState(StateManager* _pStateManager)
	:StateBase(_pStateManager)
{
	pPlayer_ = (Player*)(pStateManager_->GetGameObject());
}

void PlayerDeadState::EnterState()
{
}

void PlayerDeadState::UpdateState()
{
	pPlayer_->DeadUpdate();
	switch (pPlayer_->GetPlayerState())
	{
	case STATE_IDLE:
		pStateManager_->ChangeState("STATE_IDLE");
		break;
	}
}

void PlayerDeadState::ExitState()
{
}
