#pragma once
#include "GameObject.h"

enum FADETYPE
{
    TYPE_FADEIN = 0,    //�t�F�[�h�C��
    TYPE_FADEOUT,       //�t�F�[�h�A�E�g
};

class Fade : public GameObject
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
public:
    Fade(FADETYPE _fadeType, float _fadeSpeed);
    ~Fade();

    void Initialize();
    void Update();
    void Draw();
    void Release();

    //�t�F�[�h���I��������ǂ���
    bool IsFaded();

    float GetAlpha() { return alpha_; }

    //�t�F�[�h�̃^�C�v�̃Z�b�^�[
    void SetFadeType(FADETYPE _fadeType) { fadeType_ = _fadeType; }
};

namespace FADE
{
    int FadeStart(GameObject* _parent, float _fadeSpeed, FADETYPE _fadeType);
    bool FadeEnd(int _handle);

    //�t�F�[�h��T�����ăn���h����Ԃ�
    int FadeSerch(Fade* _fade);
}

