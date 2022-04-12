#pragma once
#include "KScene.h"
#include "KCamera.h"
#include "KSoundManager.h"
#include "KMapSpace.h"
#include "KMiniMap.h"
#include "KRenderTarget.h"
#include "KMousePicker.h"
#include "KDepthShadow.h"
#include "KFBXAsset.h"
class KScene_Maptool : public KScene
{
public:
    KLight         m_Light;
    KDepthShadow   m_Shadow;
public:
    KFbxLoader	   m_FbxLoader;
    std::vector<std::shared_ptr<KFBXAsset>> m_Scene_FBXList;
public:
    KMap           m_Terrian;
    KMapSpace      m_Terrian_Space;
public:
    KCamera        m_TopView;
    KMiniMap       m_MiniMap_DebugCamera;
    KMiniMap       m_MiniMap_DebugShadow;
public:
    KMousePicker   m_MousePicker;
    std::vector<std::shared_ptr<K3DAsset>> m_Scene_ObjList; //씬의 오브젝트 포인터
public:
    bool    Load(std::wstring file) override;
public:
    virtual bool	Init(ID3D11DeviceContext* context);
    virtual bool	Frame();
    virtual bool	Render();
    virtual bool	Release();
};

