#pragma once
#include "GameObject.h"

enum SCENE_ID
{
	SCENE_ID_PLAY = 0,
	SCENE_ID_EDIT,
	SCENE_ID_TRANSITION,
	SCENE_ID_TITLE,
	SCENE_ID_EXPLANATION,
	SCENE_ID_RESULT,
};

class SceneManager
	:public GameObject
{
	SCENE_ID currentSceneID_;
	SCENE_ID nextSceneID_;
public:
	SceneManager(GameObject* parent);

	~SceneManager();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	void ChangeScene(SCENE_ID _next);

	SCENE_ID GetCurrentSceneID(){ return currentSceneID_; }
};

