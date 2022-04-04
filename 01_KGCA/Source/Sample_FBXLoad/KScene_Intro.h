#pragma once
#include "KScene.h"
#include "KCamera.h"
#include "KSoundManager.h"
#include "KSkyBox.h"
#include "KMapLOD.h"
#include "KFbxLoader.h"
#include "KMiniMap.h"
#include "KRenderTarget.h"
#include "KMousePicker.h"
#include "KDepthShadow.h"
class KScene_Intro : public KScene
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
    KMapLOD        m_Lod;
    KMiniMap       m_Minimap;
public:
    std::shared_ptr<KBoxObj> m_pBoxObj;
    std::vector<KMapObject*> m_pObjList;
public:
    //todo: 임시 함수 나중에 클래스화 하기
    void    SetupMapObject();
public:
    bool    Load(std::wstring file) override;
public:
    virtual bool	Init(ID3D11DeviceContext* context);
    virtual bool	Frame();
    virtual bool	Render();
    virtual bool	Release();
};

