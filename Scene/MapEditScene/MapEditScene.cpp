#include "MapEditScene.h"

#include "../../Engine/SceneManager.h"
#include "../../Engine/Image.h"
#include "../../Engine/Input.h"
#include "../../Engine/Text.h"
#include "../../Engine/Audio.h"

#include "../../resource.h"
#include "../../ButtonManager.h"
#include "../../Button.h"
#include "../../GamePlayer.h"

#include "../SceneTransition.h"
#include "../PlayScene.h"

#include "Math/MathManager.h"

//マスの位置の初期値
const XMFLOAT3 mathInitPos = XMFLOAT3(-1, -1, 0);
//音楽の大きさ
const float soundVolume = 0.2f;
//それぞれのコスト
const int floorCost = 0;
const int wallCost = 1;
const int holeCost = 1;
const int converyerCost = 1;
const int togetogeCost = 5;
const int pitfallCost = 3;
const int timeLimitDecCost = 10;
const int confusionCost = 8;
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
	hTgtgRoute_(-1),
	//音楽について
	hAudio_Music_(-1)
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
		wallCost, 
		holeCost, 
		converyerCost, 
		togetogeCost, 
		pitfallCost, 
		timeLimitDecCost,
		confusionCost,
		startCost, 
		goalCost,
		floorCost};

	//障害物のおける制限調整
	int costLimitFirst;
	int costLimitPlus;

	costLimitFirst = mathVolume_.x + mathVolume_.z;
	costLimitPlus = costLimitFirst / 2;
	turnNum_ = pGP_->GetTurnNum();
	costLimit_ = costLimitFirst + (turnNum_ - 1) * costLimitPlus;

	table_ = new MathManager(mathVolume_.x, mathVolume_.z, costs_, costs_.size());
}

void MapEditScene::Initialize()
{
	//マス初期化
	MathInit();
	//ボタンの初期化
	ButtonInit();
	//マスの説明の初期化
	ExpantionInit();
	//音楽の初期化
	AudioInit();

	hTgtgRoute_ = Image::Load("Assets\\Togetoge_Route.png");
	assert(hTgtgRoute_ >= 0);

	pText_ = new Text();
	pText_->Initialize();
}

