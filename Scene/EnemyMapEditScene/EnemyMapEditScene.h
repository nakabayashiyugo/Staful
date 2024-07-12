#pragma once
#include "../MapEditScene/MapEditScene.h"

class RootNode;

class EnemyMapEditScene : public MapEditScene
{

public:
	EnemyMapEditScene(GameObject* _parent);

	void Initialize() override;
	void Update() override;
//AI
private:
	RootNode* rootNode_;
public:
	void AddNode();

	void SelectMathSet() override;
	void SelectMathType() override;
};

