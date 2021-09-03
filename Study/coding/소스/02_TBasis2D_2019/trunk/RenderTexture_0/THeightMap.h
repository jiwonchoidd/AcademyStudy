#pragma once
#include "TMap.h"
class THeightMap : public TMap
{
public:
	std::vector<float>			m_fVertexList;
	D3D11_TEXTURE2D_DESC		m_Desc;
	UINT	m_iReflectVS;
	UINT	m_iReflectPS;
	UINT	m_iRefractVS;
	UINT	m_iRefractPS;
	TShader*			m_pReflectVS;
	TShader*			m_pReflectPS;
	TShader*			m_pRefractVS;
	TShader*			m_pRefractPS;
	VSPS_WATER_CBUFFER	m_WaterCList;
	TDxBuffer			m_WaterCB;
public:
	bool CreateHeightMap(ID3D11Device* pd3dDevice, 
		ID3D11DeviceContext*	pImmediateContext, T_STR name);
	float		GetHeightOfVertex(UINT iIndex);
	bool		LoadAdd(ID3D11Device* pd3dDevice,
						ID3D11DeviceContext*	pImmediateContext,
						const TCHAR* pVSShaderName,
						const TCHAR* pPShaderName);
	bool		RenderRefract();
	bool		RenderReflect();
public:
	THeightMap();
	virtual ~THeightMap();
};

