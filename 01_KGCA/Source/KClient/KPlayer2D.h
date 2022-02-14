#pragma once
#include "K2DAsset.h"
class KPlayer2D :
    public K2DAsset
{
private:
    float speed;
    ID3D11DeviceContext* m_pContext;
public:
    bool Init(ID3D11DeviceContext* context);
    bool Frame();
public:
    KPlayer2D();
    ~KPlayer2D();
};

