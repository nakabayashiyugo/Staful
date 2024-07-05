#include "../../Engine/Work.h"

class Player;

class Action_Idle : public Work
{
public:
	NodeState operator()()override;
	Action_Idle();
private:
	//ƒvƒŒƒCƒ„[æ“¾
	Player* FindPlayer();
};