#pragma once
#include "KScene.h"
#include "KPlayer2D.h"
#include "KUI.h"
#include "KMap.h"
#include "KCamera.h"
#include "KObjObject.h"
#include "KState.h"
#include "KSoundManager.h"
class KScene_Combat : public KScene
{
public:
    KSound*                 m_BGM;
public:
    bool    Load(std::wstring file) override;
public:
    virtual bool	Init(ID3D11DeviceContext* context);
    virtual bool	Frame();
    virtual bool	Render();
    virtual bool	Release();
};