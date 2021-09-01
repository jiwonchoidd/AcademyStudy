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
struct PNCT_VERTEX
{
	D3DXVECTOR3 p; // 위치
	D3DXVECTOR3 n; // 방향
	D3DXVECTOR4 c;
	D3DXVECTOR2 t;
};

class TMap : public TShape
{
public:
	vector<PNCT_VERTEX>   m_pnctVertexList;
	T_Box		   m_Box;
	T_Sphere	   m_Sphere;
	TCamera*       m_pCamera;
	DWORD			m_iNumCollFace;
	vector<DWORD>		m_IndexListCull;
	DWORD*				m_pIndexListCull;
	int   m_iNumCols;
	int   m_iNumRows;
	int   m_iNumVertex;
	int   m_iNumSellRows;
	int   m_iNumSellCols;
	int   m_iNumFace;
	int   m_iSellSize;
	std::vector<float>			m_fVertexList;
	D3D11_TEXTURE2D_DESC		m_Desc;
public:
	float GetHeight(float fX, float fZ);
	float GetHeightmap(int row, int col);
	float Lerp(float fStart, float fEnd, float fTangent);

	bool CreateHeightMap(ID3D11Device* pd3dDevice,
		ID3D11DeviceContext*	pImmediateContext, T_STR name);
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
	D3DXVECTOR3			ComputeFaceNormal(DWORD i0, DWORD i1, DWORD i2);
	virtual bool		Render();
	virtual bool        Frame();
	virtual bool		Release();
public:
	TMap();
	virtual ~TMap();
};

