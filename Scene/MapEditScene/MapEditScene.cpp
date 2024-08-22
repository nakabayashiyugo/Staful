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
	//マスの選択について
	mathtype_(0),
	//テストプレイについて
	isClear_(false),
	canTest_(false),
	isDisp_(true),
	testPS_(nullptr),
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

	Math_Resize(mathVolume_.x, mathVolume_.y, &math_);
	Math_Resize(mathVolume_.x, mathVolume_.y, &math_origin_);
	Math_Resize(mathVolume_.x, mathVolume_.y, &isConvRot_);

	pGP_ = (GamePlayer*)this->pParent_;
	saveNum_ = pGP_->GetSaveNum();

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

	costLimitFirst = mathVolume_.x + mathVolume_.y;
	costLimitPlus = costLimitFirst / 2;
	turnNum_ = pGP_->GetTurnNum();
	int costLimit = costLimitFirst + (turnNum_ - 1) * costLimitPlus;

	table_ = new MathManager(mathVolume_.x, mathVolume_.y, MATH_DELETE, costs_, costs_.size(), costLimit);

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
	//音楽の初期化
	AudioInit();

	hTgtgRoute_ = Image::Load("Assets\\Togetoge_Route.png");
	assert(hTgtgRoute_ >= 0);

	pText_ = new Text();
	pText_->Initialize();
	Audio::Play(hAudio_Music_, soundVolume);
}

void MapEditScene::Update()
{
	

	//SelectMathSet();
	ButtonUpdate();

	//左クリッククリック
	if (Input::IsMouseButton(0))
	{
		LeftClicked();
	}
	//右クリック
	if (Input::IsMouseButtonDown(1))
	{
		RightClicked();
	}
	//右クリック離した時
	if (Input::IsMuoseButtonUp(1))
	{
		RightReleased();
	}
}

void MapEditScene::Draw()
{
	IsDisplay(isDisp_);
	if(isDisp_)
	{
		//マス表示
		MathDraw();
		//マスの説明表示
		ExpantionDraw();
		//コスト表示
		CostDraw();
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
		//プレイヤー番号表示
		pGP_->PlayerNumDraw();
	}
}

void MapEditScene::Release()
{
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
	std::string str = std::to_string(table_->GetStageCost()) + " / " + std::to_string(table_->GetCostLimit());
	pText_->Draw(curCostPos.x, curCostPos.y, str.c_str());
	//文字「costLimit」表示
	pText_->Draw(costLimitPos.x, costLimitPos.y, strCostLimit.c_str());
}

void MapEditScene::ChangeSelectMath()
{
	XMFLOAT2 set = XMFLOAT2(selectMath_.x, selectMath_.y);
	table_->SetMathType(set, mathtype_);

	//クリックしたマスを選んでるマスに変える
	if (table_->GetMathType(set) != MATH_CONVEYOR)
	{
		Transform tMath = table_->GetMathTransform(set);
		tMath.rotate_ = XMFLOAT3(0, 0, 0);
		table_->SetMathTransform(set, tMath);
	}


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

	//ボタンの普通のマスとの倍率
	const float normalMathtoMult = 2.0f;
	//ボタンのサイズ
	const float mathButtonSize = EDITERMATHSIZE * normalMathtoMult;
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
}

void MapEditScene::ButtonUpdate()
{
	SelectMathType();
	OtherButtonPush();
}

void MapEditScene::OtherButtonPush()
{
	pTestplayButton_->SetIsCanPush(canTest_ * isDisp_);
	//テストプレイクリアできないと押せないようにする
	pCompleteButton_->SetIsCanPush(isClear_ * isDisp_);

	//テストプレイボタンが押されたら
	if (pTestplayButton_->OnClick())
	{
		TestButtonPush();
	}
	//完了ボタンが押されたら
	else if (pCompleteButton_->OnClick())
	{
		CompButtonPush();
	}
}

void MapEditScene::CompButtonPush()
{
	//音楽停止
	Audio::Stop(hAudio_Music_);
	//ファイルに書き込み
	Write();
	pGP_->MapEditFinished();
}

void MapEditScene::TestButtonPush()
{
	//マップエディター非表示
	isDisp_ = false;
	Write();
	testPS_->Instantiate<PlayScene>(this);
	testPS_ = (PlayScene*)FindObject("PlayScene");
}

