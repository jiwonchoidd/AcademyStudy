#pragma once
#include "K2DAsset.h"
#include "KSoundManager.h"
struct KUIData
{
    KTexture* pTex;
    KSound* pSound;

    KUIData() {}
    KUIData(KTexture* a, KSound* b)
    {
        pTex = a;
        pSound = b;
    }
};

class KUI : public K2DAsset
{
private:
    ID3D11DeviceContext* m_pContext;
public:
    std::vector<KUIData> m_datalist;
public:
    RECT				m_rtOffset;
    RECT				m_rtOffsetTex;
public:
    virtual bool Init(ID3D11DeviceContext* context, std::wstring vs,
        std::wstring ps, std::wstring tex = nullptr, std::wstring mask = nullptr);
    virtual bool Frame()override;
    virtual bool Render(ID3D11DeviceContext* context)override;

    // 전체크기 이미지 깨짐 방지 위해 UI는 9등분으로 나눔,
    bool SetVertexData()override;
    bool SetIndexData()override;
public:
    KUI();
    ~KUI();
};

class KImage : public KUI
{
public:  
    virtual bool    Frame()override;
    virtual void    FadeIn();
    virtual void    FadeOut();
};

class KButton : public KUI
{
public:
    bool m_btn_active = false;
public:
    virtual void	SelectOverlap(KCollider* pObj, DWORD dwState) override;
    virtual bool    Frame()override;
};