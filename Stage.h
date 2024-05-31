#include "StageOrigin.h"

class PlayScene;
class Togetoge;

struct TimeLimitDecElem
{
	int x;
	int y;
	bool isStand = false;	//このマスにプレイヤーが立ったことがあるか
};

class Stage
	:public GameObject, StageOrigin
{
	int hModel_[MATH_MAX];
	int saveNum_;

	bool isStandPitfall_;

	PlayScene* pPlayScene_;

	std::vector<Togetoge*> pTgtg_;
	//落とし穴
	std::vector<std::vector<int>> makeHoleTimes_;
	//制限時間短くするやつ
	std::vector<TimeLimitDecElem> timeLimitDecs_;

	XMFLOAT3 tgtgGivePos_;
public:
	Stage(GameObject* parent);
	~Stage();

	//初期化
	void Initialize() override;

	//更新
	void Update() override;

	//描画
	void Draw() override;

	//開放
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

	//引数で指定された位置の制限時間減らすマスにプレイヤーが立ったことがあるか
	bool IsTimeLimitDecCheck(int _x, int _y);
	//引数で指定された位置の制限時間減らすマスのisStandのセッター
	void SetIsStand(int _x, int _y, bool _isStand);

	void Write();
};

