#pragma once
#include "Engine/GameObject.h"
#include "Engine/Direct3D.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <vector>
class SceneTransition;

enum MATHTYPE
{
	MATH_FLOOR,		//床
	MATH_WALL,		//壁
	MATH_HOLE,		//穴
	MATH_CONVEYOR,	//コンベア
	MATH_TOGETOGE,	//とげとげのやつ
	MATH_PITFALL,	//落とし穴
	MATH_TIMELIMITDEC, //制限時間減る
	MATH_START,		//スタートマス
	MATH_GOAL,		//ゴールマス
	MATH_MAX,
};

struct MATHDEDAIL
{
	MATHTYPE mathType_; //マスの種類
	Transform mathPos_; //マスの位置
};

struct TOGETOGEROUTE
{
	Transform route_;	//移動範囲を示す線のトランスフォーム
	XMFLOAT3 initPos_;	//とげとげの最初の位置
	XMFLOAT3 destPos_;	//とげとげの往復する到達地点
};

class StageOrigin
{
protected:
	//マスの大きさ
	const int MATHSIZE = 30;

	//Floorのモデルの高さ
	const int MODELHEIGHT = 1;

	//すべてのマスの情報
	std::vector<std::vector<MATHDEDAIL>> math_;
	//とげとげが移動する範囲
	std::vector<TOGETOGEROUTE> tTgtgRoute_;
	//ファイルネームの基本
	std::vector<std::string> fileNameInit_;

	SceneTransition* pST;

	//マスの量
	int XSIZE;
	int ZSIZE;

	//ゲームをプレイ中に保存するファイルの入ってるフォルダ名
	std::string saveFolderName = "SaveFile\\";
public:
	StageOrigin();
	//マスのサイズ変更
	void Math_Resize(int _xsize, int _zsize, std::vector<std::vector<MATHDEDAIL>> *_math)
	{
		_math->resize(_xsize);
		for (int x = 0; x < _xsize; x++)
		{
			_math->at(x).resize(_zsize);
		}
	}
	void Math_Resize(int _xsize, int _zsize, std::vector<std::vector<int>>* _math)
	{
		_math->resize(_xsize);
		for (int x = 0; x < _xsize; x++)
		{
			_math->at(x).resize(_zsize);
		}
	}
	void Math_Resize(int _xsize, int _zsize, std::vector<std::vector<bool>>* _math)
	{
		_math->resize(_xsize);
		for (int x = 0; x < _xsize; x++)
		{
			_math->at(x).resize(_zsize);
			for (int z = 0; z < _zsize; z++)
			{
				_math->at(x).at(z) = false;
			}
		}
	}
	//自分のクラスにあるmath_のvectorへの代入
	void SetTableMath(std::vector<std::vector<MATHDEDAIL>> _math)
	{
		for (int x = 0; x < XSIZE; x++)
		{
			for (int z = 0; z < ZSIZE; z++)
			{
				math_.at(x).at(z) = _math.at(x).at(z);
			}
		}
	}

	void SetTogetogeRoute(std::vector<TOGETOGEROUTE> _tgtgRoute)
	{
		for (int i = 0; i < _tgtgRoute.size(); i++)
		{
			TOGETOGEROUTE* pTg = new TOGETOGEROUTE();
			pTg->route_ = _tgtgRoute[i].route_;
			pTg->initPos_ = _tgtgRoute[i].initPos_;
			pTg->destPos_ = _tgtgRoute[i].destPos_;
			tTgtgRoute_.push_back(*pTg);
			delete pTg;
		}
	}
};

