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
	MATHTYPE type = dynamic_cast<Enemy*>(pPlayer_)->GetMathType(dynamic_cast<Enemy*>(pPlayer_)->GetDestPos()).mathType_;
	if (type == MATH_HOLE || type == MATH_TOGETOGE)
	{
		//Walk���s���Ă��Ƃ�FAILED(Jump�ɍs��)
		return NodeState::FAILED;
	}
	return NodeState::RUNNING;
}

Select_State::Select_State()
{
}
