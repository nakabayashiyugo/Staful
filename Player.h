#pragma once
#include "Engine/GameObject.h"
#include "StageOrigin.h"

class PlayScene;
class Timer;

enum PLAYER_STATE
{
	STATE_IDLE = 0,
	STATE_WALK,
	STATE_JAMP,
	STATE_FALL,
	STATE_DEAD,
};

enum STAGE_STATE
{
	STATE_START = 0,
	STATE_PLAY,
	STATE_GOAL,
	STATE_FAILURE,
};


class Player
	:public GameObject, StageOrigin
{
	int hModel_;
	//���ԃQ�[�W�̉摜
	int hFrame_, hGage_, hFrameOutline_;
	int hTime_;
	//���ԃQ�[�W�̉摜�̃g�����X�t�H�[��
	Transform tFrame_, tGage_, tFrameOutline_;
	Transform tTime_;

	//�N���A������
	bool isGoal_;
	//�q�b�g�X�g�b�v�̎���
	float hitStopTime_;
	bool isHitStop_;

	PlayScene* pPlayScene_;

	//�ړ����I��������
	bool moveFinished_;
	float moveCount_;
	//������̖��t���[���������l
	float upVecPlus_;
	//�ړ�����
	XMFLOAT3 moveDir_;
	//�ړ���̈ʒu
	XMFLOAT3 destPos_;
	//�ړ��O�̈ʒu
	XMFLOAT3 prevPos_;
	//�ړ��x�N�g��
	XMVECTOR velocity_;
	//�d��
	XMFLOAT3 gravity_;
	//�d�͉����x
	float gravityAcce_;
	//�����x�N�g��
	XMVECTOR eyeDirection_;


	XMFLOAT3 tableHitPoint_;
	bool isTableHit_;

	XMFLOAT3 startPos_, goalPos_;
	XMFLOAT3 centerPos_;
	XMFLOAT3 camRot_;

	//�v���C���[�̏��
	PLAYER_STATE playerState_;
	//�v���C���[�̒��O�̏��
	PLAYER_STATE prevPlayerState_;
	//�Q�[���̐i�s�x�̏��
	STAGE_STATE stageState_;
	//�A�j���[�V�����̃t���[��
	int nowFrame_, startFrame_, endFrame_;

	//�����Ă���}�X�̏��
	MATHDEDAIL standMath_;

	

	//��Q���̒u���鐔
	int hurdle_Limit_;

	//�^�C�}�[
	Timer* pTimer_;

public:
	Player(GameObject* parent);

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	void PlayUpdate();
	void CameraPosSet();
	//�v���C���[�̑���Ɋւ���֐�
	void PlayerOperation();
	//�ړ�
	void PlayerMove();
	//�ړ���̃}�X�̎��
	MATHTYPE DestPosMathType();

	//���ꂼ��̏�Ԃ�Update
	void IdleUpdate();
	void WalkUpdate();
	void JampUpdate();
	void FallUpdate();
	void DeadUpdate();

	//�X�^�[�g�}�X�ɖ߂�
	void ReturnToStartMath();

	//�����̗����Ă���}�X�̌���
	void MathTypeEffect();

	//�q�b�g�X�g�b�v
	float GetHitStopTime() { return hitStopTime_; }
	bool GetHitStop() { return isHitStop_; }
	void SetHitStop(bool _ishit) { isHitStop_ = _ishit; }

	//�A�j���[�V�����̃t���[�����[�g
	void SetAnimFramerate();

	//player���X�e�[�W���ɓ����Ă��邩
	bool Is_InSide_Table(XMFLOAT3 _pos);

	XMFLOAT3 GetPosition() { return centerPos_; }

	//�߂�l : �����̈ʒu�̃}�X�̎��
	MATHDEDAIL GetMathType(XMFLOAT3 _pos);

	PLAYER_STATE GetPlayerState() { return playerState_; }
	STAGE_STATE GetStageState() { return stageState_; }

	//�����蔻��
	void OnCollision(GameObject* pTarget) override;

	//���ԃQ�[�W
	void TimeGageManagement();
};