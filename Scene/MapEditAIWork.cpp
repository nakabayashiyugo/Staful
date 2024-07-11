#include "MapEditAIWork.h"

#include "MapEditScene.h"

NodeState MapEdit_IsPutMath::operator()()
{
	if (editer_->CostManagement(editer_->GetSelectMath()))
	{
		return NodeState::RUNNING;
	}
	return NodeState::FAILED;
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
