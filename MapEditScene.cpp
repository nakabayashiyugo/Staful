#include "MapEditScene.h"
#include "Engine/SceneManager.h"
#include "Engine/Image.h"
#include "Engine/Input.h"
#include "resource.h"
#include "SceneTransition.h"
#include "Engine/Text.h"

MapEditScene::MapEditScene(GameObject* parent)
	: GameObject(parent, "MapEditScene"), mathtype_(0), saveNum_(2), YSIZE(ZSIZE), hTgtgRoute_(-1),
	mathChangeNum_(0)
{
	for (int i = 0; i < MATHTYPE::MATH_MAX; i++)
	{
		hPict_[i] = -1;
	}
	pTrans_ = (SceneTransition*)FindObject("SceneTransition");
	XSIZE =  (int)pTrans_->GetMathSize_x();
	YSIZE =  (int)pTrans_->GetMathSize_z();

	Math_Resize(XSIZE, YSIZE, &math_);
	Math_Resize(XSIZE, YSIZE, &math_origin_);
	Math_Resize(XSIZE, YSIZE, &isConvRot_);

	Texture* pTexture = (Texture*)FindObject("Texture");

	saveNum_ = pTrans_->GetSaveNum();
	int mathChangeNumLimitFirst;
	int mathChangeNumLimitPlus;
	if (XSIZE >= YSIZE)
	{
		mathChangeNumLimitFirst = XSIZE;
		mathChangeNumLimitPlus = XSIZE / 2;
	}
	else
	{
		mathChangeNumLimitFirst = YSIZE;
		mathChangeNumLimitPlus = YSIZE / 2;
	}

	mathChangeNumLimit_ = mathChangeNumLimitFirst + (pTrans_->GetTurnNum() - 1) * mathChangeNumLimitPlus;

	Read();
}

void MapEditScene::Initialize()
{
	std::string filename[MATH_MAX] =
	{
		"Math_Floor.png",
		"Math_Wall.png",
		"MATH_Hole.png",
		"Math_Conveyor.png",
		"Math_Togetoge.png",
		"Math_PitFall.png",
		"Math_Start.png",
		"Math_Goal.png",
	};
	for (int i = 0; i < MATH_MAX; i++)
	{
		filename[i] = "Assets\\" + filename[i];
		hPict_[i] = Image::Load(filename[i]);
		assert(hPict_[i] >= 0);
	}
	XMFLOAT3 imageSize = Image::GetTextureSize(hPict_[0]);
	//マスのサイズ調整
	for (int x = 0; x < XSIZE; x++)
	{
		for (int y = 0; y < YSIZE; y++)
		{
			math_origin_[x][y] = math_[x][y];
			math_[x][y].mathPos_.scale_ = XMFLOAT3(1.0f / imageSize.x * MATHSIZE, 1.0f / imageSize.y * MATHSIZE, 1);
			math_[x][y].mathPos_.position_.x = ((float)x / Direct3D::scrWidth) * MATHSIZE + ((float)(x - XSIZE) / Direct3D::scrWidth) * MATHSIZE;
			math_[x][y].mathPos_.position_.y = ((float)y / Direct3D::scrHeight) * MATHSIZE + ((float)(y - YSIZE) / Direct3D::scrHeight) * MATHSIZE;
		}
	}

	hTgtgRoute_ = Image::Load("Assets\\Togetoge_Route.png");
	assert(hTgtgRoute_ >= 0);

	pText_ = new Text();
	pText_->Initialize();
}

