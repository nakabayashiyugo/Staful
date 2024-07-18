#pragma once
#include "../Engine/GameObject.h"

class Button;
class BillBoard;

class TitleScene
	:public GameObject
{
	int hSoloPlayButton_, hTwoPlayButton_;
	int hStaful_;
	Transform tSoloPlayButton_, tTwoPlayButton_;
	Transform tStaful_;

	//�{�^���̃n���h��
	int soloPlayHandle_, twoPlayHandle_;
	
public:
	TitleScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	void GameStateWrite(int _state);
};