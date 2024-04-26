#pragma once
#include "Transform.h"

class Timer;
class Easing;

class Shaker
{
private:
	//�J�����̐U���Ɋւ���ϐ�
	XMFLOAT3 *position_;		//�U������O�̈ʒu
	XMVECTOR shakeDir_;			//�U���������
	float shakeTime_;			//�U�����鎞��
	float shakePower_;			//�U���̋���
	int shakeNum_;				//�U�������
	bool isShake_, prevIsShake_;//�U�����邩�ǂ���
	Timer* pTimer_;
	Easing* pEasing_;

public:
	Shaker();
	~Shaker();

	//�J�����̐U��
	//����1 : �U�����������I�u�W�F�N�g�̃|�W�V�����̃|�C���^
	//����2 : �U���̎���
	//����3 : �U���̋���
	void ShakeInit(XMFLOAT3 *_position, float _vibTime, float _vibPower);
	void ShakeUpdate();

	void SetVibElem(float _vibTime, float _vibPower);

	//�U�����邩�ǂ����̃Z�b�^�[
	void SetIsShake(bool _isShake);
	//�U�����邩�ǂ����̃Q�b�^�[
	bool GetIsShake() { return isShake_; }
};

