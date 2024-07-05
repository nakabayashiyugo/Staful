#include "AIWork.h"

#include "../Player.h"

NodeState Action_Idle::operator()()
{

}

Action_Idle::Action_Idle()
{
}

Player* Action_Idle::FindPlayer()
{
	Player* findPlayer = (Player*)FindObject("Player");
}
