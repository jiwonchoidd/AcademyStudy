#pragma once
#pragma comment(lib,"TCoreDx_1.lib")
#include "TCoreDx.h"
#include "TLodMap.h"
#include "TObjectBox.h"

struct VSPS_LIGHT_CBUFFER
{
	D3DXVECTOR4 vLight;	
	D3DXVECTOR4 vLightColor;
	D3DXVECTOR4 vMaterialColor;
	D3DXVECTOR4 vAmbientColor;
	float time; // 컬러 // 레지스터 단위 (float*4)
	float a, b, c;
};

class Sample : public TCoreDx
{
public:
	D3DXMATRIX			m_matWorldMap;
	TLineShape			m_Line;
	TLodMap				m_Quadtree;
	VSPS_LIGHT_CBUFFER	m_WaterCList;
	TDxBuffer			m_WaterCB;
	TObjectBox			m_BoxObj;
	TMapObj				m_MapObj[100];
public:
	bool	Init();
	bool	PreFrame();
	bool	Frame();
	bool	Render();
	bool	Release();
	void    DrawBoundingBox(T_Box box, D3DXVECTOR4  vColor);
	void    DrawQuadtreeBoundingBox(TNode* pNode);
public:
	Sample();
	virtual ~Sample();
};

