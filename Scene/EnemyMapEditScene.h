#pragma once
#include "MapEditScene.h"

class EnemyMapEditScene : public MapEditScene
{

public:
	EnemyMapEditScene(GameObject* _parent);

//AI
private:
	
public:
	void SelectMathSet() override;
	void SelectMathType() override;
};

