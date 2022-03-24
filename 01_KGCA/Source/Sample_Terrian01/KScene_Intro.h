#pragma once
#include "KScene.h"
#include "KCamera.h"
#include "KSoundManager.h"
#include "KSkyBox.h"
#include "KMap.h"
#include "KMapLOD.h"
class KScene_Intro : public KScene
{
public:
    KSkyBox        m_SkyBox;
    KDebugCamera   m_Camera;
    KBoxObj        m_Box;
    KMap           m_Terrian;
    KMapLOD        m_Lod;
public:
    bool    Load(std::wstring file) override;
public:
    virtual bool	Init(ID3D11DeviceContext* context);
    virtual bool	Frame();
    virtual bool	Render();
    virtual bool	Release();
};

