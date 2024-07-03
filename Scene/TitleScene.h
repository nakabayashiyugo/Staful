#pragma once
#include "../Engine/GameObject.h"

class Button;
class BillBoard;

class TitleScene
	:public GameObject
{
	int hStartButton_, hExitButton_;
	int hStaful_;
	Transform tStartButton_;
	Transform tStaful_;

	//�{�^���̃n���h��
	int startBtnHandle_;
	
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
};