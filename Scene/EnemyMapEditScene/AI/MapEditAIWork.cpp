#include "MapEditAIWork.h"

#include "../../MapEditScene/MapEditScene.h"

NodeState MapEdit_IsPutMath::operator()()
{
	return NodeState::RUNNING;
}

MapEdit_IsPutMath::MapEdit_IsPutMath()
{
}

NodeState MapEdit_Action_ChangeMath::operator()()
{
	editer_->ChangeSelectMath();
	return NodeState::SUCCESS;
}

MapEdit_Action_ChangeMath::MapEdit_Action_ChangeMath()
{
}


NodeState MapEdit_MathPosSelect::operator()()
{
	editer_->SelectMathSet();
	if (editer_->GetSelectMath().x == -1 &&
		editer_->GetSelectMath().y == -1)
	{
		return NodeState::FAILED;
	}
	return NodeState::RUNNING;
}

MapEdit_MathPosSelect::MapEdit_MathPosSelect()
{
}


NodeState MapEdit_MathTypeSelect::operator()()
{
	editer_->SelectMathType();
	return NodeState::RUNNING;
}

MapEdit_MathTypeSelect::MapEdit_MathTypeSelect()
{
}
