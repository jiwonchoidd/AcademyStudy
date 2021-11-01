#pragma once
#include "KCore.h"
#include "KShape.h"
class KMiniMap : public KPlaneShape
{
public:
    bool		LoadTexture()
    {
        m_Tex.LoadTexture(L"../../data/charport.bmp");
    }
    bool CreateVertexData()
    {
        m_pVertexList.resize(4);

        m_pVertexList[0].pos = KVector3(-1.0f, -0.5f, 0.0f);
        m_pVertexList[0].normal = KVector3(0.0f, 0.0f, -1.0f);
        m_pVertexList[0].color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
        m_pVertexList[0].tex = KVector2(0.0f, 0.0f);

        m_pVertexList[1].pos = KVector3(-0.5f, -0.5f, 0.0f);
        m_pVertexList[1].normal = KVector3(0.0f, 0.0f, -1.0f);
        m_pVertexList[1].color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
        m_pVertexList[1].tex = KVector2(1.0f, 0.0f);


        m_pVertexList[2].pos = KVector3(-1.0f, -1.0f, 0.0f);
        m_pVertexList[2].normal = KVector3(0.0f, 0.0f, -1.0f);
        m_pVertexList[2].color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
        m_pVertexList[2].tex = KVector2(0.0f, 1.0f);


        m_pVertexList[3].pos = KVector3(-0.5f, -1.0f, 0.0f);
        m_pVertexList[3].normal = KVector3(0.0f, 0.0f, -1.0f);
        m_pVertexList[3].color = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
        m_pVertexList[3].tex = KVector2(1.0f, 1.0f);
        return true;
    }
};
class Sample : public KCore
{
    KMiniMap            m_MiniMap;
	KPlaneShape			m_MapObj;
	KRenderTarget		m_Rt;
	KVector4			m_lightPos;
	KVector3			m_MovePos;
	KPlane				m_Plane;
	KFbxObj				m_FbxObjA;
	KFbxObj				m_FbxObjB;
	KTexture			m_Texture;
	KMatrix				m_matShadow;
	ID3D11PixelShader*	m_pPSShadow = nullptr;
public:
	bool		Init()override;
	bool		Frame()override;
	bool		Render()override;
	bool		Release()override;
public:
	Sample();
	virtual ~Sample();
};

