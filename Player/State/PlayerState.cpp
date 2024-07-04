#include "PlayerState.h"

#include "../Player.h"

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
	switch (pPlayer_->GetPlayerState())
	{
	case STATE_WALK:
		pStateManager_->ChangeState("STATE_WALK");
		return;
	case STATE_JUMP:
		pStateManager_->ChangeState("STATE_JUMP");
		return;
	case STATE_FALL:
		pStateManager_->ChangeState("STATE_FALL");
		return;
	case STATE_CONVMOVE:
		pStateManager_->ChangeState("STATE_CONVMOVE");
		return;
	case STATE_CONF:
		pStateManager_->ChangeState("STATE_CONF");
		return;
	case STATE_DEAD:
		pStateManager_->ChangeState("STATE_DEAD");
		return;
	}
	pPlayer_->IdleUpdate();
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
	switch (pPlayer_->GetPlayerState())
	{
	case STATE_IDLE:
		pStateManager_->ChangeState("STATE_IDLE");
		return;
	case STATE_DEAD:
		pStateManager_->ChangeState("STATE_DEAD");
		return;
	}
	pPlayer_->WalkUpdate();
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
	switch (pPlayer_->GetPlayerState())
	{
	case STATE_IDLE:
		pStateManager_->ChangeState("STATE_IDLE");
		return;
	case STATE_DEAD:
		pStateManager_->ChangeState("STATE_DEAD");
		return;
	}
	pPlayer_->JumpUpdate();
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
	switch (pPlayer_->GetPlayerState())
	{
	case STATE_IDLE:
		pStateManager_->ChangeState("STATE_IDLE");
		return;
	case STATE_DEAD:
		pStateManager_->ChangeState("STATE_DEAD");
		return;
	}
	pPlayer_->FallUpdate();
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
	switch (pPlayer_->GetPlayerState())
	{
	case STATE_IDLE:
		pStateManager_->ChangeState("STATE_IDLE");
		return;
	case STATE_DEAD:
		pStateManager_->ChangeState("STATE_DEAD");
		return;
	}
	pPlayer_->ConvMoveUpdate();
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
	switch (pPlayer_->GetPlayerState())
	{
	case STATE_IDLE:
		pStateManager_->ChangeState("STATE_IDLE");
		return;
	case STATE_DEAD:
		pStateManager_->ChangeState("STATE_DEAD");
		return;
	}
	pPlayer_->ConfUpdate();
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
	switch (pPlayer_->GetPlayerState())
	{
	case STATE_IDLE:
		pStateManager_->ChangeState("STATE_IDLE");
		return;
	}
	pPlayer_->DeadUpdate();
}

void PlayerDeadState::ExitState()
{
}
