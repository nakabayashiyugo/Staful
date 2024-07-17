#pragma once
#include <vector>
#include "../../../StageOrigin.h"

class Math;

class MathManager
{
	std::vector<std::vector<Math*>> maths_;
	std::vector<int> costs_;	//それぞれのマスの種類のコスト
	int costLimit_;				//配置可能なコスト数
public:
	//引数1,2：ステージのサイズ
	//引数3：マスの初期値
	//引数4：コストが入ってる配列の先頭のアドレス
	//引数5：コストが入ってる配列のサイズ
	//引数6：配置可能なコスト数
	MathManager(int _xsize, int _ysize, int _initType, std::vector<int> _costs, int _num, int _costLimit);
	~MathManager();

	bool IsCostOver(XMFLOAT2 _pos, int _type);

public:
	//引数１：セットする要素数
	//引数２：セットするマスのタイプ
	void SetMathType(XMFLOAT2 _pos, int _type);
	//引数１：ゲットする要素数
	int GetMathType(XMFLOAT2 _pos);
	//引数1：ゲットする要素数
	Transform GetMathTransform(XMFLOAT2 _pos);
	//引数1：セットする要素数
	//引数2：セットするトランスフォーム
	void SetMathTransform(XMFLOAT2 _pos, Transform _transform);
	//引数１：ゲットするマスのタイプ
	int GetCost(int _type);
	//現在のコースにかかってるコスト取得
	int GetStageCost();
	//配置可能なコスト取得
	int GetCostLimit() { return costLimit_; }
};

