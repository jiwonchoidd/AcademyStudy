#pragma once
#include "K2DAsset.h"
class KPlayer2D :public K2DAsset
{
private:
    ID3D11DeviceContext* m_pContext;
public:
    bool Init(ID3D11DeviceContext* context, std::wstring vs, 
        std::wstring ps, std::wstring tex = nullptr, std::wstring mask= nullptr);
    bool Frame()override;
    void HitOverlap(KCollider* pObj, DWORD dwState);
    void SelectOverlap(KCollider* pObj, DWORD dwState);
public:
    KPlayer2D();
    ~KPlayer2D();
};
