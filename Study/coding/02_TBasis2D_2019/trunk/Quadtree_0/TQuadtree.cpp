#include "TQuadtree.h"
TNode::TNode()
{
	m_bLeafNode = false;
	m_pChild[0] = NULL;
	m_pChild[1] = NULL;
	m_pChild[2] = NULL;
	m_pChild[3] = NULL;
}
TNode::~TNode()
{
	SAFE_DEL(m_pChild[0]);
	SAFE_DEL(m_pChild[1]);
	SAFE_DEL(m_pChild[2]);
	SAFE_DEL(m_pChild[3]);
}


void TQuadtree::DrawFindNode(TNode* pNode)
{
	if (pNode == NULL) return;
	T_POSITION pos = m_pCamera->m_Frustum.CheckPositionOBBInPlane(&pNode->m_box);
	if (pos == P_FRONT)
	{
		return;
	}
	// 걸쳐 있거나 완전 포함되면 + 리프노트이면
	if ( pNode->m_bLeafNode && pos != P_FRONT)
	{
		m_RenderList.push_back(pNode);
		return;
	}
	// 완전 포함, 자식노드 탐색 불 필요
	if (pos == P_BACK)
	{
		m_RenderList.push_back(pNode);			
		return;
	}	
	// spanning
	for (int iChild = 0; iChild < 4; iChild++)
	{
		DrawFindNode(pNode->m_pChild[iChild]);
	}
}
int  TQuadtree::UpdateIndexList(
	DWORD dwCurrentIndex,
	DWORD u_left, DWORD u_right,
	DWORD b_right, DWORD b_left)
{
	DWORD dwStartRow = u_left / m_iNumCols;
	DWORD dwEndRow = b_right / m_iNumCols;
	DWORD dwStartCol = u_left % m_iNumCols;
	DWORD dwEndCol = u_right % m_iNumCols;

	int iNumFaces = 0;
	for (DWORD iRow = dwStartRow; iRow < dwEndRow; iRow++)
	{
		for (DWORD iCol = dwStartCol; iCol < dwEndCol; iCol++)
		{
			m_IndexListCull[dwCurrentIndex + 0] = iRow * m_iNumCols + iCol;
			m_IndexListCull[dwCurrentIndex + 1] = iRow * m_iNumCols + iCol + 1;
			m_IndexListCull[dwCurrentIndex + 2] = (iRow + 1) * m_iNumCols + iCol;

			m_IndexListCull[dwCurrentIndex + 3] = m_IndexListCull[dwCurrentIndex + 2];
			m_IndexListCull[dwCurrentIndex + 4] = m_IndexListCull[dwCurrentIndex + 1];
			m_IndexListCull[dwCurrentIndex + 5] = (iRow + 1) * m_iNumCols + iCol + 1;
			dwCurrentIndex += 6;
			iNumFaces += 2;
		}
	}
	return iNumFaces;
}
bool TQuadtree::Frame()
{
	//return TMap::Frame();
	m_RenderList.clear();
	DrawFindNode(m_pRootNode);

	m_iNumCollFace = 0;
	for (int iNode = 0; iNode < m_RenderList.size(); iNode++)
	{
		m_iNumCollFace += UpdateIndexList(
			m_iNumCollFace * 3,
			m_RenderList[iNode]->m_dwConer[0],
			m_RenderList[iNode]->m_dwConer[1], 
			m_RenderList[iNode]->m_dwConer[2], 
			m_RenderList[iNode]->m_dwConer[3] );
	}
	m_pImmediateContext->UpdateSubresource(m_IB.m_pBuffer,
		0, NULL,
		&m_IndexListCull.at(0),
		//m_pIndexListCull,
		m_iNumCollFace * sizeof(DWORD),
		0);
	return true;
}
TNode* TQuadtree::CreateNode( TNode* pParent, DWORD a, 
	DWORD b, DWORD c,   DWORD d)
{
	TNode* pNode = new TNode;
	pNode->m_dwConer[0] = a;
	pNode->m_dwConer[1] = b;
	pNode->m_dwConer[2] = c;
	pNode->m_dwConer[3] = d;
	pNode->m_dwDepth = 0;
	if (pParent != NULL)
	{
		pNode->m_dwDepth = pParent->m_dwDepth + 1;
	}
	//  a      b     c
	//  d      e(4)  f
	//  g      h(7)  i
	DWORD dwStartRow = pNode->m_dwConer[0] / m_iNumCols;
	DWORD dwEndRow = pNode->m_dwConer[3] / m_iNumCols;
	DWORD dwStartCol = pNode->m_dwConer[0] % m_iNumCols;
	DWORD dwEndCol = pNode->m_dwConer[1] % m_iNumCols;

	pNode->m_box.vMin = D3DXVECTOR3(1000000.0f, 1000000.0f, 1000000.0f);
	pNode->m_box.vMax = D3DXVECTOR3(-1000000.0f, -1000000.0f, -1000000.0f);
	
	for (DWORD dwRow = dwStartRow; dwRow < dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol <dwEndCol; dwCol++)
		{
			DWORD iCurrent = dwRow * m_iNumCols+ dwCol;
			//BoundBox
			if (pNode->m_box.vMax.x < m_pnctVertexList[iCurrent].p.x)
				pNode->m_box.vMax.x = m_pnctVertexList[iCurrent].p.x;
			if (pNode->m_box.vMax.y < m_pnctVertexList[iCurrent].p.y)
				pNode->m_box.vMax.y = m_pnctVertexList[iCurrent].p.y;
			if (pNode->m_box.vMax.z < m_pnctVertexList[iCurrent].p.z)
				pNode->m_box.vMax.z = m_pnctVertexList[iCurrent].p.z;
			if (pNode->m_box.vMin.x > m_pnctVertexList[iCurrent].p.x)
				pNode->m_box.vMin.x = m_pnctVertexList[iCurrent].p.x;
			if (pNode->m_box.vMin.y > m_pnctVertexList[iCurrent].p.y)
				pNode->m_box.vMin.y = m_pnctVertexList[iCurrent].p.y;
			if (pNode->m_box.vMin.z > m_pnctVertexList[iCurrent].p.z)
				pNode->m_box.vMin.z = m_pnctVertexList[iCurrent].p.z;
			//obb
			pNode->m_box.vAxis[0] = D3DXVECTOR3(1,0,0);
			pNode->m_box.vAxis[1] = D3DXVECTOR3(0, 1, 0);
			pNode->m_box.vAxis[2] = D3DXVECTOR3(0, 0, 1);
			pNode->m_box.vCenter = (pNode->m_box.vMax + pNode->m_box.vMin ) / 2;
			pNode->m_box.fExtent[0] = pNode->m_box.vMax.x - pNode->m_box.vCenter.x;
			pNode->m_box.fExtent[1] = pNode->m_box.vMax.y - pNode->m_box.vCenter.y;
			pNode->m_box.fExtent[2] = pNode->m_box.vMax.z - pNode->m_box.vCenter.z;
		}
	}
	return pNode;
}
bool  TQuadtree::BuildTree(TNode* pNode)
{
	if ( SubDivide( pNode ) )
	{
		BuildTree(pNode->m_pChild[0]);
		BuildTree(pNode->m_pChild[1]);
		BuildTree(pNode->m_pChild[2]);
		BuildTree(pNode->m_pChild[3]);
	}
	return true;
}
bool  TQuadtree::SubDivide( TNode* pNode)
{
	if (pNode == NULL) return false;
	//float fDistance = pNode->m_box.vMax.x - pNode->m_box.vMin.x;
	DWORD dwWidth = (pNode->m_dwConer[1] - pNode->m_dwConer[0]);
	DWORD dwHeight = (pNode->m_dwConer[3] - pNode->m_dwConer[0]);

	if ( dwWidth <= 1 || pNode->m_dwDepth > 3)
	{
		pNode->m_bLeafNode = true;
		return false;
	}
	//pNode->m_dwConer[0] ~ [3]
	// 0           a=1               2 
	// b(3)        c(4)              d(5)
	// 6           e(7)              8
	DWORD a, b, c, d, e;
	a = pNode->m_dwConer[0] + dwWidth / 2;
	b = pNode->m_dwConer[0] + dwHeight / 2;
	d = pNode->m_dwConer[1] + dwHeight / 2;
	e = pNode->m_dwConer[3] + dwWidth / 2;
	c = a + dwHeight / 2;
	pNode->m_pChild[0] = CreateNode(pNode, pNode->m_dwConer[0], a, c, b);
	pNode->m_pChild[1] = CreateNode(pNode, a, pNode->m_dwConer[1],d,c);
	pNode->m_pChild[2] = CreateNode(pNode, c,d, pNode->m_dwConer[2], e);
	pNode->m_pChild[3] = CreateNode(pNode, b,c,e, pNode->m_dwConer[3]);
	return true;
}
TQuadtree::TQuadtree()
{
}


TQuadtree::~TQuadtree()
{
	SAFE_DEL(m_pRootNode);
}
