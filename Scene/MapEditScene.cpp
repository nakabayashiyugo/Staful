#include "MapEditScene.h"

#include "../Engine/SceneManager.h"
#include "../Engine/Image.h"
#include "../Engine/Input.h"
#include "../Engine/Text.h"

#include "../resource.h"
#include "../Button.h"

#include "SceneTransition.h"
#include "PlayScene.h"

MapEditScene::MapEditScene(GameObject* parent)
	: GameObject(parent, "MapEditScene"),
	//コスト管理について
	curCost_(0),
	//マスの選択について
	mathtype_(0),
	//ファイルの書き込み・読み出しについて
	saveNum_(2),
	//テストプレイについて
	isClear_(false),
	canTest_(false),
	isDisp_(true),

	YSIZE(ZSIZE), 
	hTgtgRoute_(-1)
{
	for (int i = 0; i < MATHTYPE::MATH_MAX; i++)
	{
		hPict_[i] = -1;	
		hExpantion_[i] = -1;
	}
	pST = (SceneTransition*)FindObject("SceneTransition");
	XSIZE =  (int)pST->GetMathSize_x();
	YSIZE =  (int)pST->GetMathSize_z();

	Math_Resize(XSIZE, YSIZE, &math_);
	Math_Resize(XSIZE, YSIZE, &math_origin_);
	Math_Resize(XSIZE, YSIZE, &isConvRot_);

	Texture* pTexture = (Texture*)FindObject("Texture");

	saveNum_ = pST->GetSaveNum();

	//それぞれのマスのコスト設定
	costs_.resize(MATH_MAX);

	costs_ = { 0, 1, 1, 1, 1, 1, 0, 0 };
	//障害物のおける制限調整
	int costLimitFirst;
	int costLimitPlus;
	if (XSIZE >= YSIZE)
	{
		costLimitFirst = XSIZE;
		costLimitPlus = XSIZE / 2;
	}
	else
	{
		costLimitFirst = YSIZE;
		costLimitPlus = YSIZE / 2;
	}
	costLimit_ = costLimitFirst + (pST->GetTurnNum() - 1) * costLimitPlus;

	Read();
}

void MapEditScene::Initialize()
{
	//マス初期化
	MathInit();
	//ボタンの初期化
	ButtonInit();
	//マスの説明の初期化
	ExpantionInit();

	hTgtgRoute_ = Image::Load("Assets\\Togetoge_Route.png");
	assert(hTgtgRoute_ >= 0);

	pText_ = new Text();
	pText_->Initialize();
}

