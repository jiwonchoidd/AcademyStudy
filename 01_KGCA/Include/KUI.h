#pragma once
#include "KUIModelManager.h"
#include "KSoundManager.h"
#include "KObjectManager.h"
struct KUIData
{
    KTexture* pTex;
    KSound* pSound;

    KUIData() 
    {
        pTex = nullptr;
        pSound = nullptr;
    }
    KUIData(KTexture* a, KSound* b)
    {
        pTex = a;
        pSound = b;
    }
};
// ��� ���� KObject->KCollider->K2DAsset->KUIModel->KUI
class KUI : public KUIModel
{
private:
    KUIModel* Clone()
    {
        KUIModel* copy = new KUI(*this);
        copy->CreateVertexBuffer();
        copy->CreateIndexBuffer();
        copy->CreateConstantBuffer();
        copy->CreateVertexLayout();
        return copy;
    }
    void  UpdateData()
    {
        m_rtColl = KRect(m_pos, m_rtSize.width, m_rtSize.height);
        m_matWorld._41 = m_pos.x;
        m_matWorld._42 = m_pos.y;
        g_ObjManager.AddCollisionExecute(this,
            std::bind(&KCollider::ObjectOverlap, this,
                std::placeholders::_1,
                std::placeholders::_2));
        g_ObjManager.AddSelectExecute(this,
            std::bind(&KCollider::SelectOverlap, this,
                std::placeholders::_1,
                std::placeholders::_2));
    }
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

    // ��üũ�� �̹��� ���� ���� ���� UI�� 9������� ����,
    bool SetVertexData()override;
    bool SetIndexData()override;
public:
    KUI();
    ~KUI();
};

class KImage : public KUI
{
private:
    KUIModel* Clone()
    {
        KImage* copy = new KImage(*this);
        copy->CreateVertexBuffer();
        copy->CreateIndexBuffer();
        copy->CreateConstantBuffer();
        copy->CreateVertexLayout();

        return copy;
    }
    void  UpdateData()
    {
        m_rtColl = KRect(m_pos, m_rtSize.width, m_rtSize.height);
        m_matWorld._41 = m_pos.x;
        m_matWorld._42 = m_pos.y;
        g_ObjManager.AddCollisionExecute(this,
            std::bind(&KCollider::ObjectOverlap, this,
                std::placeholders::_1,
                std::placeholders::_2));
        g_ObjManager.AddSelectExecute(this,
            std::bind(&KCollider::SelectOverlap, this,
                std::placeholders::_1,
                std::placeholders::_2));
    }
public:  
    virtual bool    Frame()override;
    virtual void    FadeIn();
    virtual void    FadeOut();
};

class KButton : public KUI
{
private:
    KUIModel* Clone()
    {
        KButton* copy = new KButton(*this);
        copy->CreateVertexBuffer();
        copy->CreateIndexBuffer();
        copy->CreateConstantBuffer();
        copy->CreateVertexLayout();

        return copy;
    }
    void  UpdateData()
    {
        m_rtColl = KRect(m_pos, m_rtSize.width, m_rtSize.height);
        m_matWorld._41 = m_pos.x;
        m_matWorld._42 = m_pos.y;
        g_ObjManager.AddCollisionExecute(this,
            std::bind(&KCollider::ObjectOverlap, this,
                std::placeholders::_1,
                std::placeholders::_2));
        g_ObjManager.AddSelectExecute(this,
            std::bind(&KCollider::SelectOverlap, this,
                std::placeholders::_1,
                std::placeholders::_2));
    }
public:
    bool m_btn_active = false;
public:
    virtual void	SelectOverlap(KCollider* pObj, DWORD dwState) override;
    virtual bool    Frame()override;
};