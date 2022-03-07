#pragma once
#include "K2DAsset.h"
struct PoketmonInfo
{
    RECT            mon_rect;
    int             mon_level; //����
    float           mon_height;//Ű
    float           mon_weight;//������
    int             type; //Ÿ��
    bool            sex; //����
    std::vector<int> skill;
};

struct ItemInfo
{
    int item_id;
};

struct Slot
{
    ItemInfo slot_item;
    int slot_count;
    void Additem(int count = 1)
    {
        slot_count += count;
    }
    void Delitem(int count = 1)
    {
        slot_count -= count;
    }
};
struct KInventory
{
    std::list<Slot>         item_list;
    std::list<PoketmonInfo> poketmon_list;
};

class KPlayer2D :public K2DAsset
{
public:
    KInventory   m_inven;
    KVector2    m_pre_pos;
    bool        m_bMove;
public:
    bool Init(ID3D11DeviceContext* context, std::wstring vs, 
        std::wstring ps, std::wstring tex = nullptr, std::wstring mask= nullptr);
    bool Frame()override;
    bool Render(ID3D11DeviceContext* pContext)override;
    virtual void	ObjectOverlap(KCollider* pObj, DWORD dwState);
    virtual void	SelectOverlap(KCollider* pObj, DWORD dwState);
public:
    KPlayer2D();
    ~KPlayer2D();
};

