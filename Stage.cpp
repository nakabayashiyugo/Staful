#include <string>
#include <vector>
#include <array>

#include "Engine/Input.h"
#include "Engine/Model.h"
#include "Engine/Direct3D.h"
#include "Engine/RootJob.h"
#include "Engine/Camera.h"

#include "Scene/SceneTransition.h"
#include "Scene/PlayScene.h"

#include "Stage.h"
#include "Togetoge.h"

#include "resource.h"

const float MAKEHOLETIME = 0.5f;

Stage::Stage(GameObject* parent)
	: GameObject(parent, "Stage"), isStandPitfall_(false),
	tgtgGivePos_(0, 0, 0)
{
	for (int i = 0; i < 5; i++)
	{
		hModel_[i] = -1;
	}
	
	MathVolumeRead();

	Math_Resize(mathVolume_.x, mathVolume_.z, &math_);
	Math_Resize(mathVolume_.x, mathVolume_.z, &makeHoleTimes_);

	Read();

	pPlayScene_ = (PlayScene*)FindObject("PlayScene");
	SetTableMath(pPlayScene_->GetTableMath());
	SetTogetogeRoute(pPlayScene_->GetTogetogeRoute());

	saveNum_ = pPlayScene_->GetSaveNum();
	
	for (int x = 0; x < makeHoleTimes_.size(); x++)
	{
		for (int z = 0; z < makeHoleTimes_[x].size(); z++)
		{
			makeHoleTimes_[x][z] = FPS * MAKEHOLETIME;
		}
	}
}

Stage::~Stage()
{
	for (int i = 0; i < pTgtg_.size(); i++)
	{
		pTgtg_[i]->KillMe();
	}
}

void Stage::Initialize()
{
	//モデルファイルネーム
	std::string modelName[MATH_MAX];
	for (int i = 0; i < MATH_MAX; i++)
	{
		modelName[i] = "Block_" + fileNameInit_[i] + ".fbx";
	};

	std::string fname_base = "Assets\\Block\\";
	//モデルデータのロード
	for (int i = 0; i < MATH_MAX; i++)
	{
		hModel_[i] = Model::Load(fname_base + modelName[i]);
		assert(hModel_[i] >= 0);
	}

	//とげとげ出す
	pTgtg_.resize(tTgtgRoute_.size());
	for (int i = 0; i < pTgtg_.size(); i++)
	{
		tgtgGivePos_ = tTgtgRoute_[i].initPos_;
		pTgtg_[i]->Instantiate<Togetoge>(this);
	}

	//制限時間減らすマスが何個あるか
	for (int i = 0; i < math_.size(); i++)
	{
		for (int j = 0; j < math_[i].size(); j++)
		{
			if (math_[i][j].mathType_ == MATH_TIMELIMITDEC)
			{
				TimeLimitDecElem tlde;
				tlde.x = i;
				tlde.y = j;
				tlde.isStand = false;
				timeLimitDecs_.push_back(tlde);
			}
		}
	}
}


void Stage::Update()
{
	MATHDEDAIL pStandMath = pPlayScene_->GetPlayerStandMath();
	if (pStandMath.mathType_ == MATH_PITFALL)
	{
		XMFLOAT3 playerPos = pPlayScene_->GetPlayerPos();
		makeHoleTimes_[(int)playerPos.x][(int)playerPos.z]--;
		if (makeHoleTimes_[(int)playerPos.x][(int)playerPos.z] <= 0)
		{
			math_[(int)playerPos.x][(int)playerPos.z].mathType_ = MATH_HOLE;
			Write();
			Read();
			pPlayScene_->SetTableChange(true);
		}
	}
}

