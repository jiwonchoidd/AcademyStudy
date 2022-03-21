#pragma once
#include "KScene.h"
#include "KCamera.h"
#include "KSoundManager.h"
#include "KBoxObj.h"
class KScene_Intro : public KScene
{
public:
    KDebugCamera  m_Camera;
    KBoxObj        m_Box;
public:
    bool    Load(std::wstring file) override;
public:
    virtual bool	Init(ID3D11DeviceContext* context);
    virtual bool	Frame();
    virtual bool	Render();
    virtual bool	Release();
};

