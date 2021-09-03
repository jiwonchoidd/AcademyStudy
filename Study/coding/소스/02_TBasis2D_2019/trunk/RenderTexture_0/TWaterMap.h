#pragma once
#include "TMap.h"

class TWaterMap : public TMap
{
public:
	UINT		m_iHeightMap;
	UINT		m_iNormalMap;
	VSPS_WATER_CBUFFER	m_WaterCList;
	TDxBuffer			m_WaterCB;
public:
	 bool        Load(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext*	pImmediateContext,
		TMapDesc& desc);
public:
	TWaterMap();
	virtual ~TWaterMap();
};

