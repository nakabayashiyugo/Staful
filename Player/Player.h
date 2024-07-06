#pragma once
#include "../Engine/GameObject.h"

#include "../StageOrigin.h"

#include "../Engine/VFX.h"

class PlayScene;
class Timer;
class Shaker;
class Text;
class StateManager;

enum PLAYER_STATE
{
	STATE_IDLE = 0,	//�~�܂��Ă�Ƃ�
	STATE_WALK,		//�����Ă���Ƃ�
	STATE_JUMP,		//�W�����v�����Ƃ�
	STATE_FALL,		//�������Ă���Ƃ�
	STATE_CONVMOVE,	//�R���x�A�ɂ���Ĉړ����Ă���Ƃ�
	STATE_CONF,		//�����}�X�ɏ���Ă��邭�邷��Ƃ�
	STATE_DEAD,		//���񂾂Ƃ�
};

enum STAGE_STATE
{
	STATE_START = 0,
	STATE_PLAY,
	STATE_GOAL,
	STATE_FAILURE,
};

enum PLAYER_MOVEDIR
{
	DIR_W = 0,
	DIR_S,
	DIR_D,
	DIR_A,
	DIR_MAX,
};

class Player
	:public GameObject, StageOrigin
{
	int hModel_;

	PlayScene* pPlayScene_;
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
	//�J�����̈ʒu
	XMFLOAT3 camPos_;
	//�J�����̌���I�u�W�F�N�g
	XMFLOAT3 camTarget_;
	//�J�����̌���I�u�W�F�N�g�ƃJ�����̋���
	XMFLOAT3 dirCamToPlayer_;

	Shaker* pCamShaker_;
public:
	//�_���[�W���󂯂�����
	void DamageDir(XMFLOAT3 _hitTgtgPos);
	//�J�����̈ʒu
	void CameraPosSet();
	//�J�����̐U���̏�����
	void CameraShakeInit();
	//�J�����̐U��
	void CameraShake();


//�v���C���[�̑���ɂ���
private:
	//�X�^�[�g�ʒu�A�S�[���ʒu
	XMFLOAT3 startPos_, goalPos_;
	//��̍���
	float playerHeight_;
	//�ړ����I��������
	bool moveFinished_;
	float moveCount_;
	//moveCount_�̖��t���[���������l
	float moveCntUpdate_;
	//�����鑬�x
	float fallSpeed_;
	//�X�y�[�X�{�^���������ꂽ��
	bool isPushSpace_;
	//wasd���������Ƃ��̈ړ�����
	PLAYER_MOVEDIR pushButtonMoveDir_[DIR_MAX];
	//�ړ����\�ȕ���
	XMFLOAT3 possiMoveDir_[DIR_MAX]; 
	//���{�^�������ꂽ��
	PLAYER_MOVEDIR pushButton_;
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
	//�X�^�[�g�}�X�ɖ߂��Ă��邩
	bool isReturn_;
	//�R���x�A�}�X�ɏ����������ł���t���[����
	int operableFrames_;
	//�R���x�A�}�X�ɉ��t���[���A�����ď���Ă��邩
	int onConvFrames_;

	//�����Ă���}�X�̏��
	MATHDEDAIL standMath_;
	//���O�܂ł����Ă����}�X�̏��
	MATHDEDAIL prevStandMath_;

	StateManager* pStateManager_;
	//�v���C���[�̏��
	PLAYER_STATE playerState_;
	//�v���C���[�̒��O�̏��
	PLAYER_STATE prevPlayerState_;

	//�Q�[���̐i�s�x�̏��
	STAGE_STATE stageState_;

	//�������Ă�}�X�ō����̃A�j���[�V�������I���Ă��邩
	bool isConfAnim_;
	//�����}�X�ɏ�������̍ŏ���rotate�̒l
	XMFLOAT3 prevRot_;


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
	void JumpUpdate();
	void FallUpdate();
	void ConvMoveUpdate();
	void ConfUpdate();
	void DeadUpdate();
	//�X�^�[�g�}�X�ɖ߂�
	void ReturnToStartMath();

	//�ړ����I�������Ƃ��̏���
	void MoveFinished();

	//�����̗����Ă���}�X�̌���
	void MathTypeEffect();
	//player���X�e�[�W���ɓ����Ă��邩
	bool Is_InSide_Table(XMFLOAT3 _pos);
	//�X�e�[�g������
	void StateInit();

	//�v���C���[�̃|�W�V�����̃Q�b�^�[
	XMFLOAT3 GetPosition() { return transform_.position_; }
	//standMath_�̃Q�b�^�[
	MATHDEDAIL GetStandMath() { return standMath_; }
	//�v���C���[�̃X�e�[�g�̃Q�b�^�[�E�Z�b�^�[
	PLAYER_STATE GetPlayerState() { return playerState_; }
	void SetPlayerState(PLAYER_STATE _state) { playerState_ = _state; }
	//�Q�[���̐i�s�x�̃X�e�[�g�̃Q�b�^�[
	STAGE_STATE GetStageState() { return stageState_; }

	void PossiMoveDirInit();
	

//��������ɂ���
private:
	//�ړ������𕶎��Ƃ��ē��ꂽ�z��
	std::string moveDirStr_[DIR_MAX];

	//���������\������Ă���ꏊ�̔w�i
	int hOpeExplanBackGround_;

	Text* pText_;

public:
	//��������̕`��
	void OperateExplanDraw();


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
	//�ړ��ɂ�����t���[����
	int moveFrameNum_;
public:
	//�A�j���[�V�����̃t���[�����[�g
	void SetAnimFramerate();


//�q�b�g�X�g�b�v�ɂ���
private:
	//�q�b�g�X�g�b�v�̎���
	float hitStopTime_;
	bool isHitStop_;

	Timer* pHitStopTimer_;

	//�q�b�g�X�g�b�v���J������U�������邩
	bool isCamShake_;
public:
	//�q�b�g�X�g�b�v�̏�����
	void HitStopInit();
	//�q�b�g�X�g�b�v���̏���
	void HitStopUpdate();


//���ԃQ�[�W�ɂ���
private:
	//��������
	int gameTime_;
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


//�G�t�F�N�g�ɂ���
private:
	//�Ƃ��Ƃ��Ɠ����������ɏo��G�t�F�N�g�Ɋւ���f�[�^
	EmitterData deadEmitData_;
	//�Ƃ��Ƃ��Ɠ����������ɏo��G�t�F�N�g�Ɋւ���f�[�^�̃n���h��
	int deadEmitHandle_;

public:
	//�Ƃ��Ƃ��Ɠ����������ɏo��G�t�F�N�g�̑��
	//���� : ���������Ƃ��Ƃ��̈ʒu
	void EmitterDataAssign(XMFLOAT3 _hitTgtgPos);


//���y�ɂ���
private:
	//�Ƃ��Ƃ��Ɠ�����������SE�̃n���h��
	int hSE_Damage_;
public:
	//���̏�����
	void AudioInit();
};