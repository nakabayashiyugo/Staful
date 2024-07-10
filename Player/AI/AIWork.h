#include "../../Engine/Work.h"

class Player;

//ActionNode
class PlayerActionBase : public Work
{
public:
	virtual NodeState operator()() override = 0{};
	PlayerActionBase() : pPlayer_(nullptr) {};
	//Player�̃Z�b�^�[
	void SetPlayer(Player* _p) { pPlayer_ = _p; }

protected:
	Player* pPlayer_;
};

class Action_Walk : public PlayerActionBase
{
public:
	NodeState operator()()override;
	Action_Walk();
private:
};

class Action_Jump : public PlayerActionBase
{
public:
	NodeState operator()()override;
	Action_Jump();
private:
};

//�ړ��������߂�Work
class PlayerMoveDirSelect : public PlayerActionBase
{
public:
	NodeState operator()()override;
	PlayerMoveDirSelect();
private:

};

//�W�����v���E�H�[�N�����߂�Work
class Select_State : public PlayerActionBase
{
public:
	NodeState operator()()override;
	Select_State();
};