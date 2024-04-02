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

	//�ړ�����
	XMFLOAT3 moveVec_;
	//�ړ���̈ʒu
	XMFLOAT3 destPos_;
	//�ړ��O�̈ʒu
	XMFLOAT3 prevPos_;
	//�ړ��x�N�g��
	XMVECTOR velocity_;
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
	STAGE_STATE stageState;
	//�A�j���[�V�����̃t���[��
	int nowFrame_, startFrame_, endFrame_;

	//�����Ă���}�X�̏��
	MATHDEDAIL standMath_;

	//�d��
	XMFLOAT3 gravity_;

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
	//�v���C���[�̑���Ɋւ���֐�
	void PlayerOperation();
	//�O�ړ�
	void PlayerMoveFront();
	//���ړ�
	void PlayerMoveBack();
	//�E�ړ�
	void PlayerMoveRight();
	//���ړ�
	void PlayerMoveLeft();

	void IdleUpdate();
	void WalkUpdate();
	void JampUpdate();
	void FallUpdate();
	void DeadUpdate();
	void ReturnToStartMath();

	float GetHitStopTime() { return hitStopTime_; }
	bool GetHitStop() { return isHitStop_; }
	void SetHitStop(bool _ishit) { isHitStop_ = _ishit; }

	void SetAnimFramerate();

	bool Is_InSide_Table(XMFLOAT3 _pos);

	

	bool Is_Goal() { return isGoal_; }

	XMFLOAT3 GetPosition() { return centerPos_; }

	MATHDEDAIL GetMathType(XMFLOAT3 _pos);

	bool GetFailed() {
		if (stageState == STATE_FAILURE)
		{
			return true;
		}
		return false;
	}
	PLAYER_STATE GetPlayerState() { return playerState_; }

	void OnCollision(GameObject* pTarget) override;

	//���ԃQ�[�W
	void TimeGageManagement();
};