#include "Engine/GameObject.h"

class Togetoge
	:public GameObject
{
	int hModel_;
	float moveCount_;

	XMFLOAT3 initPos_, destPos_;
public:
	Togetoge(GameObject* parent);

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
	void Release() override;

	XMFLOAT3 GetTogetogeCenterPos();
};

