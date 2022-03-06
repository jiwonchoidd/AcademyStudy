#pragma once
#include "K2DAsset.h"
#include "KQuadTree.h"
#include "KCamera.h"
#include "KPlayer2D.h"
class KMap : public K2DAsset
{
public:
    bool       m_bDebugRender=false;
    KCollider* m_Player;
public:
    KQuadTree m_Space;
    void SetMatrix(KMatrix* pMatWorld, KMatrix* pMatView, KMatrix* pMatProj)override;
    bool Init(ID3D11DeviceContext* context,
        std::wstring vs, std::wstring ps,
        std::wstring tex = L"",std::wstring maks = L"");
    bool Frame()override;
    bool Render(ID3D11DeviceContext* context);
    virtual void    HitOverlap(KCollider* pObj, DWORD dwState);
    virtual void	ObjectOverlap(KCollider* pObj, DWORD dwState);
    bool Release();
public:
    KMap();
    ~KMap();
};

