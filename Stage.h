#include "StageOrigin.h"

class PlayScene;
class Togetoge;

//Floorのモデルの高さ
const int MODELHEIGHT = 1;

class Stage
	:public GameObject, StageOrigin
{
	int hModel_[MATH_MAX];
	int saveNum_;

	bool isStandPitfall_;

	PlayScene* pPlayScene_;

	std::vector<Togetoge*> pTgtg_;
	std::vector<std::vector<int>> makeHoleTimes_;

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

	void Write();
};

