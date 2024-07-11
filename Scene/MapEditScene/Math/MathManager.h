#pragma once
#include <vector>
#include "../../../StageOrigin.h"

class Math;

class MathManager : public StageOrigin
{
	std::vector<std::vector<Math*>> maths_;
	std::vector<int> costs_;	//それぞれのマスの種類のコスト
public:
	//引数1,2：ステージのサイズ
	//引数3：コストが入ってる配列の先頭のアドレス
	//引数4：コストが入ってる配列のサイズ
	MathManager(int _xsize, int _ysize, std::vector<int> _costs, int _num);

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
};

