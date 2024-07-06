#include "AIWork.h"

#include "../Player.h"

NodeState Action_Walk::operator()()
{
	NodeState ret;
	pPlayer_->SetPlayerState(STATE_WALK);
	if (pPlayer_->GetPlayerState() == STATE_IDLE)
	{
		ret = NodeState::SUCCESS;
	}
	else if (pPlayer_->GetPlayerState() == STATE_DEAD)
	{
		ret = NodeState::FAILED;
	}
	else if (pPlayer_->GetPlayerState() == STATE_WALK)
	{
		ret = NodeState::RUNNING;
	}
	return ret;
}

Action_Walk::Action_Walk()
{
}

NodeState Action_Jump::operator()()
{
	NodeState ret;
	pPlayer_->SetPlayerState(STATE_JUMP);
	if (pPlayer_->GetPlayerState() == STATE_IDLE)
	{
		ret = NodeState::SUCCESS;
	}
	else if (pPlayer_->GetPlayerState() == STATE_DEAD)
	{
		ret = NodeState::FAILED;
	}
	else if (pPlayer_->GetPlayerState() == STATE_JUMP)
	{
		ret = NodeState::RUNNING;
	}
	return ret;
}

Action_Jump::Action_Jump()
{
}
