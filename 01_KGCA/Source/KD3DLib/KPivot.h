#pragma once
#include "KBoxObj.h"
class KPivot
{
public:
	KBox	m_BoxAxis[3];
	KBoxObj	m_Pivot_Box;// Temporary debug object
public:
	void DrawDebugInit(ID3D11DeviceContext* pContext);
	void DrawDebugRender(KBox* pBox, ID3D11DeviceContext* pContext, KVector4 color);
public:
	bool Init(ID3D11DeviceContext* pContext);
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
public:
	KPivot();
	~KPivot();
};

