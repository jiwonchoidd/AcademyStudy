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
    float                m_image_ratio;
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
    float  m_fAlpha = 1.0f;
    bool   m_bFadeIn = false;
    bool   m_bFadeOut = false;
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

class KPlayerMenu : KImage
{
private:
    ID3D11DeviceContext* m_pContext;
public:
    KImage               img_background;
    std::vector<KButton> btn_list;
    std::vector<KImage>  img_list;
    bool Load(ID3D11DeviceContext* context);
    bool Frame();
    bool Render(ID3D11DeviceContext* context);
    bool Release();
};
