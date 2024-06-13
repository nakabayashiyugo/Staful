#include "MapEditScene.h"

#include "../Engine/SceneManager.h"
#include "../Engine/Image.h"
#include "../Engine/Input.h"
#include "../Engine/Text.h"

#include "../resource.h"
#include "../Button.h"
#include "../GamePlayer.h"

#include "SceneTransition.h"
#include "PlayScene.h"

//マスの位置の初期値
const XMFLOAT3 mathInitPos = XMFLOAT3(-1, -1, 0);
//それぞれのコスト
const int floorCost = 0;
const int wallCost = 1;
const int holeCost = 1;
const int converyerCost = 1;
const int togetogeCost = 1;
const int pitfallCost = 1;
const int timeLimitDecCost = 1;
const int confusionCost = 1;
const int startCost = 0;
const int goalCost = 0;

MapEditScene::MapEditScene(GameObject* parent)
	: GameObject(parent, "MapEditScene"),
	//マウス操作
	mousePos_(0, 0, 0),
	//コスト管理について
	curCost_(0),
	//マスの選択について
	mathtype_(0),
	//テストプレイについて
	isClear_(false),
	canTest_(false),
	isDisp_(true),
	//とげとげについて
	tgtgRouteMathDown_(mathInitPos),
	tgtgRouteMathUp_(mathInitPos),
	hTgtgRoute_(-1)
{
	for (int i = 0; i < MATHTYPE::MATH_MAX; i++)
	{
		hPict_[i] = -1;	
		hExpantion_[i] = -1;
	}
	MathVolumeRead();

	Math_Resize(mathVolume_.x, mathVolume_.z, &math_);
	Math_Resize(mathVolume_.x, mathVolume_.z, &math_origin_);
	Math_Resize(mathVolume_.x, mathVolume_.z, &isConvRot_);

	pGP_ = (GamePlayer*)this->pParent_;
	saveNum_ = pGP_->GetSaveNum();

	Read();

	//それぞれのマスのコスト設定
	costs_.resize(MATH_MAX);

	costs_ = { 
		floorCost, 
		wallCost, 
		holeCost, 
		converyerCost, 
		togetogeCost, 
		pitfallCost, 
		timeLimitDecCost,
		confusionCost,
		startCost, 
		goalCost };

	//障害物のおける制限調整
	int costLimitFirst;
	int costLimitPlus;
	if (mathVolume_.x >= mathVolume_.z)
	{
		costLimitFirst = mathVolume_.x;
		costLimitPlus = mathVolume_.x / 2;
	}
	else
	{
		costLimitFirst = mathVolume_.z;
		costLimitPlus = mathVolume_.z / 2;
	}
	turnNum_ = pGP_->GetTurnNum();
	costLimit_ = costLimitFirst + (turnNum_ - 1) * costLimitPlus;
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
	//カーソルが置かれてるマスの位置
	static XMFLOAT3 selectMath;
	
	MousePosSet();

	selectMath.x = (int)(mousePos_.x / MATHSIZE);
	selectMath.y = mathVolume_.z - 1 - (int)(mousePos_.y / MATHSIZE);

	//マウスの位置がマス目から出たら
	if (selectMath.x < 0 || selectMath.x >= mathVolume_.x ||
		selectMath.y < 0 || selectMath.y >= mathVolume_.z)
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
					if (turnNum_ == 1)
					{
						//スタートマスがすでにあるかどうか探索
						for (int x = 0; x < mathVolume_.x; x++)
						{
							for (int y = 0; y < mathVolume_.z; y++)
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
					if (turnNum_ == 1)
					{
						//ゴールマスがすでにあるかどうか探索
						for (int x = 0; x < mathVolume_.x; x++)
						{
							for (int y = 0; y < mathVolume_.z; y++)
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
						tgtgRouteMathDown_ = XMFLOAT3((int)selectMath.x, (int)selectMath.y, 0);
						ChangeSelectMath(selectMath);

						//tTgtgRouteの中に位置が同じなとげとげが存在してるか
						auto itr = tTgtgRoute_.begin();
						while (itr != tTgtgRoute_.end())
						{
							if (itr->initPos_.x == tgtgRouteMathDown_.x &&
								itr->initPos_.y == tgtgRouteMathDown_.y)
							{
								break;
							}
							itr++;
						}
						//存在しなかったら
						if (itr == tTgtgRoute_.end())
						{
							//tTgtgRoute_に追加
							TOGETOGEROUTE* ptg = new TOGETOGEROUTE();
							ptg->initPos_ = ptg->destPos_ = tgtgRouteMathDown_;
							ptg->route_.scale_ = XMFLOAT3(0, 0, 0);
							tTgtgRoute_.push_back(*ptg);
							delete ptg;
						}
					}
				}
				if (Input::IsMouseButtonDown(1))
				{
					if (math_[(int)selectMath.x][(int)selectMath.y].mathType_ == MATH_TOGETOGE)
					{
						tgtgRouteMathDown_ = XMFLOAT3((int)selectMath.x, (int)selectMath.y, 0);
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

	//とげとげルート設定
	TogetogeRouteSet();
	//とげとげマスがほかのマスに変わった時の要素の削除
	TogetogeElemDelete();
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
		pGP_->PlayerNumDraw();
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
			for (int x = 0; x < mathVolume_.x; x++)
			{
				for (int y = 0; y < mathVolume_.z; y++)
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

void MapEditScene::MousePosSet()
{
	//マウスの位置
	mousePos_.x = Input::GetMousePosition().x;
	mousePos_.y = Input::GetMousePosition().y;

	mousePos_.x -= ((math_[0][0].mathPos_.position_.x + 1.0f) * Direct3D::bfr_scrWidth / 2) - MATHSIZE / 2;
	mousePos_.y -= ((-(math_[mathVolume_.x - 1][mathVolume_.z - 1].mathPos_.position_.y) + 1.0f) * Direct3D::bfr_scrHeight / 2) - MATHSIZE / 2;
}

bool MapEditScene::CostManagement()
{
	//コストの合計
	int costSum = 0;
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.z; y++)
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
	std::string buttonName[MATH_MAX];
	for(int i = 0; i < MATH_MAX; i++)
	{
		if (i == (int)MATH_FLOOR)
		{
			buttonName[i] = "Button_Delete.png";
		}
		else
		{
			buttonName[i] = "Button_" + fileNameInit_[i] + ".png";
		}
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
		mbPos.y = -(((float)((buttonNum_ - buttonInitNum) / mbNewLineNum * mbNewLineNum) / Direct3D::bfr_scrHeight) * mathButtonSize) + mbInitPos.y;
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
	std::string expantionName[MATH_MAX];
	for(int i = 0; i < MATH_MAX; i++)
	{
		if (i == (int)MATH_FLOOR)
		{
			expantionName[i] = "";
		}
		else
		{
			expantionName[i] = "Expantion_" + fileNameInit_[i] + ".png";
		}
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
	std::string filename[MATH_MAX];
	for(int i = 0; i < MATH_MAX; i++)
	{
		filename[i] = "Math_" + fileNameInit_[i] + ".png";
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
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.z; y++)
		{
			math_origin_[x][y] = math_[x][y];
			math_[x][y].mathPos_.scale_ = XMFLOAT3(1.0f / imageSize.x * MATHSIZE, 1.0f / imageSize.y * MATHSIZE, 1);
			math_[x][y].mathPos_.position_.x = ((float)x / Direct3D::bfr_scrWidth) * MATHSIZE + ((float)(x - mathVolume_.x) / Direct3D::bfr_scrWidth) * MATHSIZE;
			math_[x][y].mathPos_.position_.y = ((float)y / Direct3D::bfr_scrHeight) * MATHSIZE + ((float)(y - mathVolume_.z) / Direct3D::bfr_scrHeight) * MATHSIZE;
		}
	}
}

void MapEditScene::MathDraw()
{
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.z; y++)
		{
			//コンベアの回転
			if (isConvRot_[x][mathVolume_.z - 1 - y])
			{
				math_[x][mathVolume_.z - 1 - y].mathPos_.rotate_.z += 5;
			}
			if ((int)math_[x][mathVolume_.z - 1 - y].mathPos_.rotate_.z % 90 == 0)
			{
				math_[x][mathVolume_.z - 1 - y].mathPos_.rotate_.z = (int)(math_[x][mathVolume_.z - 1 - y].mathPos_.rotate_.z / 90) * 90;
				isConvRot_[x][mathVolume_.z - 1 - y] = false;
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
		pGP_->SetIsSceneFinished(true);
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
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.z; y++)
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

void MapEditScene::TogetogeRouteSet()
{
	//とげとげマスでクリックして話したとき
	if (Input::IsMuoseButtonUp(1))
	{
		if (tgtgRouteMathDown_.x != -1)
		{
			tgtgRouteMathUp_ = XMFLOAT3((int)mousePos_.x / MATHSIZE, mathVolume_.z - 1 - (int)(mousePos_.y / MATHSIZE), 0);

			auto itr = tTgtgRoute_.begin();

			while (itr != tTgtgRoute_.end())
			{
				if (itr->initPos_.x == tgtgRouteMathDown_.x &&
					itr->initPos_.y == tgtgRouteMathDown_.y)
				{
					break;
				}
				itr++;
			}
			//とげとげルートの太さ
			const int tgtgRouteThick = 5;
			//縦移動
			if (abs(tgtgRouteMathUp_.x - tgtgRouteMathDown_.x) < abs(tgtgRouteMathUp_.y - tgtgRouteMathDown_.y))
			{

				itr->route_.scale_.x = math_[0][0].mathPos_.scale_.x / tgtgRouteThick;
				itr->route_.scale_.y = math_[0][0].mathPos_.scale_.y * abs(tgtgRouteMathUp_.y - tgtgRouteMathDown_.y);

				itr->route_.position_ = math_[(int)tgtgRouteMathDown_.x][((int)tgtgRouteMathUp_.y + tgtgRouteMathDown_.y) / 2].mathPos_.position_;

				if (((int)tgtgRouteMathUp_.y + (int)tgtgRouteMathDown_.y) % 2 != 0)
				{
					itr->route_.position_.y += (math_[0][0].mathPos_.scale_.y) / 2;
				}
				itr->destPos_.y = tgtgRouteMathUp_.y;
				itr->destPos_.x = tgtgRouteMathDown_.x;
			}
			//横移動
			else
			{
				itr->route_.scale_.x = math_[0][0].mathPos_.scale_.x * abs(tgtgRouteMathUp_.x - tgtgRouteMathDown_.x);
				itr->route_.scale_.y = math_[0][0].mathPos_.scale_.y / tgtgRouteThick;

				itr->route_.position_ = math_[((int)tgtgRouteMathUp_.x + (int)tgtgRouteMathDown_.x) / 2][(int)tgtgRouteMathDown_.y].mathPos_.position_;

				if (((int)tgtgRouteMathUp_.x + (int)tgtgRouteMathDown_.x) % 2 != 0)
				{
					itr->route_.position_.x += (math_[0][0].mathPos_.scale_.x) / 2;
				}
				itr->destPos_.x = tgtgRouteMathUp_.x;
				itr->destPos_.y = tgtgRouteMathDown_.y;
			}

			if (itr->route_.scale_.x <= 0 && itr->route_.scale_.y <= 0)
			{
				tTgtgRoute_.erase(itr);
			}
		}
		tgtgRouteMathDown_ = mathInitPos;
	}
}

void MapEditScene::TogetogeElemDelete()
{
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
