#pragma once
#include "GameObject.h"

enum FADETYPE
{
    TYPE_FADEIN = 0,    //フェードイン
    TYPE_FADEOUT,       //フェードアウト
};

class Fade :
    public GameObject
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

    //フェードが終わったか
    bool isFinished_;
public:
    Fade(GameObject* parent);
    ~Fade();

    void Initialize() override;
    void Update() override;
    void Draw() override;
    void Release() override;

    float GetAlpha() { return alpha_; }
    bool GetIsFinished() { return isFinished_; }
};

