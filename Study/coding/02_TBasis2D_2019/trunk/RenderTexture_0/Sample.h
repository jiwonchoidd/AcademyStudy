#pragma once
#pragma comment(lib,"TCoreDx_1.lib")
#include "TCoreDx.h"
#include "THeightMap.h"
#include "TWaterMap.h"
#include "TRenderTexture.h"
class Sample : public TCoreDx
{
public:
	THeightMap		m_Map;
	TWaterMap	    m_WaterMap;
	D3DXMATRIX		m_matWorldMap;
	TRenderTexture  m_ReflcetRT;
	TRenderTexture  m_RefractRT;
	TPlaneShape     m_Plane;
	float			m_fWaterHeight;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();
public:
	Sample();
	virtual ~Sample();
};

