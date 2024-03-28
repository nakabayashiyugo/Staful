#pragma once
#include "StageOrigin.h"

class Text;
class Button;

class MapEditScene
	:public GameObject, StageOrigin
{
	int hPict_[MATH_MAX];
	int hTgtgRoute_;

	//�}�X�̏c�����̗�
	int YSIZE;

	//�I�΂�Ă�}�X�̎��
	int mathtype_;
	//�������ށE�ǂ݂����t�@�C��
	int saveNum_;
	//�u�����Q���̐�
	int mathChangeNumLimit_;
	//�����Ă��Q���̐�
	int mathChangeNum_;

	XMFLOAT3 texture_size_;

	//���X�̃}�X�̏��
	std::vector<std::vector<MATHDEDAIL>> math_origin_;
	//true : �I�����ꂽ�R���x�A�}�X�����
	//false : �R���x�A�}�X�͉��Ȃ�
	std::vector<std::vector<bool>> isConvRot_;

	Text* pText_;

	//��ʏ�̃}�X�̎�ނ�I������{�^��
	Button* pMathButton_[MATH_MAX];
public:
	MapEditScene(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	BOOL DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

	void Write();
	void Read();

	bool isMathChangeNumLimit();

	//�}�X�I���{�^���̏�����
	void MathButtonInit(XMFLOAT3 _imageSize);
};