void MapEditScene::Update()
{
	Audio::Play(hAudio_Music_, soundVolume);
	
	MousePosSet();
	SelectMathSet();
	ButtonUpdate();

	if (selectMath_.x != -1 && selectMath_.y != -1)
	{
		if (math_origin_[(int)selectMath_.x][(int)selectMath_.y].mathType_ == MATH_DELETE)
		{
			XMFLOAT2 select = XMFLOAT2(selectMath_.x, selectMath_.y);
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
								if (table_->GetMathType(XMFLOAT2(x, y)) == MATH_START)
								{
									table_->SetMathType(XMFLOAT2(x, y), MATH_DELETE);
								}
							}
						}
						ChangeSelectMath();
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
								if (table_->GetMathType(XMFLOAT2(x, y)) == MATH_GOAL)
								{
									table_->SetMathType(XMFLOAT2(x, y), MATH_DELETE);
								}
							}
						}
						ChangeSelectMath();
					}
				}
				break;
			case MATH_DELETE:
				if (Input::IsMouseButton(0))
				{
					ChangeSelectMath();
				}
			case MATH_CONVEYOR:
				if (Input::IsMouseButton(0))
				{
					if (CostManagement(selectMath_))
					{
						ChangeSelectMath();
					}
				}
				if (Input::IsMouseButtonDown(1))
				{
					if (math_[(int)selectMath_.x][(int)selectMath_.y].mathType_ == MATHTYPE::MATH_CONVEYOR)
					{
						isConvRot_[(int)selectMath_.x][(int)selectMath_.y] = true;
						isClear_ = false;
					}
				}
				break;
			case MATH_TOGETOGE:
				if (Input::IsMouseButton(0))
				{
					if (CostManagement(selectMath_))
					{
						tgtgRouteMathDown_ = XMFLOAT3((int)selectMath_.x, (int)selectMath_.y, 0);
						ChangeSelectMath();

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
					if (table_->GetMathType(select) == MATH_TOGETOGE)
					{
						tgtgRouteMathDown_ = XMFLOAT3((int)selectMath_.x, (int)selectMath_.y, 0);
						isClear_ = false;
					}
				}
				break;
			default:
				if (Input::IsMouseButton(0))
				{
					if (CostManagement(selectMath_))
					{
						ChangeSelectMath();
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
		case IDC_MAPEDIT_FLOOR:		mathtype_ = MATH_DELETE; break;
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

	mousePos_.x -= ((table_->GetMathTransform(XMFLOAT2(0, 0)).position_.x + 1.0f) * Direct3D::bfr_scrWidth / 2) - MATHSIZE / 2;
	mousePos_.y -= ((-(table_->GetMathTransform(XMFLOAT2(mathVolume_.x - 1, mathVolume_.z - 1)).position_.y) + 1.0f) * Direct3D::bfr_scrHeight / 2) - MATHSIZE / 2;
}

bool MapEditScene::CostManagement(XMFLOAT3 selectMath_)
{
	int costSum = 0;
	costSum = table_->GetStageCost();
	//変更後のコストたす
	if (table_->GetMathType(XMFLOAT2(selectMath_.x, selectMath_.y)) != mathtype_)
	{
		costSum += costs_[mathtype_];
		//コストの制限を超えていたら
		if (costSum > costLimit_)
		{
			costSum -= costs_[mathtype_];
			curCost_ = costSum;
			return false;
		}
	}
	curCost_ = costSum;
	return true;
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
	const XMFLOAT3 costPosBase(300, 650, 0);
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

void MapEditScene::ChangeSelectMath()
{
	//クリックしたマスを選んでるマスに変える
	if (math_[(int)selectMath_.x][(int)selectMath_.y].mathType_ != MATH_CONVEYOR)
	{
		math_[(int)selectMath_.x][(int)selectMath_.y].mathPos_.rotate_ = XMFLOAT3(0, 0, 0);
	}
	math_[(int)selectMath_.x][(int)selectMath_.y].mathType_ = (MATHTYPE)mathtype_;
	isClear_ = false;

	//テストプレイできるかどうかチェック
	CheckCanTest();

	//マスを置いた時のSE
	//直前までカーソルの置かれていたマス
	static XMFLOAT3 prevMath = mathInitPos;
	if (prevMath.x != selectMath_.x || prevMath.y != selectMath_.y)
	{
		Audio::Stop(hSE_PutMath_);
	}
	Audio::Play(hSE_PutMath_, 1);
	prevMath = selectMath_;

	XMFLOAT2 set = XMFLOAT2(selectMath_.x, selectMath_.y);
	table_->SetMathType(set, mathtype_);
}

void MapEditScene::ButtonInit()
{
	//マス選択ボタンのファイルネーム指定
	std::string buttonName[MATH_MAX];
	for (int i = 0; i < MATH_MAX; i++)
	{
		buttonName[i] = "Button_" + fileNameInit_[i] + ".png";
	}
	//その他のボタン
	//完了ボタン
	std::string buttonComplete = "Button_Complete.png";
	//テストプレイボタン
	std::string buttonTestplay = "Button_TestPlay.png";
	//中止ボタン
	std::string buttonCancel = "Button_Cancel.png";

	//上のファイルが入ってるフォルダー
	std::string folderName = "Assets\\Button\\MathSelect\\";

	//マス選択ボタンの画像番号
	int mathButtonNum[MATH_MAX];
	//その他のボタンの画像番号
	int completeNum = -1;
	int testplayNum = -1;
	int cancelNum = -1;

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

	//中止ボタンのロード
	folderName = "Assets\\Button\\";
	buttonCancel = folderName + buttonCancel;
	cancelNum = Image::Load(buttonCancel);
	assert(cancelNum >= 0);

	//ボタンの普通のマスとの倍率
	const float normalMathtoMult = 2.0f;
	//ボタンのサイズ
	const float mathButtonSize = MATHSIZE * normalMathtoMult;
	//ボタンの大きさ設定
	const XMFLOAT3 mbScale = XMFLOAT3(1.0f / imageSize.x * mathButtonSize, 1.0f / imageSize.y * mathButtonSize, 1);
	const int buttonInitNum = 0;
	//ボタンのオブジェクトネーム
	std::string buttonStr;

	for (int i = buttonInitNum; i < MATH_MAX; i++)
	{
		pMathButton_[i] = ButtonManager::GetButton(ButtonManager::AddButton(("MathButton" + std::to_string(i)), this));
		pMathButton_[i]->SetPictNum(mathButtonNum[i]);

		
		//マス選択ボタンの基準の位置
		const XMFLOAT3 mbInitPos = XMFLOAT3(-0.9f, 0.9f, 0);
		//マス選択ボタンが何個ごとに改行されるか
		const int mbNewLineNum = 5;
		//マス選択ボタンの位置
		XMFLOAT3 mbPos;
		//ボタンの大きさ
		const XMFLOAT3 mbSize = XMFLOAT3((1.0f / Direct3D::bfr_scrWidth) * mathButtonSize * 2,
			(1.0f / Direct3D::bfr_scrHeight) * mathButtonSize * 2, 0);
		//マス選択ボタンの間隔
		const XMFLOAT3 mbIntervalBase = XMFLOAT3(0, 0, 0);
		const XMFLOAT3 mbInterval = XMFLOAT3(((i - buttonInitNum) % mbNewLineNum) * mbIntervalBase.x,
			-((i - buttonInitNum) / mbNewLineNum) * mbIntervalBase.y, 0);
		mbPos.x =  (float)((i - buttonInitNum) % mbNewLineNum) * mbSize.x + mbInitPos.x + mbInterval.x;
		mbPos.y = -(float)((i - buttonInitNum) / mbNewLineNum) * mbSize.y + mbInitPos.y + mbInterval.y;
		mbPos.z = mbInitPos.z;

		Transform mbTransform;
		mbTransform.position_ = mbPos;
		mbTransform.scale_ = mbScale;
		pMathButton_[i]->SetTransform(mbTransform);
	}

	//その他のボタンの大きさ
	const XMFLOAT3 obScale = XMFLOAT3(0.3f, 0.3f, 1);
	//完了ボタン
	pCompleteButton_ = ButtonManager::GetButton(ButtonManager::AddButton("compButton", (GameObject*)this));

	const XMFLOAT3 cbPos = XMFLOAT3(-0.8f, -0.7f, 0);
	Transform cbTransform;
	cbTransform.position_ = cbPos;
	cbTransform.scale_ = obScale;
	pCompleteButton_->SetTransform(cbTransform);
	pCompleteButton_->SetPictNum(completeNum);

	//テストプレイボタン
	//テストプレイボタンの番号
	pTestplayButton_ = ButtonManager::GetButton(ButtonManager::AddButton("testButton", (GameObject*)this));

	const XMFLOAT3 tbPos = XMFLOAT3(-0.8f, -0.6f, 0);
	Transform tbTransform;
	tbTransform.position_ = tbPos;
	tbTransform.scale_ = obScale;
	pTestplayButton_->SetTransform(tbTransform);
	pTestplayButton_->SetPictNum(testplayNum);

	//中止ボタン
	pCancelButton_ = ButtonManager::GetButton(ButtonManager::AddButton("cancelButton", (GameObject*)this));

	const XMFLOAT3 ccbPos = tbPos;
	Transform ccbTransform;
	ccbTransform.position_ = ccbPos;
	ccbTransform.scale_ = obScale;
	pCancelButton_->SetTransform(ccbTransform);
	pCancelButton_->SetPictNum(cancelNum);
}

void MapEditScene::ButtonUpdate()
{
	SelectMathType();
	OtherButtonPush();
}

void MapEditScene::OtherButtonPush()
{
	static PlayScene* pTestplay = nullptr;

	pTestplayButton_->SetIsCanPush(canTest_ * isDisp_);
	//テストプレイクリアできないと押せないようにする
	pCompleteButton_->SetIsCanPush(isClear_ * isDisp_);
	//テストプレイしてる時だけ押せるようにする
	pCancelButton_->SetIsCanPush(!isDisp_);

	//テストプレイボタンが押されたら
	if (pTestplayButton_->OnClick())
	{
		//マップエディター非表示
		isDisp_ = false;
		Write();
		pTestplay->Instantiate<PlayScene>(this);
		pTestplay = (PlayScene*)FindObject("PlayScene");
	}
	//完了ボタンが押されたら
	else if (pCompleteButton_->OnClick())
	{
		//音楽停止
		Audio::Stop(hAudio_Music_);
		//ファイルに書き込み
		Write();
		pGP_->MapEditFinished();
	}
	//中止ボタン
	//テストプレイしているとき
	else if (pTestplay != nullptr)
	{
		if (pCancelButton_->OnClick())
		{
			pTestplay->KillMe();
			pTestplay = nullptr;
			//マップエディター表示
			isDisp_ = true;
		}
	}
}

void MapEditScene::ExpantionInit()
{
	//マスの説明のファイルネーム
	std::string expantionName[MATH_MAX];
	for(int i = 0; i < MATH_MAX; i++)
	{
		expantionName[i] = "Expantion_" + fileNameInit_[i] + ".png";
	};
	//マスの説明の画像が入ってるフォルダ名
	std::string folderName = "Assets\\MathExpantion\\";
	//マスの説明画像ロード
	for (int i = 0; i < MATH_MAX; i++)
	{
		if (i != (int)MATH_DELETE)
		{
			expantionName[i] = folderName + expantionName[i];

			hExpantion_[i] = Image::Load(expantionName[i]);
			assert(hExpantion_[i] >= 0);
		}
	}
}

void MapEditScene::ExpantionDraw()
{
	//画像の大きさ
	XMFLOAT3 exTexSize = Image::GetTextureSize(hExpantion_[mathtype_]);
	//画像の上の位置
	const XMFLOAT3 exPos = XMFLOAT3(-0.7f, 0.5f, 0);
	//画像の大きさ
	const XMFLOAT3 exScale = XMFLOAT3(0.7f, 0.7f, 1);
	tExpantion_.position_ = XMFLOAT3(exPos.x, 
		exPos.y - ((exTexSize.y) / Direct3D::bfr_scrHeight * exScale.y), 
		exPos.z);
	tExpantion_.scale_ = exScale;
	//そのマスの種類のコスト表示
	// 画面の中央から左上までの距離
	const XMFLOAT2 upperLeftDir = XMFLOAT2(1.0f, 1.0f);
	//画面全体の大きさ
	const XMFLOAT2 scrSize = XMFLOAT2(2.0f, 2.0f);
	//位置
	XMFLOAT2 mathCostPos = XMFLOAT2((exPos.x + upperLeftDir.x) * Direct3D::bfr_scrWidth / scrSize.x,
		abs(exPos.y - upperLeftDir.y) * Direct3D::bfr_scrHeight / scrSize.y + (exTexSize.y * exScale.y) + pText_->GetHeight());
	//表示する文字
	std::string str = "cost : ";
	str += std::to_string(costs_[mathtype_]);

	//コスト表示
	pText_->Draw(mathCostPos.x, mathCostPos.y, str.c_str());

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
	//マスを置く基準の位置（中央のマスの位置)
	const XMFLOAT3 mathInitPos = XMFLOAT3(0.25f, 0, 0);

	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.z; y++)
		{
			math_origin_[x][y] = math_[x][y];
			XMFLOAT2 pos = XMFLOAT2(x, y);

			Transform tMath;

			tMath.scale_ = XMFLOAT3(1.0f / imageSize.x * MATHSIZE, 1.0f / imageSize.y * MATHSIZE, 1);
			//1マスの大きさ
			const XMFLOAT2 mathSize = XMFLOAT2(((float)x / Direct3D::bfr_scrWidth) * MATHSIZE * 2,
				((float)y / Direct3D::bfr_scrHeight) * MATHSIZE * 2);
			//マスを置く位置
			const XMFLOAT2 mathPos = XMFLOAT2(mathInitPos.x - ((float)mathVolume_.x / Direct3D::bfr_scrWidth) * MATHSIZE,
				mathInitPos.y - ((float)mathVolume_.z / Direct3D::bfr_scrHeight) * MATHSIZE);

			tMath.position_.x = mathSize.x + mathPos.x;
			tMath.position_.y = mathSize.y + mathPos.y;

			table_->SetMathTransform(pos, tMath);
		}
	}
}

void MapEditScene::MathDraw()
{
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.z; y++)
		{
			XMFLOAT2 drawPos = XMFLOAT2(x, mathVolume_.z - 1 - y);
			//コンベアの回転
			if (isConvRot_[drawPos.x][drawPos.y])
			{
				//コンベアが回る音
				Audio::Play(hSE_ConvRot_, 1);

				//1フレームに回転する量
				const float rotValUpdate = 5;
				Transform tConv = table_->GetMathTransform(drawPos);
				tConv.rotate_.z += rotValUpdate;
				
				//1回転の量
				const float rotVal = 90;

				if ((int)tConv.rotate_.z % (int)rotVal == 0)
				{
					tConv.rotate_.z = (tConv.rotate_.z / rotVal) * rotVal;
					isConvRot_[x][mathVolume_.z - 1 - y] = false;
					Audio::Stop(hSE_ConvRot_);
				}
				table_->SetMathTransform(drawPos, tConv);
			}
			Image::SetTransform(hPict_[table_->GetMathType(XMFLOAT2(x, y))], table_->GetMathTransform(XMFLOAT2(x, y)));
			Image::Draw(hPict_[table_->GetMathType(XMFLOAT2(x, y))]);

		}
	}
}

