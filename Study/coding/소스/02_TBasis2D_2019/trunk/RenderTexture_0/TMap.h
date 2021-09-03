#pragma once
#include "TShape.h"
struct TMapDesc
{
	UINT   width;
	UINT   height;
	UINT   size;
	T_STR  texturename;
	T_STR  shadername;
};
struct VSPS_WATER_CBUFFER
{
	float x, y, z, w;
	float time; // 컬러 // 레지스터 단위 (float*4)
	float a, b, c;
};
class TMap : public TShape
{
public:
	PCT_VERTEX*    m_vVertexList;
public:
	virtual float		GetHeightOfVertex(UINT iIndex);
	virtual  bool        Load(	ID3D11Device* pd3dDevice,
						ID3D11DeviceContext*	pImmediateContext, 
						TMapDesc& desc);
	virtual  bool        LoadAdd(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext*	pImmediateContext,
		const TCHAR* pVSShaderName, 
		const TCHAR* pPShaderName) {
		return true;
	};
	bool		Create(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext*	pImmediateContext,
		const TCHAR* pTextureName,
		const TCHAR* pVShaderName,
		const TCHAR* pPShaderName);
	bool CreateVertexBuffer(
		D3DXVECTOR3 vMan, D3DXVECTOR3 vMin);
	bool		Render();
	bool		Release();
public:
	TMap();
	virtual ~TMap();
};

