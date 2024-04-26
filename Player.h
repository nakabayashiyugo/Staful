#pragma once
#include "Engine/GameObject.h"
#include "StageOrigin.h"

class PlayScene;
class Timer;
class Shaker;

enum PLAYER_STATE
{
	STATE_IDLE = 0,	//�~�܂��Ă�Ƃ�
	STATE_WALK,		//�����Ă���Ƃ�
	STATE_JAMP,		//�W�����v�����Ƃ�
	STATE_FALL,		//�������Ă���Ƃ�
	STATE_CONVMOVE,	//�R���x�A�ɂ���Ĉړ����Ă���Ƃ�
	STATE_DEAD,		//���񂾂Ƃ�
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

	PlayScene* pPlayScene_;

	//��Q���̒u���鐔
	int hurdle_Limit_;
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

	//�����蔻��
	void OnCollision(GameObject* pTarget) override;

//�J�����ɂ���
private:
	Shaker* pShaker_;
public:
	//�J�����̈ʒu
	void CameraPosSet();
//�v���C���[�̑���ɂ���
private:
	//�X�^�[�g�ʒu�A�S�[���ʒu
	XMFLOAT3 startPos_, goalPos_;
	//��̍���
	float playerHeight_;
	//�ړ����I��������
	bool moveFinished_;
	float moveCount_;
	//�����鑬�x
	float fallSpeed_;
	//�ړ�����
	XMFLOAT3 moveDir_;
	//�ړ���̈ʒu
	XMFLOAT3 destPos_;
	//�ړ��O�̈ʒu
	XMFLOAT3 prevPos_;
	//�d��
	XMFLOAT3 gravity_;
	//�d�͉����x
	float gravityAcce_;
	//�ړ��x�N�g��
	XMVECTOR velocity_;
	//�����x�N�g��
	XMVECTOR eyeDirection_;
	//���������̎��ʍ���
	float deadHeight_;

	//�����Ă���}�X�̏��
	MATHDEDAIL standMath_;
	//�v���C���[�̏��
	PLAYER_STATE playerState_;
	//�v���C���[�̒��O�̏��
	PLAYER_STATE prevPlayerState_;
	//�Q�[���̐i�s�x�̏��
	STAGE_STATE stageState_;
public:
	void PlayUpdate();
	//�v���C���[�̑���Ɋւ���֐�
	void PlayerOperation();
	//�ړ�
	void PlayerMove();
	//�v���C���[�������̕����Ɍ�������
	void ChangePlayerDir(XMVECTOR _vec);
	void ChangePlayerDir(XMFLOAT3 _vec);
	//�߂�l : �����̈ʒu�̃}�X�̎��
	MATHDEDAIL GetMathType(XMFLOAT3 _pos);
	//�ړ���̃}�X�̎��
	MATHTYPE DestPosMathType();
	//�ړ���Ƃ̊Ԃɕǂ����邩
	bool WallCheck();

	//���ꂼ��̏�Ԃ�Update
	void IdleUpdate();
	void WalkUpdate();
	void JampUpdate();
	void FallUpdate();
	void ConvMoveUpdate();
	void DeadUpdate();
	//�X�^�[�g�}�X�ɖ߂�
	void ReturnToStartMath();

	//�����̗����Ă���}�X�̌���
	void MathTypeEffect();
	//player���X�e�[�W���ɓ����Ă��邩
	bool Is_InSide_Table(XMFLOAT3 _pos);

	//�v���C���[�̃|�W�V�����̃Q�b�^�[
	XMFLOAT3 GetPosition() { return transform_.position_; }
	//standMath_�̃Q�b�^�[
	MATHDEDAIL GetStandMath() { return standMath_; }
	//�v���C���[�̃X�e�[�g�̃Q�b�^�[
	PLAYER_STATE GetPlayerState() { return playerState_; }
	//�Q�[���̐i�s�x�̃X�e�[�g�̃Q�b�^�[
	STAGE_STATE GetStageState() { return stageState_; }
	

//�e�ɂ���
private:
	//�e�̃��f���ԍ�
	int hShadow_;
	//�e�̃g�����X�t�H�[��
	Transform tShadow_;

public:
	void ShadowInit();
	void ShadowDraw();
	//�e�̓����̐���
	void ShadowManagement();

//�v���C���[�̃A�j���[�V�����ɂ���
private:
	//�A�j���[�V�����̃t���[��
	int nowFrame_, startFrame_, endFrame_;
public:
	//�A�j���[�V�����̃t���[�����[�g
	void SetAnimFramerate();


//�q�b�g�X�g�b�v�ɂ���
private:
	//�q�b�g�X�g�b�v�̎���
	float hitStopTime_;
	bool isHitStop_;
public:
	//�q�b�g�X�g�b�v
	float GetHitStopTime() { return hitStopTime_; }
	bool GetHitStop() { return isHitStop_; }
	void SetHitStop(bool _ishit) { isHitStop_ = _ishit; }


//���ԃQ�[�W�ɂ���
private:
	//���ԃQ�[�W�̉摜
	int hFrame_, hGage_, hFrameOutline_;
	int hTime_;
	//���ԃQ�[�W�̉摜�̃g�����X�t�H�[��
	Transform tFrame_, tGage_, tFrameOutline_;
	Transform tTime_;
	//�^�C�}�[
	Timer* pTimer_;
public:
	//���ԃQ�[�W
	void TimeGageManagement();
};