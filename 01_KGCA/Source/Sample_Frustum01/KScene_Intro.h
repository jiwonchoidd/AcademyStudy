#pragma once
#include "KScene.h"
#include "KCamera.h"
#include "KSoundManager.h"
#include "KSkyBox.h"
#include "KMap.h"
#include "KMapLOD.h"
class KScene_Intro : public KScene
{
public://Imgui 변수
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
    //todo: 임시 함수 나중에 클래스화 하기
    void    SetupMapObject();
public:
    KVector3 m_vIntersect;
    bool    PointInPolygon(KVector3 vert, KVector3 faceNormal,
        KVector3 v0, KVector3 v1, KVector3 v2);
    bool    GetIntersect(KVector3 vStart, KVector3 vEnd,
        KVector3 v0, KVector3 v1, KVector3 v2,
        KVector3 vNormal);
    bool    IntersectTriangle(
        const KVector3& orig, const KVector3& dir,
        KVector3& v0, KVector3& v1, KVector3& v2,
        FLOAT* t, FLOAT* u, FLOAT* v);
public:
    bool    Load(std::wstring file) override;
public:
    virtual bool	Init(ID3D11DeviceContext* context);
    virtual bool	Frame();
    virtual bool	Render();
    virtual bool	Release();
};

