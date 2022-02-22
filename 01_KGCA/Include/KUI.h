#pragma once
#include "K2DAsset.h"
class KUI :
    public K2DAsset
{
private:
    ID3D11DeviceContext* m_pContext;
public:
    float  m_fAlpha = 0.0f;
    bool   m_bFadeIn = false;
    bool   m_bFadeOut = false;
public:
    bool Init(ID3D11DeviceContext* context, std::wstring vs,
        std::wstring ps, std::wstring tex = nullptr, std::wstring mask = nullptr);
    bool Frame()override;
    bool Render(ID3D11DeviceContext* pContext)override;
    void SelectOverlap(KCollider* pObj, DWORD dwState);

    void    FadeIn();
    void    FadeOut();
public:
    KUI();
    ~KUI();
};


