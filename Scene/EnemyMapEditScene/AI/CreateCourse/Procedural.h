#pragma once
#include <vector>

#include "../../../Engine/Direct3D.h"

class Seed
{
	//�̐�
	XMFLOAT3 volume_;
	//�Z�W
	int shade_;
	//�����̈ʒu
	XMFLOAT2 position_;

public:
	Seed(XMFLOAT2 _pos);

	//�Q�b�^�[�Z�b�^�[
	XMFLOAT3 GetVolume() { return volume_; }
	void SetVolume(XMFLOAT3 _volume) { volume_ = _volume; }
	int GetShade() { return shade_; }
	void SetShade(int _shade) { shade_ = _shade; }
	XMFLOAT2 GetPos() { return position_; }
	void SetPos(XMFLOAT2 _pos) { position_ = _pos; }

};

class Procedural
{
	std::vector<std::vector<int>> mathShades_;	//���ꂼ��̃}�X�̔Z�W�̒l
	std::vector<Seed*> seedList_;	//����
public:
	//����1�F�}�X�̃^�C�v�������Ă�擪�̃A�h���X
	//����2,3�F�}�X�̃T�C�Y
	//����4�F�}�X���Ƃ̔Z�W��񂪓������z��̐擪�̃A�h���X
	//����5�F����4�̔z��̃T�C�Y
	Procedural(std::vector<std::vector<int>> _mathShades, int _xsize, int _ysize);

	//��̔z�u���s
	//�����F�΂�܂���̐�
	void Excute(int _seedNum);

	//��΂�܂�
	//�����F�΂�܂���̐�
	void SeedScatter(int _seedNum);
	//�����ʒu�Ɏ킪���łɂ��邩�ǂ���
	bool IsSeedPresence(int _x, int _y);

	//Seed���m�ő̐ς��Ԃ����Ă邩�`�F�b�N
	//�Ԃ����Ă���ǂ���������
	void VolumeCheck();

	//seedList_�̒��g��Z�W���������Ƀ\�[�g����
	void SeedListSort();

	//seedList_�Ԃ�
	std::vector<Seed*> GetSeedList() { return seedList_; }
	//seedList_�̃T�C�Y�Ԃ�
	int GetSeedListSize() { return seedList_.size(); }
};

