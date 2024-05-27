#pragma once
#include "GameObject.h"

enum FADETYPE
{
    TYPE_FADEIN = 0,    //�t�F�[�h�C��
    TYPE_FADEOUT,       //�t�F�[�h�A�E�g
};

class Fade
{
    //�摜�ԍ�
    int hFade_;
    //�摜�̃g�����X�t�H�[��
    Transform tFade_;

    //�t�F�[�h�̃^�C�v
    FADETYPE fadeType_;

    //�l�𑝂₹�ΈÂ��Ȃ��Ă���
    float alpha_;

    //�Â��Ȃ�n�߂Đ^���ÂɂȂ鎞��
    float fadeSpeed_;

    //�t�F�[�h���I�������
    bool isFinished_;
public:
    Fade(FADETYPE _fadeType);
    ~Fade();

    void Initialize();
    void Update();
    void Draw();
    void Release();

    float GetAlpha() { return alpha_; }
    bool GetIsFinished() { return isFinished_; }

    //�t�F�[�h�̃^�C�v�̃Z�b�^�[
    void SetFadeType(FADETYPE _fadeType) { fadeType_ = _fadeType; }
};

