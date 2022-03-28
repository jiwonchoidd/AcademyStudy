#pragma once
#include "KScene.h"
#include "KCamera.h"
#include "KSoundManager.h"
#include "KSkyBox.h"
#include "KMap.h"
#include "KMapLOD.h"
class KScene_Intro : public KScene
{
public://Imgui º¯¼ö
    float          m_LightPos[3] = {0,100,0};
    float          m_LightColor[3] = { 0.9f,0.9f,0.85f};
public:
    KSkyBox        m_SkyBox;
    KDebugCamera   m_Camera;
    KCamera        m_TopView;
    KBoxObj        m_Box;
    KMap           m_Terrian;
    KMapLOD        m_Lod;
public:
    std::shared_ptr<KBoxObj> m_pBoxObj;
    std::vector<KMapObject*> m_pObjList;
public:
    void    SetupMapObject();
    bool    Load(std::wstring file) override;
public:
    virtual bool	Init(ID3D11DeviceContext* context);
    virtual bool	Frame();
    virtual bool	Render();
    virtual bool	Release();
};

