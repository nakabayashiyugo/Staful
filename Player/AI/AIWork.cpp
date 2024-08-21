#include "AIWork.h"

#include "../Player.h"
#include "../Enemy.h"

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
	dynamic_cast<Enemy*>(pPlayer_)->SelectMoveDir();
	return NodeState::RUNNING;
}

PlayerMoveDirSelect::PlayerMoveDirSelect()
{
}

NodeState Select_State::operator()()
{

	if (dynamic_cast<Enemy*>(pPlayer_)->IsJumpWise())
	{
		//Walké∏îsÇ¡ÇƒÇ±Ç∆Ç≈FAILED(JumpÇ…çsÇ≠)
		return NodeState::FAILED;
	}
	return NodeState::RUNNING;
}

Select_State::Select_State()
{
}