void MapEditScene::Update()
{
	//debug出力例
	/*std::string resStr = std::to_string(mousePosX) + '\n';
	OutputDebugString(resStr.c_str());*/

	static XMFLOAT3 selectMath;
	static XMFLOAT3 tgtgRouteMathDown = XMFLOAT3(-1, -1, 0);
	static XMFLOAT3 tgtgRouteMathUp = XMFLOAT3(-1, -1, 0);

	float mousePosX = Input::GetMousePosition().x;
	float mousePosY = Input::GetMousePosition().y;
	mousePosX -= ((math_[0][0].mathPos_.position_.x + 1.0f) * Direct3D::scrWidth / 2) - MATHSIZE / 2;
	mousePosY -= ((-(math_[XSIZE - 1][YSIZE - 1].mathPos_.position_.y) + 1.0f) * Direct3D::scrHeight / 2) - MATHSIZE / 2;

	selectMath.x = mousePosX / MATHSIZE;
	selectMath.y = mousePosY / MATHSIZE;

	if (selectMath.x < 0 || selectMath.x >= XSIZE ||
		selectMath.y < 0 || selectMath.y >= YSIZE)
	{
		selectMath = XMFLOAT3(-1, -1, 0);
	}

	if (selectMath.x != -1 && selectMath.y != -1)
	{
		if (math_origin_[(int)selectMath.x][YSIZE - 1 - (int)selectMath.y].mathType_ == MATH_FLOOR)
		{
			switch ((MATHTYPE)mathtype_)
			{
			case MATH_START:
				if (Input::IsMouseButton(0))
				{
					if (pTrans_->GetTurnNum() == 1)
					{
						for (int x = 0; x < XSIZE; x++)
						{
							for (int y = 0; y < YSIZE; y++)
							{
								if (math_[x][y].mathType_ == MATH_START)
								{
									math_[x][y].mathType_ = MATH_FLOOR;
								}
							}
						}
						math_[(int)selectMath.x][YSIZE - 1 - (int)selectMath.y].mathType_ = MATH_START;
						math_[(int)selectMath.x][YSIZE - 1 - (int)selectMath.y].mathPos_.rotate_ = XMFLOAT3(0, 0, 0);
					}
				}
				break;
			case MATH_GOAL:
				if (Input::IsMouseButton(0))
				{
					if (pTrans_->GetTurnNum() == 1)
					{
						for (int x = 0; x < XSIZE; x++)
						{
							for (int y = 0; y < YSIZE; y++)
							{
								if (math_[x][y].mathType_ == MATH_GOAL)
								{
									math_[x][y].mathType_ = MATH_FLOOR;
								}
							}
						}
						math_[(int)selectMath.x][YSIZE - 1 - (int)selectMath.y].mathType_ = MATH_GOAL;
						math_[(int)selectMath.x][YSIZE - 1 - (int)selectMath.y].mathPos_.rotate_ = XMFLOAT3(0, 0, 0);
					}
				}
				break;
			case MATH_FLOOR:
				if (Input::IsMouseButton(0))
				{
					math_[(int)selectMath.x][YSIZE - 1 - (int)selectMath.y].mathPos_.rotate_ = XMFLOAT3(0, 0, 0);
					math_[(int)selectMath.x][YSIZE - 1 - (int)selectMath.y].mathType_ = (MATHTYPE)mathtype_;
				}
			case MATH_CONVEYOR:
				if (!isMathChangeNumLimit())
				{
					if (Input::IsMouseButtonDown(0))
					{
						if (math_[(int)selectMath.x][YSIZE - 1 - (int)selectMath.y].mathType_ == MATHTYPE::MATH_CONVEYOR)
						{
							isConvRot_[(int)selectMath.x][YSIZE - 1 - (int)selectMath.y] = true;
						}
						else
						{
							math_[(int)selectMath.x][YSIZE - 1 - (int)selectMath.y].mathType_ = (MATHTYPE)mathtype_;
						}
					}
				}
				break;
			case MATH_TOGETOGE:
				if (!isMathChangeNumLimit())
				{
					if (Input::IsMouseButtonDown(0))
					{
						tgtgRouteMathDown = XMFLOAT3((int)selectMath.x, YSIZE - 1 - (int)selectMath.y, 0);
						auto itr = tTgtgRoute_.begin();

						while (itr != tTgtgRoute_.end())
						{
							//押されたマスがとげとげマスだったら
							if (itr->initPos_.x == tgtgRouteMathDown.x &&
								itr->initPos_.y == tgtgRouteMathDown.y)
							{
								std::string resStr = "座標 : " + std::to_string((int)tgtgRouteMathDown.x) + ", " + std::to_string((int)tgtgRouteMathDown.y) + '\n';
								OutputDebugString(resStr.c_str());
								break;
							}
							itr++;
						}
						//ちがったら
						if (itr == tTgtgRoute_.end())
						{
							math_[(int)selectMath.x][YSIZE - 1 - (int)selectMath.y].mathType_ = (MATHTYPE)mathtype_;

							tTgtgRoute_.resize(tTgtgRoute_.size() + 1);
							itr = tTgtgRoute_.end() - 1;
							itr->initPos_ = itr->destPos_ = tgtgRouteMathDown;
							itr->route_.scale_ = XMFLOAT3(0, 0, 0);
						}
					}
				}
				break;
			default:
				if (!isMathChangeNumLimit())
				{
					if (Input::IsMouseButton(0))
					{
						math_[(int)selectMath.x][YSIZE - 1 - (int)selectMath.y].mathPos_.rotate_ = XMFLOAT3(0, 0, 0);
						math_[(int)selectMath.x][YSIZE - 1 - (int)selectMath.y].mathType_ = (MATHTYPE)mathtype_;
					}
				}
				break;
			}
		}

		//とげとげマスでクリックして話したとき
		if (Input::IsMuoseButtonUp(0))
		{
			if (tgtgRouteMathDown.x != -1)
			{
				tgtgRouteMathUp = XMFLOAT3((int)mousePosX / MATHSIZE, YSIZE - 1 - (int)(mousePosY / MATHSIZE), 0);

				auto itr = tTgtgRoute_.begin();

				while (itr != tTgtgRoute_.end())
				{
					if (itr->initPos_.x == tgtgRouteMathDown.x &&
						itr->initPos_.y == tgtgRouteMathDown.y)
					{
						break;
					}
					itr++;
				}
				//とげとげルートの太さ
				const int tgtgRouteThick = 5;
				//縦移動
				if (abs(tgtgRouteMathUp.x - tgtgRouteMathDown.x) < abs(tgtgRouteMathUp.y - tgtgRouteMathDown.y))
				{
					
					itr->route_.scale_ =
						XMFLOAT3(math_[0][0].mathPos_.scale_.x / tgtgRouteThick,
							math_[0][0].mathPos_.scale_.y * abs(tgtgRouteMathUp.y - tgtgRouteMathDown.y), 0);

					itr->route_.position_ = math_[(int)tgtgRouteMathDown.x][((int)tgtgRouteMathUp.y + tgtgRouteMathDown.y) / 2].mathPos_.position_;

					if (((int)tgtgRouteMathUp.y + (int)tgtgRouteMathDown.y) % 2 != 0)
					{
						itr->route_.position_.y += (math_[0][0].mathPos_.scale_.y) / 2;
					}
					itr->destPos_.y = tgtgRouteMathUp.y;
					itr->destPos_.x = tgtgRouteMathDown.x;
				}
				//横移動
				else
				{
					itr->route_.scale_ =
						XMFLOAT3(math_[0][0].mathPos_.scale_.x * abs(tgtgRouteMathUp.x - tgtgRouteMathDown.x),
							math_[0][0].mathPos_.scale_.y / tgtgRouteThick, 0);

					itr->route_.position_ = math_[((int)tgtgRouteMathUp.x + (int)tgtgRouteMathDown.x) / 2][(int)tgtgRouteMathDown.y].mathPos_.position_;

					if (((int)tgtgRouteMathUp.x + (int)tgtgRouteMathDown.x) % 2 != 0)
					{
						itr->route_.position_.x += (math_[0][0].mathPos_.scale_.x) / 2;
					}
					itr->destPos_.x = tgtgRouteMathUp.x;
					itr->destPos_.y = tgtgRouteMathDown.y;
				}

				if (itr->route_.scale_.x <= 0 && itr->route_.scale_.y <= 0)
				{
					tTgtgRoute_.erase(itr);
				}
			}
			tgtgRouteMathDown = XMFLOAT3(-1, -1, 0);
		}

		//ルートがあるとげとげのマスが他のマスに変更になったとき
		auto itr = tTgtgRoute_.begin();
		while (itr != tTgtgRoute_.end())
		{
			if (math_[itr->initPos_.x][itr->initPos_.y].mathType_ != MATH_TOGETOGE)
			{
				tTgtgRoute_.erase(itr);
				break;
			}
			itr++;
		}
	}
}


