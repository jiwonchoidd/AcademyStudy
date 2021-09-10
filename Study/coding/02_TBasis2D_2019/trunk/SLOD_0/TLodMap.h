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
	// Ʈ���� ������ ���̿� ���� �� ����
	int			m_iNumCell;
	// LOD ���� ����( z = pow( x,y )���� y = log(z) / log(x) ) 
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

