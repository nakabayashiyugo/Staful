#include "Engine/GameObject.h"
#include <vector>


class Button;
class RadioButton;

#pragma once
namespace ButtonManager
{
	//�m�[�}���{�^��
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

	////���W�I�{�^��
	////�{�^���̒ǉ�
	//int AddRadioButton(std::string _btnName, int btn[], int _num, GameObject* _parent);
	////�{�^���̍폜
	//void RemoveRadioButton(int _handle);
	////�{�^���̑S�폜
	//void RemoveAllRadioButton();
	////�{�^���̎擾
	//RadioButton* GetRadioButton(int _handle);

};

