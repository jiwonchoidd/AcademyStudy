#pragma once
#include "KScene.h"
#include "KCamera.h"
#include "KSoundManager.h"
#include "KSkyBox.h"
#include "KMapSpace.h"
#include "KFbxLoader.h"
#include "KMiniMap.h"
#include "KRenderTarget.h"
#include "KMousePicker.h"
#include "KDepthShadow.h"
class KScene_Maptool : public KScene
{
public:
    KLight         m_Light;
    KDepthShadow   m_Shadow;
    KDebugCamera   m_Camera;
    KCamera        m_TopView;
public:
    KFbxLoader	   m_FbxLoader;
    KMousePicker   m_MousePicker;
public:
    KSkyBox        m_SkyBox;
    KMap           m_Terrian;
    KMapSpace      m_Terrian_Space;
    KMiniMap       m_MiniMap_DebugCamera;
    KMiniMap       m_MiniMap_DebugShadow;
public:
    std::vector<std::shared_ptr<K3DAsset>> m_Scene_ObjList;
public:
    bool    Load(std::wstring file) override;
public:
    virtual bool	Init(ID3D11DeviceContext* context);
    virtual bool	Frame();
    virtual bool	Render();
    virtual bool	Release();
};

