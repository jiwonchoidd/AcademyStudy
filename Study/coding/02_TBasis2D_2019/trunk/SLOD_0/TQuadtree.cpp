#include "TQuadtree.h"
#include <queue>

typedef struct T_POINT
{
	float  x;
	float  y;
	float  z;
} t_POINT;

TNode::TNode()
{
	m_bLeafNode = false;
	m_pChild[0] = NULL;
	m_pChild[1] = NULL;
	m_pChild[2] = NULL;
	m_pChild[3] = NULL;
	m_pParent = NULL;
	//m_dwLodLevel = rand() % 4;
}
TNode::~TNode()
{
	m_IndexList.clear();
	m_VertexList.clear();
	m_ObjList.clear();	
	m_VB.Release();
	m_IB.Release();
	SAFE_DEL(m_pChild[0]);
	SAFE_DEL(m_pChild[1]);
	SAFE_DEL(m_pChild[2]);
	SAFE_DEL(m_pChild[3]);
}
int TQuadtree::DrawChild(TNode* pNode)
{	
	if (pNode==NULL) return 0;
	int iNumDraw = 0;
	for (int iObj = 0; iObj < pNode->m_ObjList.size(); iObj++)
	{
		if (pNode->m_ObjList[iObj]->m_bDraw == false)
		{
			pNode->m_ObjList[iObj]->Draw(m_matView, m_matProj);
			iNumDraw++;
		}
	}	
	return iNumDraw;
}
T_POSITION TQuadtree::CheckRect(TNode* pNode, TMapObj* pObj)
{
	// 완전 포함의 경우
	if (pNode->m_box.vMin.x <= pObj->m_tBox.vMin.x
		&& pNode->m_box.vMax.x >= pObj->m_tBox.vMax.x)
	{
		if (pNode->m_box.vMin.z <= pObj->m_tBox.vMin.z
			&& pNode->m_box.vMax.z >= pObj->m_tBox.vMax.z)
		{
			return P_BACK;
		}
	}	

	t_POINT  cDesk, cSrc, Radius, DeskRadius, SrcRadius;
	cDesk.x = (pNode->m_box.vMax.x + pNode->m_box.vMin.x) / 2;
	cDesk.z = (pNode->m_box.vMax.z + pNode->m_box.vMin.z) / 2;
	cSrc.x = (pObj->m_tBox.vMax.x + pObj->m_tBox.vMin.x) / 2;
	cSrc.z = (pObj->m_tBox.vMax.z + pObj->m_tBox.vMin.z) / 2;

	Radius.x = abs(cDesk.x - cSrc.x);
	Radius.z = abs(cDesk.z - cSrc.z);

	DeskRadius.x = (pNode->m_box.vMax.x - pNode->m_box.vMin.x) / 2;
	DeskRadius.z = (pNode->m_box.vMax.z - pNode->m_box.vMin.z) / 2;
	SrcRadius.x = (pObj->m_tBox.vMax.x - pObj->m_tBox.vMin.x) / 2;
	SrcRadius.z = (pObj->m_tBox.vMax.x - pObj->m_tBox.vMin.x) / 2;
	if (Radius.x <= (DeskRadius.x + SrcRadius.x) &&
		Radius.z <= (DeskRadius.z + SrcRadius.z))
	{
		return P_SPANNING;
	}
	return P_FRONT;
}
// 오브젝트를 모든 자식 노드에 추가한다.
void TQuadtree::AddObject(TNode* pNode, TMapObj* pObj)
{
	if (pNode == NULL) return;
	T_POSITION pos = CheckRect(pNode, pObj);
	// 완전 제외 되었으면
	if (pos == P_FRONT)
	{
		return;
	}	
	pNode->m_ObjList.push_back(pObj);
	for (int iChild = 0; iChild < 4; iChild++)
	{
		AddObject(pNode->m_pChild[iChild], pObj);
	}
}
// 완벽하게 포함된 노드를 찾아 반환한다.
TNode* TQuadtree::FindNode(TNode* pNode, TMapObj* pObj)
{
	std::queue<TNode*> m_QuadTreeQueue;
	do {
		for (DWORD dwChild = 0; dwChild < 4; dwChild++)
		{
			if (pNode->m_pChild[dwChild] && 
				P_BACK == CheckRect(pNode->m_pChild[dwChild], pObj))
			{
				m_QuadTreeQueue.push(pNode->m_pChild[dwChild]);
				break;
			}
		}
		if (m_QuadTreeQueue.empty()) break;

		pNode = m_QuadTreeQueue.front();
		m_QuadTreeQueue.pop();
	} while (pNode);
	return pNode;
}
void  TQuadtree::AddObject(TMapObj*  pObj)
{
	// 1) 해당 오브젝트를 포함하는 노드 반환 함수.
	TNode* pNode = FindNode(m_pRootNode, pObj);
	_ASSERT(pNode);
	pNode->m_ObjList.push_back(pObj);
	
};
void TQuadtree::DrawFindNode(TNode* pNode)
{
	if (pNode == NULL) return;
	T_POSITION pos = m_pCamera->m_Frustum.CheckPositionOBBInPlane(&pNode->m_box);
	// 완전 제외 되었으면
	if (pos == P_BACK)
	{
		return;
	}
	// 리프노드이면서 걸쳐 있으면
	if ( pNode->m_bLeafNode && pos != P_BACK)
	{
		m_RenderList.push_back(pNode);
		return;
	}
	//// 완전 포함, 자식노드 탐색 불 필요
	if (pos == P_FRONT)
	{
		m_RenderList.push_back(pNode);			
		return;
	}	
	// 리프노드가 아니면서 걸쳐 있으면 spanning
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

	ldiv_t divValue = ldiv(a, this->m_iNumCols);
	pNode->m_dwPositionIndex[0] = divValue.rem / (b - a); // 나머지-> X
	pNode->m_dwPositionIndex[1] = divValue.quot / (b - a);// 몫 -> Y

	if (pParent != NULL)
	{
		pNode->m_dwDepth = pParent->m_dwDepth + 1;
		pNode->m_pParent = pParent;
		if (m_dwNumDepth < pNode->m_dwDepth)
		{
			m_dwNumDepth = pNode->m_dwDepth;
		}
	}
	if ((b-a) <= 1 || pNode->m_dwDepth > 2)
	{
		pNode->m_bLeafNode = true;
		CreateBuffer(pNode);
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
	
	for (DWORD dwRow = dwStartRow; dwRow <= dwEndRow; dwRow++)
	{
		for (DWORD dwCol = dwStartCol; dwCol <=dwEndCol; dwCol++)
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
		for (int iChild = 0; iChild < 4; iChild++)
		{
			DWORD dwIndex = pNode->m_pChild[iChild]->m_dwPositionIndex[1] * 
							pow(2.0f, (float)pNode->m_pChild[iChild]->m_dwDepth) +
							pNode->m_pChild[iChild]->m_dwPositionIndex[0];

			m_LevelList[pNode->m_pChild[iChild]->m_dwDepth][dwIndex] 
								= pNode->m_pChild[iChild];

			BuildTree(pNode->m_pChild[iChild]);
		}		
	}
	return true;
}
bool  TQuadtree::SubDivide( TNode* pNode)
{
	if (pNode == NULL) return false;
	//float fDistance = pNode->m_box.vMax.x - pNode->m_box.vMin.x;
	DWORD dwWidth = (pNode->m_dwConer[1] - pNode->m_dwConer[0]);
	DWORD dwHeight = (pNode->m_dwConer[3] - pNode->m_dwConer[0]);

	if (pNode->m_bLeafNode==true)
	{
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
	m_dwNumDepth = 0;
}


TQuadtree::~TQuadtree()
{
	m_RenderList.clear();
	m_LevelList.clear();
	SAFE_DEL(m_pRootNode);
}
