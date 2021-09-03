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

class TMap : public TShape
{
public:
	PCT_VERTEX*    m_vVertexList;
public:
	virtual float		GetHeightOfVertex(UINT iIndex);
	bool        Load(	ID3D11Device* pd3dDevice,
						ID3D11DeviceContext*	pImmediateContext, 
						TMapDesc& desc);
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

