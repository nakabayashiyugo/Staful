#pragma once
#include "GameObject.h"

const float fadeTimeBase = 0.3f;

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

    //�t�F�[�h�̃^�C�v�̃Z�b�^�[�E�Q�b�^�[
    FADETYPE GetFadeType() { return fadeType_; }
    void SetFadeType(FADETYPE _fadeType);
};

namespace FADE
{
    //�t�F�[�h�J�n
    void FadeStart(GameObject* _parent, float _fadeSpeed, FADETYPE _fadeType);
    //�t�F�[�h�C�����I��������ǂ���
    bool IsFadeinFinished();
    //�t�F�[�h�A�E�g���I��������ǂ���
    bool IsFadeoutFinished();
    //�t�F�[�h������
    void FadeRemove();
}