void MapEditScene::CancelButtonPush()
{
	testPS_->KillMe();
	testPS_ = nullptr;
	//マップエディター表示
	isDisp_ = true;
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
		for (int y = 0; y < mathVolume_.y; y++)
		{
			math_origin_[x][y] = math_[x][y];
			XMFLOAT2 pos = XMFLOAT2(x, y);

			Transform tMath = math_[x][y].mathPos_;

			tMath.scale_ = XMFLOAT3(1.0f / imageSize.x * EDITERMATHSIZE, 1.0f / imageSize.y * EDITERMATHSIZE, 1);
			//1マスの大きさ
			const XMFLOAT2 mathSize = XMFLOAT2(((float)x / Direct3D::bfr_scrWidth) * EDITERMATHSIZE * 2,
				((float)y / Direct3D::bfr_scrHeight) * EDITERMATHSIZE * 2);
			//マスを置く位置
			const XMFLOAT2 mathPos = XMFLOAT2(mathInitPos.x - ((float)mathVolume_.x / Direct3D::bfr_scrWidth) * EDITERMATHSIZE,
				mathInitPos.y - ((float)mathVolume_.y / Direct3D::bfr_scrHeight) * EDITERMATHSIZE);

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
		for (int y = 0; y < mathVolume_.y; y++)
		{
			XMFLOAT2 drawPos = XMFLOAT2(x, mathVolume_.y - 1 - y);
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
					isConvRot_[x][mathVolume_.y - 1 - y] = false;
					Audio::Stop(hSE_ConvRot_);
				}
				table_->SetMathTransform(drawPos, tConv);
			}
			Image::SetTransform(hPict_[table_->GetMathType(XMFLOAT2(x, y))], table_->GetMathTransform(XMFLOAT2(x, y)));
			Image::Draw(hPict_[table_->GetMathType(XMFLOAT2(x, y))]);

		}
	}
}

void MapEditScene::LeftClicked()
{
	leftClickPos_ = table_->GetCursorCellIndex();
	selectMath_ = XMFLOAT3(leftClickPos_.x, leftClickPos_.y, 0);

	//押したところがマスの範囲外だったら
	if (leftClickPos_.x != -1)
	{
		if (math_origin_[(int)leftClickPos_.x][(int)leftClickPos_.y].mathType_ == MATH_DELETE)
		{
			switch ((MATHTYPE)mathtype_)
			{
			case MATH_START:
				if (turnNum_ == 1)
				{
					//スタートマスがすでにあるかどうか探索
					for (int x = 0; x < mathVolume_.x; x++)
					{
						for (int y = 0; y < mathVolume_.y; y++)
						{
							if (table_->GetMathType(XMFLOAT2(x, y)) == MATH_START)
							{
								table_->SetMathType(XMFLOAT2(x, y), MATH_DELETE);
							}
						}
					}
					ChangeSelectMath();
				}
				break;
			case MATH_GOAL:
				if (turnNum_ == 1)
				{
					//ゴールマスがすでにあるかどうか探索
					for (int x = 0; x < mathVolume_.x; x++)
					{
						for (int y = 0; y < mathVolume_.y; y++)
						{
							if (table_->GetMathType(XMFLOAT2(x, y)) == MATH_GOAL)
							{
								table_->SetMathType(XMFLOAT2(x, y), MATH_DELETE);
							}
						}
					}
					ChangeSelectMath();
				}
				break;
			case MATH_TOGETOGE:
				XMFLOAT3 mousePos = XMFLOAT3((int)leftClickPos_.x, (int)leftClickPos_.y, 0);
				ChangeSelectMath();

				TgtgPushBack(mousePos);
				break;
			default:
				ChangeSelectMath();
				break;
			}
		}
	}

	//とげとげマスがほかのマスに変わった時の要素の削除
	TogetogeElemDelete();
}

void MapEditScene::RightClicked()
{
	rightClickPos_ = table_->GetCursorCellIndex();
	

	//コンベアマス回転
	//押したところがマスの範囲外だったら
	if (rightClickPos_.x != -1)
	{
		if ((MATHTYPE)mathtype_ == MATH_CONVEYOR)
		{
			if (table_->GetMathType(rightClickPos_) == MATHTYPE::MATH_CONVEYOR)
			{
				isConvRot_[(int)rightClickPos_.x][(int)rightClickPos_.y] = true;
				isClear_ = false;
			}
		}
		if ((MATHTYPE)mathtype_ == MATH_TOGETOGE)
		{
			if (table_->GetMathType(rightClickPos_) == MATHTYPE::MATH_TOGETOGE)
			{
				tgtgRouteMathDown_ = XMFLOAT3(rightClickPos_.x, rightClickPos_.y, 0);
			}
		}
	}
}

void MapEditScene::RightReleased()
{
	rightReleasePos_ = table_->GetCursorCellIndex();
	tgtgRouteMathUp_ = XMFLOAT3(rightReleasePos_.x, rightReleasePos_.y, 0);
	
	//押したところがマスの範囲外じゃなかったら
	if (rightReleasePos_.x != -1)
	{
		if ((MATHTYPE)mathtype_ == MATH_TOGETOGE)
		{
			//とげとげルート設定
			TogetogeRouteSet();
		}
	}

}

