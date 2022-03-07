#pragma once
#include "K2DAsset.h"
#include "KPlayer2D.h"

class KCombat
{
public:
    ID3D11DeviceContext* m_pContext;
    KPlayer2D*          m_pPlayer; // ���ϸ�, �κ��丮 
    PoketmonInfo m_enemy_info; // �� ���ϸ�
public:
    bool Init(ID3D11DeviceContext* context, KPlayer2D* player,int id= -1);
    bool Frame();
    bool Render(ID3D11DeviceContext* pContext);
    bool Release();
    virtual void	ObjectOverlap(KCollider* pObj, DWORD dwState);
public:
    KCombat();
    ~KCombat();
};

