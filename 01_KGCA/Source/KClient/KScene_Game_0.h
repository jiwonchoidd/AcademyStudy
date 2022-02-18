#pragma once
#include "KScene.h"
#include "K2DAsset.h"
#include "KSound.h"
#include "KPlayer2D.h"
#include "KNpc2D.h"
#include "KPlaneObj.h"
#include "KBoxObj.h"
#include "KCamera.h"
#include "KSkyBox.h"
#include "KState.h"
#include "KSoundManager.h"
class KScene_Game_0 : public KScene
{
public:
    KSkyBox                 m_Skybox;
    KDebugCamera            m_Camera;
    KSound*                 m_BGM;
    KPlayer2D			    m_PlayerObj;
    std::vector<KNpc2D*>    m_NpcLlist;
    KPlaneObj			    m_Plane;
    KBoxObj				    m_Box;
public:
    bool    Load(std::wstring file) override;
public:
    virtual bool	Init(ID3D11Device* device, ID3D11DeviceContext* context);
    virtual bool	Frame();
    virtual bool	Render();
    virtual bool	Release();
};

