#pragma once
#include "GameObject.h"

const float fadeTimeBase = 0.3f;

enum FADETYPE
{
    TYPE_FADEIN = 0,    //フェードイン
    TYPE_FADEOUT,       //フェードアウト
};

class Fade : public GameObject
{
    //画像番号
    int hFade_;
    //画像のトランスフォーム
    Transform tFade_;

    //フェードのタイプ
    FADETYPE fadeType_;

    //値を増やせば暗くなっていく
    float alpha_;

    //暗くなり始めて真っ暗になる時間
    float fadeSpeed_;
public:
    Fade(FADETYPE _fadeType, float _fadeSpeed);
    ~Fade();

    void Initialize();
    void Update();
    void Draw();
    void Release();

    //フェードが終わったかどうか
    bool IsFaded();

    float GetAlpha() { return alpha_; }

    //フェードのタイプのセッター・ゲッター
    FADETYPE GetFadeType() { return fadeType_; }
    void SetFadeType(FADETYPE _fadeType);
};

namespace FADE
{
    //フェード開始
    void FadeStart(GameObject* _parent, float _fadeSpeed, FADETYPE _fadeType);
    //フェードインが終わったかどうか
    bool IsFadeinFinished();
    //フェードアウトが終わったかどうか
    bool IsFadeoutFinished();
    //フェードを消す
    void FadeRemove();
}

