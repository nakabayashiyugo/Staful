#pragma once
#include "../Engine/Work.h"

class MapEditScene;

class MapEdit_Work : public Work
{
public:
	virtual NodeState operator()() override = 0 {};
	MapEdit_Work() : editer_(nullptr) {};
	void SetEditer(MapEditScene* _editer) { editer_ = _editer; }

protected:
	MapEditScene* editer_;
};

//����ȏ�}�X���u���邩
class MapEdit_IsPutMath : public MapEdit_Work
{
public:
	NodeState operator()()override;
	MapEdit_IsPutMath();
};

//�}�X�ς���
class MapEdit_Action_ChangeMath : public MapEdit_Work
{
public:
	NodeState operator()()override;
	MapEdit_Action_ChangeMath();
};

//�ς���}�X�̈ʒu�Z�b�g
class MapEdit_MathPosSelect : public MapEdit_Work
{
public:
	NodeState operator()()override;
	MapEdit_MathPosSelect();
};

//�}�X�^�C�v�Z�b�g
class MapEdit_MathTypeSelect : public MapEdit_Work
{
public:
	NodeState operator()()override;
	MapEdit_MathTypeSelect();
};