void MapEditScene::Draw()
{
	auto itr = tTgtgRoute_.begin();
	while (itr != tTgtgRoute_.end())
	{
		if (itr->route_.scale_.x != 1)
		{
			Image::SetTransform(hTgtgRoute_, itr->route_);
			Image::Draw(hTgtgRoute_);
			
		}
		itr++;
	}
	std::string str = std::to_string(mathChangeNum_) + " / " + std::to_string(mathChangeNumLimit_);
	pText_->Draw(1000, 700, str.c_str());
	for (int x = 0; x < XSIZE; x++)
	{
		for (int y = 0; y < YSIZE; y++)
		{
			if (isConvRot_[x][YSIZE - 1 - y])
			{
				math_[x][YSIZE - 1 - y].mathPos_.rotate_.z += 5;
			}
			if ((int)math_[x][YSIZE - 1 - y].mathPos_.rotate_.z % 90 == 0)
			{
				math_[x][YSIZE - 1 - y].mathPos_.rotate_.z = (int)(math_[x][YSIZE - 1 - y].mathPos_.rotate_.z / 90) * 90;
				isConvRot_[x][YSIZE - 1 - y] = false;
			}
			
			Image::SetTransform(hPict_[math_[x][y].mathType_], math_[x][y].mathPos_);
			Image::Draw(hPict_[math_[x][y].mathType_]);

		}
	}	
}

void MapEditScene::Release()
{
}

