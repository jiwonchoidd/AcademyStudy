#pragma once
#include "KPlaneObj.h"
#include "KRenderTarget.h"
class KMiniMap : public KPlaneObj
{
public:
	KVector2		m_MinMax;
	KRenderTarget	m_Rt;
public:
	bool Init(float min, float max);
	bool Render(ID3D11DeviceContext* m_pContext);
public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
public:
	bool Release();
};