void MapEditScene::SelectMathSet()
{
	selectMath_.x = (int)(mousePos_.x / MATHSIZE);
	selectMath_.y = mathVolume_.z - 1 - (int)(mousePos_.y / MATHSIZE);

	//マウスの位置がマス目から出たら
	if (selectMath_.x < 0 || selectMath_.x >= mathVolume_.x ||
		selectMath_.y < 0 || selectMath_.y >= mathVolume_.z)
	{
		selectMath_ = mathInitPos;
	}
}

void MapEditScene::SelectMathType()
{
	for (int i = 0; i < MATH_MAX; i++)
	{
		if (pMathButton_[i]->OnClick())
		{
			mathtype_ = i;
		}
	}
	//選ばれてるボタン
	for (int i = 0; i < MATH_MAX; i++)
	{
		pMathButton_[i]->SetIsSelect(false);
		if (mathtype_ == i)
		{
			pMathButton_[i]->SetIsSelect(true);
		}
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
	//中止ボタン表示日表示切替
	pCancelButton_->SetIsDisplay(!_isDisp);
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
			//tgtgRouteMathUpがマスの範囲外だった時
			if (tgtgRouteMathUp_.x < 0)
			{
				tgtgRouteMathUp_.x = 0;
			}
			else if (tgtgRouteMathUp_.x >= mathVolume_.x)
			{
				tgtgRouteMathUp_.x = mathVolume_.x - 1;
			}
			if (tgtgRouteMathUp_.y < 0)
			{
				tgtgRouteMathUp_.y = 0;
			}
			else if (tgtgRouteMathUp_.y >= mathVolume_.x)
			{
				tgtgRouteMathUp_.y = mathVolume_.z - 1;
			}

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
		if (table_->GetMathType(XMFLOAT2(itr->initPos_.x, itr->initPos_.y)) != (int)MATH_TOGETOGE)
		{
			tTgtgRoute_.erase(itr);
			break;
		}
		itr++;
	}
}

void MapEditScene::AudioInit()
{
	//音楽が入ってるフォルダ名
	const std::string folderName = "Assets\\Audio\\";
	//音楽
	const std::string musicFolder = "Music\\";
	//音楽ロード
	std::string music = folderName + musicFolder + "Audio_MapEdit.wav";
	hAudio_Music_ = Audio::Load(music, true);

	//SE
	const std::string SEFolder = "SE\\";
	//SEロード
	std::string se = folderName + SEFolder + "SE_PutMath.wav";
	hSE_PutMath_ = Audio::Load(se, false);
	//コンベアを回転させたときのSE
	std::string convSe = folderName + SEFolder + "SE_Rotate.wav";
	hSE_ConvRot_ = Audio::Load(convSe, false);
}