void MapEditScene::TgtgPushBack(XMFLOAT3 _mousePos)
{
	//tTgtgRouteの中に位置が同じなとげとげが存在してるか
	auto itr = tTgtgRoute_.begin();
	while (itr != tTgtgRoute_.end())
	{
		if (itr->initPos_.x == _mousePos.x &&
			itr->initPos_.y == _mousePos.y)
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
		ptg->initPos_ = ptg->destPos_ = _mousePos;
		ptg->route_.scale_ = XMFLOAT3(0, 0, 0);
		tTgtgRoute_.push_back(*ptg);
		delete ptg;
	}
}

void MapEditScene::SelectMathSet()
{
	selectMath_ = XMFLOAT3(table_->GetCursorCellIndex().x,
							table_->GetCursorCellIndex().y, 0);
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
}

void MapEditScene::CheckCanTest()
{
	//スタートマスとゴールマスをおいてないとテストプレイをできないようにする
	bool isStart = false, isGoal = false;
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int y = 0; y < mathVolume_.y; y++)
		{
			if (table_->GetMathType(XMFLOAT2(x, y)) == MATH_START)
			{
				isStart = true;
			}
			if (table_->GetMathType(XMFLOAT2(x, y)) == MATH_GOAL)
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
	if (tgtgRouteMathDown_.x != -1)
	{
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
		else if (tgtgRouteMathUp_.y >= mathVolume_.y)
		{
			tgtgRouteMathUp_.y = mathVolume_.y - 1;
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
		//1マスの大きさ
		XMFLOAT3 mathSize = table_->GetMathTransform(XMFLOAT2(0, 0)).scale_;
		//縦移動
		if (abs(tgtgRouteMathUp_.x - tgtgRouteMathDown_.x) < abs(tgtgRouteMathUp_.y - tgtgRouteMathDown_.y))
		{
			//y方向の大きさ
			float ysize = abs(tgtgRouteMathUp_.y - tgtgRouteMathDown_.y);

			itr->route_.scale_.x = mathSize.x / tgtgRouteThick;
			itr->route_.scale_.y = mathSize.y * ysize;

			//ルートを表示する位置
			//y方向の表示する位置(マウスを押した位置と離した位置の中間の位置)
			float ypos = ((int)tgtgRouteMathUp_.y + tgtgRouteMathDown_.y) / 2;
			XMFLOAT3 routePos = table_->GetMathTransform(
				XMFLOAT2((int)tgtgRouteMathDown_.x, ypos)).position_;
			itr->route_.position_ = routePos;

			if (((int)tgtgRouteMathUp_.y + (int)tgtgRouteMathDown_.y) % 2 != 0)
			{
				itr->route_.position_.y += (mathSize.y) / 2;
			}
			itr->destPos_.y = tgtgRouteMathUp_.y;
			itr->destPos_.x = tgtgRouteMathDown_.x;
		}
		//横移動
		else
		{
			//x方向の大きさ
			float xsize = abs(tgtgRouteMathUp_.x - tgtgRouteMathDown_.x);

			itr->route_.scale_.x = mathSize.x * xsize;
			itr->route_.scale_.y = mathSize.y / tgtgRouteThick;

			//ルートを表示する位置
			//x方向の表示する位置(マウスを押した位置と離した位置の中間の位置)
			float xpos = ((int)tgtgRouteMathUp_.x + tgtgRouteMathDown_.x) / 2;
			XMFLOAT3 routePos = table_->GetMathTransform(
				XMFLOAT2(xpos, (int)tgtgRouteMathDown_.y)).position_;
			itr->route_.position_ = routePos;

			if (((int)tgtgRouteMathUp_.x + (int)tgtgRouteMathDown_.x) % 2 != 0)
			{
				itr->route_.position_.x += (mathSize.x) / 2;
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
	tgtgRouteMathUp_ = mathInitPos;
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

void MapEditScene::Write()
{
	//math_に値入れる
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int z = 0; z < mathVolume_.y; z++)
		{
			math_[x][z].mathType_ = (MATHTYPE)table_->GetMathType(XMFLOAT2(x, z));
			math_[x][z].mathPos_ = table_->GetMathTransform(XMFLOAT2(x, z));
		}
	}
	StageOrigin::Write();
}

void MapEditScene::Read()
{
	StageOrigin::Read();
	//math_から値受け取る
	for (int x = 0; x < mathVolume_.x; x++)
	{
		for (int z = 0; z < mathVolume_.y; z++)
		{
			table_->SetMathType(XMFLOAT2(x, z), math_[x][z].mathType_);
			table_->SetMathTransform(XMFLOAT2(x, z), math_[x][z].mathPos_);
		}
	}
}
