#pragma once
#include "../../../Engine/Work.h"

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

//マス変える
class MapEdit_Action_ChangeMath : public MapEdit_Work
{
public:
	NodeState operator()()override;
	MapEdit_Action_ChangeMath();
};

//変えるマスの位置セット
class MapEdit_MathPosSelect : public MapEdit_Work
{
public:
	NodeState operator()()override;
	MapEdit_MathPosSelect();
};

//マスタイプセット
class MapEdit_MathTypeSelect : public MapEdit_Work
{
public:
	NodeState operator()()override;
	MapEdit_MathTypeSelect();
};

