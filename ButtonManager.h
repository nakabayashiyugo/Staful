#include "Engine/GameObject.h"
#include <vector>

class Button;

#pragma once
namespace ButtonManager
{
	//�{�^���̒ǉ�
	int AddButton(std::string _btnName, GameObject* _parent);
	//�{�^���̍폜
	void RemoveButton(int _handle);
	//�{�^���̑S�폜
	void RemoveAllButton();

	//�{�^���̎擾
	Button* GetButton(int _handle);

	//�摜�̐ݒ�
	void SetPict(int _handle, int _imageHandle);
	//�g�����X�t�H�[���̐ݒ�
	void SetTransform(int _handle, Transform _transform);
};