BOOL MapEditScene::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		//ラジオボタンの初期値
		SendMessage(GetDlgItem(hDlg, IDC_MAPEDIT_FLOOR), BM_SETCHECK, BST_CHECKED, 0);

		return TRUE;

	case WM_COMMAND:
		bool startFlg = false, goalFlg = false;
		switch (LOWORD(wp))
		{
		case IDC_MAPEDIT_FLOOR:		mathtype_ = MATH_FLOOR; break;
		case IDC_MAPEDIT_WALL:		mathtype_ = MATH_WALL; break;
		case IDC_MAPEDIT_HOLL:		mathtype_ = MATH_HOLE; break;
		case IDC_MAPEDIT_CONVEYOR:	mathtype_ = MATH_CONVEYOR; break;
		case IDC_MAPEDIT_TOGETOGE:	mathtype_ = MATH_TOGETOGE; break;
		case IDC_MAPEDIT_PITFALL:	mathtype_ = MATH_PITFALL; break;
		case IDC_MAPEDIT_START:		mathtype_ = MATH_START; break;
		case IDC_MAPEDIT_GOAL:		mathtype_ = MATH_GOAL; break;
		case IDC_MAPEDIT_COMPLETE:	
			for (int x = 0; x < XSIZE; x++)
			{
				for (int y = 0; y < YSIZE; y++)
				{
					if (math_[x][y].mathType_ == MATH_START)
					{
						startFlg = true;
					}
					if (math_[x][y].mathType_ == MATH_GOAL)
					{
						goalFlg = true;
					}
				}
			}
			if (startFlg && goalFlg)
			{
				Write();
				EndDialog(hDlg, 0);
			}
			break;
		default: break;
		}

		return TRUE;
	}
	return FALSE;
}

void MapEditScene::Write()
{
	std::ofstream write;
	std::string savefile = "StageSaveFile\\saveMath";
	savefile += std::to_string(saveNum_);
	write.open(savefile, std::ios::out);

	//  ファイルが開けなかったときのエラー表示
	if (!write) {
		std::cout << "ファイル " << savefile << " が開けません";
		return;
	}
	for (int i = 0; i < XSIZE; i++) {
		for (int j = 0; j < YSIZE; j++)
		{
			write.write((char*)&math_[i][j], sizeof(math_[i][j]));
			//文字列ではないデータをかきこむ
		}
	}
	write.close();  //ファイルを閉じる

	//とげとげルート
	savefile = "StageSaveFile\\tgtgRoute";
	savefile += std::to_string(saveNum_);
	write.open(savefile, std::ios::out);
	//  ファイルが開けなかったときのエラー表示
	if (!write) {
		std::cout << "ファイル " << savefile << " が開けません";
		return;
	}
	for (int i = 0; i < tTgtgRoute_.size(); i++)
	{
		write.write((char*)&tTgtgRoute_[i], sizeof(tTgtgRoute_[i]));
	}
	write.close();  //ファイルを閉じる

	
	pTrans_->SetNextScene();
	KillMe();
}

void MapEditScene::Read()
{
	std::ifstream read;
	std::string savefile = "StageSaveFile\\saveMath";
	
	savefile += std::to_string(saveNum_);
	read.open(savefile, std::ios::in);
	//  ファイルを開く
	//  ios::in は読み込み専用  ios::binary はバイナリ形式

	if (!read) {
		std::cout << "ファイルが開けません";
		return;
	}
	//  ファイルが開けなかったときの対策

	//ファイルの最後まで続ける
	for (int i = 0; i < XSIZE; i++)
	{
		for (int j = 0; j < YSIZE; j++)
		{
			read.read((char*)&math_[i][j], sizeof(math_[i][j]));
			//文字列ではないデータを読みこむ

		}
	}
	read.close();  //ファイルを閉じる

	//とげとげルート
	savefile = "StageSaveFile\\tgtgRoute";
	savefile += std::to_string(saveNum_);
	read.open(savefile, std::ios::in);
	if (!read) {
		std::cout << "ファイルが開けません";
		return;
	}

	int i = 0;
	while (!read.eof())
	{
		tTgtgRoute_.resize(tTgtgRoute_.size() + 1);
		read.read((char*)&tTgtgRoute_[i], sizeof(tTgtgRoute_[i]));
		i++;
	}
	read.close();  //ファイルを閉じる
}

bool MapEditScene::isMathChangeNumLimit()
{
	int num = 0;
	for (int x = 0; x < XSIZE; x++)
	{
		for (int y = 0; y < YSIZE; y++)
		{
			if (math_[x][y].mathType_ != math_origin_[x][y].mathType_)
			{
				if (math_[x][y].mathType_ != MATH_START &&
					math_[x][y].mathType_ != MATH_GOAL)
				{
					num++;
				}
			}
		}
	}
	mathChangeNum_ = num;
	if (mathChangeNumLimit_ > num)
	{
		return false;
	}
	else
	{
		return true;
	}
}
