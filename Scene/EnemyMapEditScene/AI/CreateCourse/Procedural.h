#pragma once
#include <vector>

#include "../../../Engine/Direct3D.h"

class Seed
{
	//体積
	XMFLOAT3 volume_;
	//濃淡
	int shade_;
	//自分の位置
	XMFLOAT2 position_;

public:
	Seed(XMFLOAT2 _pos);

	//ゲッターセッター
	XMFLOAT3 GetVolume() { return volume_; }
	void SetVolume(XMFLOAT3 _volume) { volume_ = _volume; }
	int GetShade() { return shade_; }
	void SetShade(int _shade) { shade_ = _shade; }
	XMFLOAT2 GetPos() { return position_; }
	void SetPos(XMFLOAT2 _pos) { position_ = _pos; }

};

class Procedural
{
	std::vector<std::vector<int>> mathShades_;	//それぞれのマスの濃淡の値
	std::vector<Seed*> seedList_;	//種情報
public:
	//引数1：マスのタイプが入ってる先頭のアドレス
	//引数2,3：マスのサイズ
	//引数4：マスごとの濃淡情報が入った配列の先頭のアドレス
	//引数5：引数4の配列のサイズ
	Procedural(std::vector<std::vector<int>> _mathShades, int _xsize, int _ysize);

	//種の配置実行
	//引数：ばらまく種の数
	void Excute(int _seedNum);

	//種ばらまく
	//引数：ばらまく種の数
	void SeedScatter(int _seedNum);
	//同じ位置に種がすでにあるかどうか
	bool IsSeedPresence(int _x, int _y);

	//Seed同士で体積がぶつかってるかチェック
	//ぶつかってたらどっちか消す
	void VolumeCheck();

	//seedList_の中身を濃淡が多い順にソートする
	void SeedListSort();

	//seedList_返す
	std::vector<Seed*> GetSeedList() { return seedList_; }
	//seedList_のサイズ返す
	int GetSeedListSize() { return seedList_.size(); }
};