void MapEditScene::Update()
{
	//マスの位置の初期値
	const XMFLOAT3 mathInitPos = XMFLOAT3(-1, -1, 0);
	//カーソルが置かれてるマスの位置
	static XMFLOAT3 selectMath;
	//とげとげマスを押した位置
	static XMFLOAT3 tgtgRouteMathDown = XMFLOAT3(-1, -1, 0);
	//とげとげマスを押してドラッグして話した位置
	static XMFLOAT3 tgtgRouteMathUp = XMFLOAT3(-1, -1, 0);

	//マウスの位置
	float mousePosX = Input::GetMousePosition().x;
	float mousePosY = Input::GetMousePosition().y;

	//x方向のマスの数
	const int xsizeMax = XSIZE - 1;
	//y方向のマスの数
	const int ysizeMax = YSIZE - 1;
	mousePosX -= ((math_[0][0].mathPos_.position_.x + 1.0f) * Direct3D::scrWidth / 2) - MATHSIZE / 2;
	mousePosY -= ((-(math_[xsizeMax][ysizeMax].mathPos_.position_.y) + 1.0f) * Direct3D::scrHeight / 2) - MATHSIZE / 2;

	selectMath.x = (int)(mousePosX / MATHSIZE);
	selectMath.y = ysizeMax - (int)(mousePosY / MATHSIZE);

	//マウスの位置がマス目から出たら
	if (selectMath.x < 0 || selectMath.x >= XSIZE ||
		selectMath.y < 0 || selectMath.y >= YSIZE)
	{
		selectMath = mathInitPos;
	}

	//選ばれてるマスの種類検索
	SelectMathType();

	if (selectMath.x != -1 && selectMath.y != -1)
	{
		if (math_origin_[(int)selectMath.x][(int)selectMath.y].mathType_ == MATH_FLOOR)
		{
			switch ((MATHTYPE)mathtype_)
			{
			case MATH_START:
				if (Input::IsMouseButton(0))
				{
					if (pST->GetTurnNum() == 1)
					{
						//スタートマスがすでにあるかどうか探索
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
						ChangeSelectMath(selectMath);
					}
				}
				break;
			case MATH_GOAL:
				if (Input::IsMouseButton(0))
				{
					if (pST->GetTurnNum() == 1)
					{
						//ゴールマスがすでにあるかどうか探索
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
						ChangeSelectMath(selectMath);
					}
				}
				break;
			case MATH_FLOOR:
				if (Input::IsMouseButton(0))
				{
					ChangeSelectMath(selectMath);
				}
			case MATH_CONVEYOR:
				if (!CostManagement())
				{
					if (Input::IsMouseButton(0))
					{
						ChangeSelectMath(selectMath);
					}
				}
				if (Input::IsMouseButtonDown(1))
				{
					if (math_[(int)selectMath.x][(int)selectMath.y].mathType_ == MATHTYPE::MATH_CONVEYOR)
					{
						isConvRot_[(int)selectMath.x][(int)selectMath.y] = true;
						isClear_ = false;
					}
				}
				break;
			case MATH_TOGETOGE:
				if (Input::IsMouseButton(0))
				{
					if (!CostManagement())
					{
						tgtgRouteMathDown = XMFLOAT3((int)selectMath.x, (int)selectMath.y, 0);
						ChangeSelectMath(selectMath);

						//tTgtgRoute_に追加
						TOGETOGEROUTE* ptg = new TOGETOGEROUTE();
						ptg->initPos_ = ptg->destPos_ = tgtgRouteMathDown;
						ptg->route_.scale_ = XMFLOAT3(0, 0, 0);
						tTgtgRoute_.push_back(*ptg);
						delete ptg;
					}
				}
				if (Input::IsMouseButtonDown(1))
				{
					if (math_[(int)selectMath.x][(int)selectMath.y].mathType_ == MATH_TOGETOGE)
					{
						tgtgRouteMathDown = XMFLOAT3((int)selectMath.x, (int)selectMath.y, 0);
						isClear_ = false;
					}
				}
				break;
			default:
				if (!CostManagement())
				{
					if (Input::IsMouseButton(0))
					{
						ChangeSelectMath(selectMath);
					}
				}
				break;
			}
		}
	}


	//とげとげマスでクリックして話したとき
	if (Input::IsMuoseButtonUp(1))
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

				itr->route_.scale_.x = math_[0][0].mathPos_.scale_.x / tgtgRouteThick;
				itr->route_.scale_.y = math_[0][0].mathPos_.scale_.y * abs(tgtgRouteMathUp.y - tgtgRouteMathDown.y);

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
				itr->route_.scale_.x = math_[0][0].mathPos_.scale_.x * abs(tgtgRouteMathUp.x - tgtgRouteMathDown.x);
				itr->route_.scale_.y = math_[0][0].mathPos_.scale_.y / tgtgRouteThick;

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
		tgtgRouteMathDown = mathInitPos;
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



void MapEditScene::Draw()
{
	IsDisplay(isDisp_);
	if(isDisp_)
	{
		//とげとげルート表示
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
		//コスト表示
		CostDraw();

		//マスの説明表示
		ExpantionDraw();
		//マス表示
		MathDraw();

		//プレイヤー番号表示
		pST->PlayerNumDraw();
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
	std::string savefile = saveFolderName + "saveMath";
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
	savefile = saveFolderName + "tgtgRoute";
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
}

void MapEditScene::Read()
{
	std::ifstream read;
	std::string savefile = saveFolderName + "saveMath";
	
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
	savefile = saveFolderName + "tgtgRoute";
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

bool MapEditScene::CostManagement()
{
	//コストの合計
	int costSum = 0;
	for (int x = 0; x < XSIZE; x++)
	{
		for (int y = 0; y < YSIZE; y++)
		{
			costSum += costs_[(int)math_[x][y].mathType_];
		}
	}
	curCost_ = costSum;
	if (costLimit_ > curCost_)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void MapEditScene::CostDraw()
{
	const std::string strCost = "cost";
	const std::string strCostLimit = "costLimit";
	//文字「cost」の表示位置
	XMFLOAT3 costPos;
	//文字「costLimit」の表示位置
	XMFLOAT3 costLimitPos;
	//コストの表示位置
	XMFLOAT3 curCostPos;
	//コスト表示の基準の位置
	const XMFLOAT3 costPosBase(900, 600, 0);
	//上の3つの位置のそれぞれの差
	const XMFLOAT3 posDiff = XMFLOAT3(50, 30, 0);

	costPos = costPosBase;
	curCostPos = XMFLOAT3(costPos.x + posDiff.x, costPos.y + posDiff.y, costPosBase.z);
	costLimitPos = XMFLOAT3(curCostPos.x + posDiff.x, curCostPos.y + posDiff.y, costPosBase.z);
	//文字「cost」表示
	pText_->Draw(costPos.x, costPos.y, strCost.c_str());
	//コスト表示
	std::string str = std::to_string(curCost_) + " / " + std::to_string(costLimit_);
	pText_->Draw(curCostPos.x, curCostPos.y, str.c_str());
	//文字「costLimit」表示
	pText_->Draw(costLimitPos.x, costLimitPos.y, strCostLimit.c_str());
}

void MapEditScene::ChangeSelectMath(XMFLOAT3 _selectMath)
{
	//クリックしたマスを選んでるマスに変える
	if (math_[(int)_selectMath.x][(int)_selectMath.y].mathType_ != MATH_CONVEYOR)
	{
		math_[(int)_selectMath.x][(int)_selectMath.y].mathPos_.rotate_ = XMFLOAT3(0, 0, 0);
	}
	math_[(int)_selectMath.x][(int)_selectMath.y].mathType_ = (MATHTYPE)mathtype_;
	isClear_ = false;

	//テストプレイできるかどうかチェック
	CheckCanTest();
}

void MapEditScene::ButtonInit()
{
	//マス選択ボタンのファイルネーム指定
	std::string buttonName[MATH_MAX] =
	{
		"Button_Delete.png",
		"Button_Wall.png",
		"Button_Hole.png",
		"Button_Conveyor.png",
		"Button_Togetoge.png",
		"Button_PitFall.png",
		"Button_Start.png",
		"Button_Goal.png",
	};
	//その他のボタン
	//完了ボタン
	std::string buttonComplete = "Button_Complete.png";
	//テストプレイボタン
	std::string buttonTestplay = "Button_TestPlay.png";
	//上のファイルが入ってるフォルダー
	std::string folderName = "Assets\\Button\\MathSelect\\";
	//マス選択ボタンの画像番号
	int mathButtonNum[MATH_MAX];
	//その他のボタンの画像番号
	int completeNum = -1;
	int testplayNum = -1;
	//マス選択ボタンのロード
	for (int i = 0; i < MATH_MAX; i++)
	{
		buttonName[i] = folderName + buttonName[i];

		mathButtonNum[i] = Image::Load(buttonName[i]);
		assert(mathButtonNum[i] >= 0);
	}
	//ボタンのテクスチャサイズ
	XMFLOAT3 imageSize = Image::GetTextureSize(mathButtonNum[0]);
	//その他のボタンが入ってるフォルダ名
	folderName = "Assets\\Button\\SceneTrans\\";
	//完了ボタンのロード
	buttonComplete = folderName + buttonComplete;
	completeNum = Image::Load(buttonComplete);
	assert(completeNum >= 0);
	//テストプレイボタンのロード
	buttonTestplay = folderName + buttonTestplay;
	testplayNum = Image::Load(buttonTestplay);
	assert(testplayNum >= 0);

	//ボタンの普通のマスとの倍率
	const float normalMathtoMult = 2.0f;
	//ボタンのサイズ
	const float mathButtonSize = MATHSIZE * normalMathtoMult;
	//ボタンの大きさ設定
	const XMFLOAT3 mbScale = XMFLOAT3(1.0f / imageSize.x * mathButtonSize, 1.0f / imageSize.y * mathButtonSize, 1);
	const int buttonInitNum = 1;
	//ボタンのオブジェクトネーム
	std::string buttonStr;
	for (buttonNum_ = buttonInitNum; buttonNum_ < MATH_MAX; buttonNum_++)
	{
		pMathButton_[buttonNum_]->Instantiate<Button>(this);
		//探すボタンのオブジェクトネーム
		buttonStr = "Button";
		buttonStr += std::to_string(buttonNum_);
		pMathButton_[buttonNum_] = (Button*)FindObject(buttonStr);
		pMathButton_[buttonNum_]->SetPictNum(mathButtonNum[buttonNum_]);

		
		//マス選択ボタンの基準の位置
		const XMFLOAT3 mbInitPos = XMFLOAT3(-0.9f, 0.6f, 0);
		//マス選択ボタンが何個ごとに改行されるか
		const int mbNewLineNum = 4;
		//マス選択ボタンの位置
		XMFLOAT3 mbPos;
		mbPos.x = (float)((buttonNum_ - buttonInitNum) % mbNewLineNum) * mbScale.x + mbInitPos.x;
		mbPos.y = -(((float)((buttonNum_ - buttonInitNum) / mbNewLineNum * mbNewLineNum) / Direct3D::scrHeight) * mathButtonSize) + mbInitPos.y;
		mbPos.z = mbInitPos.z;

		Transform mbTransform;
		mbTransform.position_ = mbPos;
		mbTransform.scale_ = mbScale;
		pMathButton_[buttonNum_]->SetTransform(mbTransform);
	}

	//Deleteボタン
	//Deleteボタンの番号
	buttonNum_ = (int)MATH_FLOOR;

	pMathButton_[buttonNum_]->Instantiate<Button>(this);
	//Deleteボタンのオブジェクトネーム
	buttonStr = "Button";
	buttonStr += std::to_string(buttonNum_);
	pMathButton_[buttonNum_] = (Button*)FindObject(buttonStr);
	pMathButton_[buttonNum_]->SetPictNum(mathButtonNum[buttonNum_]);

	//マス選択ボタンの基準の位置
	const XMFLOAT3 dbPos = XMFLOAT3(-0.9f, -0.3f, 0);
	
	Transform dbTransform;
	dbTransform.position_ = dbPos;
	dbTransform.scale_ = mbScale;
	pMathButton_[buttonNum_]->SetTransform(dbTransform);

	//その他のボタンの大きさ
	const XMFLOAT3 obScale = XMFLOAT3(0.3f, 0.3f, 1);
	//完了ボタン
	//完了ボタンの番号
	buttonNum_ = (int)MATH_MAX;
	pCompleteButton_->Instantiate<Button>(this);
	//完了ボタンのオブジェクトネーム
	buttonStr = "Button";
	buttonStr += std::to_string(buttonNum_);
	pCompleteButton_ = (Button*)FindObject(buttonStr);
	pCompleteButton_->SetPictNum(completeNum);

	const XMFLOAT3 cbPos = XMFLOAT3(-0.8f, -0.7f, 0);
	Transform cbTransform;
	cbTransform.position_ = cbPos;
	cbTransform.scale_ = obScale;
	pCompleteButton_->SetTransform(cbTransform);

	//テストプレイボタン
	//テストプレイボタンの番号
	buttonNum_++;
	pTestplayButton_->Instantiate<Button>(this);
	//テストプレイボタンのオブジェクトネーム
	buttonStr = "Button";
	buttonStr += std::to_string(buttonNum_);
	pTestplayButton_ = (Button*)FindObject(buttonStr);
	pTestplayButton_->SetPictNum(testplayNum);

	const XMFLOAT3 tbPos = XMFLOAT3(-0.8f, -0.6f, 0);
	Transform tbTransform;
	tbTransform.position_ = tbPos;
	tbTransform.scale_ = obScale;
	pTestplayButton_->SetTransform(tbTransform);
}

void MapEditScene::ExpantionInit()
{
	//マスの説明のファイルネーム
	std::string expantionName[MATH_MAX] =
	{
		"",
		"Expantion_Wall.png",
		"Expantion_Hole.png",
		"Expantion_Conveyor.png",
		"Expantion_Togetoge.png",
		"Expantion_PitFall.png",
		"Expantion_Start.png",
		"Expantion_Goal.png",
	};
	//マスの説明の画像が入ってるフォルダ名
	std::string folderName = "Assets\\MathExpantion\\";
	//マスの説明画像ロード
	for (int i = 1; i < MATH_MAX; i++)
	{
		expantionName[i] = folderName + expantionName[i];

		hExpantion_[i] = Image::Load(expantionName[i]);
		assert(hExpantion_[i] >= 0);
	}
}

void MapEditScene::ExpantionDraw()
{
	const XMFLOAT3 exPos = XMFLOAT3(0.7f, 0.1f, 0);
	const XMFLOAT3 exScale = XMFLOAT3(0.7f, 0.7f, 1);
	tExpantion_.position_ = exPos;
	tExpantion_.scale_ = exScale;

	Image::SetTransform(hExpantion_[mathtype_], tExpantion_);
	Image::Draw(hExpantion_[mathtype_]);
}

void MapEditScene::MathInit()
{
	//マスの画像ロード
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
		std::string folderName = "Assets\\MathType\\";
		filename[i] = folderName + filename[i];
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
}

void MapEditScene::MathDraw()
{
	for (int x = 0; x < XSIZE; x++)
	{
		for (int y = 0; y < YSIZE; y++)
		{
			//コンベアの回転
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

void MapEditScene::SelectMathType()
{
	for (int i = 0; i < MATH_MAX; i++)
	{
		if (pMathButton_[i]->GetIsClicked())
		{
			mathtype_ = i;
		}
	}
	pTestplayButton_->SetIsCanPush(canTest_ * isDisp_);
	//テストプレイボタンが押されたら
	if (pTestplayButton_->GetIsClicked())
	{
		pTestplayButton_->SetIsClicked(false);
		//マップエディター非表示
		isDisp_ = false;
		Write();
		Instantiate<PlayScene>(this);
	}
	//テストプレイクリアできないと押せないようにする
	pCompleteButton_->SetIsCanPush(isClear_ * isDisp_);
	//完了ボタンが押されたら
	if (pCompleteButton_->GetIsClicked())
	{
		Write();
		pST->SetNextScene();
		KillMe();
	}
}

void MapEditScene::IsDisplay(bool _isDisp)
{
	//ボタン表示非表示切り替え
	for (int i = 0; i < MATH_MAX; i++)
	{
		pMathButton_[i]->SetIsDisplay(_isDisp);
	}
	//完了ボタン表示非表示切り替え
	pCompleteButton_->SetIsDisplay(_isDisp);
	//テストプレイボタン表示非表示切り替え
	pTestplayButton_->SetIsDisplay(_isDisp);
}

void MapEditScene::CheckCanTest()
{
	//スタートマスとゴールマスをおいてないとテストプレイをできないようにする
	bool isStart = false, isGoal = false;
	for (int x = 0; x < XSIZE; x++)
	{
		for (int y = 0; y < YSIZE; y++)
		{
			if (math_[x][y].mathType_ == MATH_START)
			{
				isStart = true;
			}
			if (math_[x][y].mathType_ == MATH_GOAL)
			{
				isGoal = true;
			}
		}
	}
	if (isStart && isGoal)
	{
		canTest_ = true;
	}
	else
	{
		canTest_ = false;
	}
}
