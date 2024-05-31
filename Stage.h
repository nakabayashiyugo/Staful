#include "StageOrigin.h"

class PlayScene;
class Togetoge;

struct TimeLimitDecElem
{
	int x;
	int y;
	bool isStand = false;	//���̃}�X�Ƀv���C���[�����������Ƃ����邩
};

class Stage
	:public GameObject, StageOrigin
{
	int hModel_[MATH_MAX];
	int saveNum_;

	bool isStandPitfall_;

	PlayScene* pPlayScene_;

	std::vector<Togetoge*> pTgtg_;
	//���Ƃ���
	std::vector<std::vector<int>> makeHoleTimes_;
	//�������ԒZ��������
	std::vector<TimeLimitDecElem> timeLimitDecs_;

	XMFLOAT3 tgtgGivePos_;
public:
	Stage(GameObject* parent);
	~Stage();

	//������
	void Initialize() override;

	//�X�V
	void Update() override;

	//�`��
	void Draw() override;

	//�J��
	void Release() override;

	void SetBlock(int x, int z, MATHTYPE _type);

	void SetTableMath(std::vector<std::vector<MATHDEDAIL>> _math);

	XMFLOAT3 GetTogetogeInitPos();

	XMFLOAT3 GetTogetogeDestPos(XMFLOAT3 _pos)
	{
		auto itr = tTgtgRoute_.begin();
		while (itr != tTgtgRoute_.end())
		{
			if ((int)itr->initPos_.x == _pos.x &&
				(int)itr->initPos_.y == _pos.z)
			{
				return XMFLOAT3(itr->destPos_.x, 0, itr->destPos_.y);
			}
			itr++;
		}
		return XMFLOAT3();
	}

	//�����Ŏw�肳�ꂽ�ʒu�̐������Ԍ��炷�}�X�Ƀv���C���[�����������Ƃ����邩
	bool IsTimeLimitDecCheck(int _x, int _y);
	//�����Ŏw�肳�ꂽ�ʒu�̐������Ԍ��炷�}�X��isStand�̃Z�b�^�[
	void SetIsStand(int _x, int _y, bool _isStand);

	void Write();
};

