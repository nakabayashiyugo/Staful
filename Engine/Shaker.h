#pragma once
#include "Transform.h"

class Timer;
class Easing;

enum SHAKETYPE
{
	TYPE_VERTICAL = 0,
	TYPE_BESIDE,
	TYPE_RANDOM,
	TYPE_MAX,
};

class Shaker
{
private:
	//�J�����̐U���Ɋւ���ϐ�
	XMFLOAT3 *position_;		//�U������I�u�W�F�N�g�̈ʒu
	XMFLOAT3 prevPosition_;		//�U������O�̈ʒu
	XMVECTOR shakeDir_;			//�U���������
	float shakeTime_;			//�U�����鎞��
	float shakePower_;			//�U���̋���
	bool isShake_, prevIsShake_;//�U�����邩�ǂ���
	float moveCount_;			//�U���̕�Ԓl
	XMVECTOR shaft_;			//�U���̎�
	Timer* pTimer_;
	Easing* pEasing_;

	SHAKETYPE shakeType_;

public:
	Shaker();
	~Shaker();

	//�J�����̐U��
	//����1 : �U�����������I�u�W�F�N�g�̃|�W�V�����̃|�C���^
	//����2 : �U���̎���
	//����3 : �U���̋���
	void ShakeInit(XMFLOAT3 *_position, SHAKETYPE _shakeType, float _vibTime, float _vibPower);
	void ShakeUpdate();

	//�ړ����������߂�
	void ShakeDirAssign();

	void SetVibElem(float _vibTime, float _vibPower);

	//�U�����邩�ǂ����̃Z�b�^�[
	void SetIsShake(bool _isShake);
	//�U�����邩�ǂ����̃Q�b�^�[
	bool GetIsShake() { return isShake_; }

	//��Ԓl�̃Q�b�^�[
	float GetMoveCount() { return moveCount_; }
};

