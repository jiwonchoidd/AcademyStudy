#pragma once
#include "KScene.h"
#include "KPlayer2D.h"
#include "KUI.h"
#include "KMapSpace.h"
#include "KNpc2D.h"
#include "KCamera.h"
#include "KObjObject.h"
#include "KState.h"
#include "KSoundManager.h"
class KScene_Game_0 : public KScene
{
public:
    KCamera                 m_Camera;
    float                   m_Timer;
public:
    bool    Load(std::wstring file) override;
public:
    virtual bool	Init(ID3D11DeviceContext* context);
    virtual bool	Frame();
    virtual bool	Render();
    virtual bool	Release();
};