void Stage::Draw()
{
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int z = 0; z < mathVolume_.z; z++)
		{
			Transform mathTrans;
			
			mathTrans.position_ = XMFLOAT3(x, pPlayScene_->GetFloorHeight(), z);

			//コンベアの初期のrotate
			const int convRotInit = 180;

			switch (math_[x][z].mathType_)
			{
			case MATH_FLOOR:
				Model::SetTransform(hModel_[math_[x][z].mathType_], mathTrans);
				Model::Draw(hModel_[math_[x][z].mathType_]);
				break;
			case MATH_WALL :
				Model::SetTransform(hModel_[MATH_FLOOR], mathTrans);
				Model::Draw(hModel_[MATH_FLOOR]);
				mathTrans.position_.y += 1;
				Model::SetTransform(hModel_[math_[x][z].mathType_], mathTrans);
				Model::Draw(hModel_[math_[x][z].mathType_]);
				break;
			case MATH_CONVEYOR:
				mathTrans.rotate_.y = convRotInit;
				mathTrans.rotate_.y += -math_[x][z].mathPos_.rotate_.z;
				Model::SetTransform(hModel_[math_[x][z].mathType_], mathTrans);
				Model::Draw(hModel_[math_[x][z].mathType_]);
				break;
			case MATH_TOGETOGE:
				Model::SetTransform(hModel_[MATH_FLOOR], mathTrans);
				Model::Draw(hModel_[MATH_FLOOR]);
				mathTrans.position_.y += 1;
				break;
			case MATH_PITFALL:
				if (makeHoleTimes_[x][z] >= FPS * MAKEHOLETIME / 2)
				{
					Model::SetTransform(hModel_[MATH_FLOOR], mathTrans);
					Model::Draw(hModel_[MATH_FLOOR]);
				}
				else
				{
					Model::SetTransform(hModel_[math_[x][z].mathType_], mathTrans);
					Model::Draw(hModel_[math_[x][z].mathType_]);
				}
				break;
			case MATH_TIMELIMITDEC:
				if (IsTimeLimitDecCheck(x, z))
				{
					Model::SetTransform(hModel_[math_[x][z].mathType_], mathTrans);
					Model::Draw(hModel_[math_[x][z].mathType_]);
				}
				else
				{
					Model::SetTransform(hModel_[MATH_FLOOR], mathTrans);
					Model::Draw(hModel_[MATH_FLOOR]);
				}
				break;
			case MATH_CONFUSION:
				Model::SetTransform(hModel_[math_[x][z].mathType_], mathTrans);
				Model::Draw(hModel_[math_[x][z].mathType_]);
				break;
			case MATH_START:
				Model::SetTransform(hModel_[MATH_FLOOR], mathTrans);
				Model::Draw(hModel_[MATH_FLOOR]);
				mathTrans.position_.y += 1;
				Model::SetTransform(hModel_[math_[x][z].mathType_], mathTrans);
				Model::Draw(hModel_[math_[x][z].mathType_]);
				break;
			case MATH_GOAL:
				Model::SetTransform(hModel_[MATH_FLOOR], mathTrans);
				Model::Draw(hModel_[MATH_FLOOR]);
				mathTrans.position_.y += 1;
				Model::SetTransform(hModel_[math_[x][z].mathType_], mathTrans);
				Model::Draw(hModel_[math_[x][z].mathType_]);
				break;
			}
		}
	}
	
}

void Stage::Release()
{
}

void Stage::SetBlock(int x, int z, MATHTYPE _type)
{
	math_.at(x).at(z).mathType_ = _type;
	assert(_type <= MATHTYPE::MATH_MAX);
}

bool Stage::IsTimeLimitDecCheck(int _x, int _y)
{
	bool ret = false;
	for (int i = 0; i < timeLimitDecs_.size(); i++)
	{
		if (timeLimitDecs_[i].x == _x && timeLimitDecs_[i].y == _y)
		{
			ret = timeLimitDecs_[i].isStand;
		}
	}
	return ret;
}

void Stage::SetIsStand(int _x, int _y, bool _isStand)
{
	for (int i = 0; i < timeLimitDecs_.size(); i++)
	{
		if (timeLimitDecs_[i].x == _x && timeLimitDecs_[i].y == _y)
		{
			timeLimitDecs_[i].isStand = _isStand;
		}
	}
}

void Stage::Write()
{
	std::ofstream write;
	std::string savefile = saveFolderName + "saveMath";
	savefile += std::to_string(saveNum_);
	write.open(savefile, std::ios::out);

	//  ファイルが開けなかったときのエラー表示
	if (!write) {
		std::cout << "ファイル " << savefile << " が開けません";
		return;
	}

	for (int i = 0; i < mathVolume_.x; i++) {
		for (int j = 0; j < mathVolume_.z; j++)
		{
			write.write((char*)&math_[i][j], sizeof(math_[i][j]));
			//文字列ではないデータをかきこむ
		}
	}

	write.close();  //ファイルを閉じる

	//とげとげルート
	savefile = saveFolderName + "tgtgRoute";
	savefile += std::to_string(saveNum_);
	write.open(savefile, std::ios::out);
	//  ファイルが開けなかったときのエラー表示
	if (!write) {
		std::cout << "ファイル " << savefile << " が開けません";
		return;
	}
	for (auto itr = tTgtgRoute_.begin(); itr != tTgtgRoute_.end(); itr++)
	{
		write.write((char*)&itr, sizeof(itr));
	}
	write.close();  //ファイルを閉じる
}

void Stage::SetTableMath(std::vector<std::vector<MATHDEDAIL>> _math)
{
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int z = 0; z < mathVolume_.z; z++)
		{
			math_.at(x).at(z) = _math.at(x).at(z);
		}
	}
}

XMFLOAT3 Stage::GetTogetogeInitPos()
{
	return tgtgGivePos_;
}


