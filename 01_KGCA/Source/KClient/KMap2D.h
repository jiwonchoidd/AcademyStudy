#pragma once
#include "K2DAsset.h"
class KMap2D : public K2DAsset
{
private:
    ID3D11DeviceContext* m_pContext;
public:
    bool Init(ID3D11DeviceContext* context, std::wstring vs, std::wstring ps,
        std::wstring tex = nullptr, std::wstring mask = nullptr);
    bool Frame()override;
    bool Render(ID3D11DeviceContext* pContext)override;
    void HitOverlap(KCollider* pObj, DWORD dwState);
public:
    KMap2D();
    ~KMap2D();
};

