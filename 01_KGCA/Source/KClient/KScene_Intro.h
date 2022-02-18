#pragma once
#include "KScene.h"
#include "K2DAsset.h"
#include "KSoundManager.h"
class KScene_Intro : public KScene
{
public:
    KSound*         m_BGM;
public:
    bool    Load(std::wstring file) override;
public:
    virtual bool	Init(ID3D11Device* device, ID3D11DeviceContext* context);
    virtual bool	Frame();
    virtual bool	Render();
    virtual bool	Release();
};

