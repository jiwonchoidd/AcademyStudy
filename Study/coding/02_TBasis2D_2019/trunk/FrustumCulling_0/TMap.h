#pragma once
#include "TShape.h"
#include "TCamera.h"

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
	T_Box		   m_Box;
	T_Sphere	   m_Sphere;
	PCT_VERTEX*    m_vVertexList;
	TCamera*       m_pCamera;
	vector<DWORD>		m_IndexListCull;
	DWORD*				m_pIndexListCull;
	int   m_iNumCols;
	int   m_iNumRows;
	int   m_iNumVertex;
	int   m_iNumSellRows;
	int   m_iNumSellCols;
	int   m_iNumFace;
	int   m_iSellSize;
public:
	virtual float		GetHeightOfVertex(UINT iIndex);
	bool        Load(	ID3D11Device* pd3dDevice,
						ID3D11DeviceContext*	pImmediateContext,
						TCamera* pCamera,
						TMapDesc& desc);
	bool		Create(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext*	pImmediateContext,
		const TCHAR* pTextureName,
		const TCHAR* pVShaderName,
		const TCHAR* pPShaderName);
	bool CreateVertexBuffer(
		D3DXVECTOR3 vMan, D3DXVECTOR3 vMin);
	bool		Render();
	bool        Frame();
	bool		Release();
public:
	TMap();
	virtual ~TMap();
};

