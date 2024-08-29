#pragma once
#include "GameObject.h"

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

    //フェードのタイプのセッター
    void SetFadeType(FADETYPE _fadeType) { fadeType_ = _fadeType; }
};

namespace FADE
{
    int FadeStart(GameObject* _parent, float _fadeSpeed, FADETYPE _fadeType);
    bool FadeEnd(int _handle);

    //フェードを探索してハンドルを返す
    int FadeSerch(Fade* _fade);
}

