#include "AIWork.h"

#include "../Player.h"

NodeState Action_Walk::operator()()
{
	pPlayer_->SetPlayerState(STATE_WALK);
	return NodeState::SUCCESS;
}

Action_Walk::Action_Walk()
{
}

NodeState Action_Jump::operator()()
{
	pPlayer_->SetPlayerState(STATE_JUMP);
	return NodeState::SUCCESS;
}

Action_Jump::Action_Jump()
{
}

NodeState PlayerMoveDirSelect::operator()()
{
	pPlayer_->SetMoveDir(DIR_W);
	return NodeState::RUNNING;
}

PlayerMoveDirSelect::PlayerMoveDirSelect()
{
}
