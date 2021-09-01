#pragma once
#include "TQuadtree.h"
const int g_iLodLevel = 4;

class TLodMap : public TQuadtree
{
public:	
	TDxBuffer     m_StaticIB[g_iLodLevel]/*[16]*/;
	DWORD		  m_NumIndexStatic[g_iLodLevel]/*[16]*/;
	TDxBuffer	  m_LodVB;
	vector<PNCT_VERTEX>  m_LodVertexList;
	// 트리의 레벨의 깊이에 따른 셀 개수
	int			m_iNumCell;
	// LOD 레벨 개수( z = pow( x,y )에서 y = log(z) / log(x) ) 
	int			m_iPatchLodCount;
	
public:
	void   InitLOD( TNode* pNodex, UINT iDepth );
	void   FindNeighborNode(TNode* pNode);
	void   SetNeighborNode(TNode* pNode);
	void   DrawFindNode(TNode* pNode);
	
	bool	Frame();
	bool	Render();
	bool	CreateBuffer(TNode* pNode);
	bool	Start(ID3D11Device* pd3dDevice, int iNumWidth, int iNumHeight);
	bool	CreateIndexBufferLOD(ID3D11Device* pd3dDevice, int iLod, int iNumWidth, int iNumHeight);
	void	SetLOD(DWORD dwWidth, int iNumDepth);
	DWORD	GetLodSubIndex(TNode* pNode);
	DWORD	GetLodType(TNode* pNode);
	float	GetExpansionRatio(D3DXVECTOR3 vCenter);
public:
	TLodMap();
	virtual ~TLodMap();
};

