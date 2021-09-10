#include "TLodMap.h"
#include "TTexture.h"
#include "TDxState.h"
float TLodMap::GetExpansionRatio(D3DXVECTOR3 vCenter)
{
	float fRatio = D3DXVec3Length(&(m_pCamera->m_vEye - vCenter)) / 100.0f;//m_pCamera->m_fFarPlane;
	if (fRatio >= 1.0f)
	{
    	return 1.0f;
	}
	return fRatio;
}
DWORD TLodMap::GetLodSubIndex(TNode* pNode)
{
	int iNumPatchIndex = m_iNumCell;	
	float fRatio = GetExpansionRatio(pNode->m_box.vCenter);
	DWORD dwCurentRatio = fRatio * m_iPatchLodCount;
	pNode->m_dwLodLevel = dwCurentRatio;	
	return pNode->m_dwLodLevel;
}
DWORD TLodMap::GetLodType(TNode* pNode)
{
	if (pNode == 0) return 0;

	DWORD dwType = 0;
	if (pNode->m_pNeighborList[0] && pNode->m_pNeighborList[0]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 1;
	if (pNode->m_pNeighborList[1] && pNode->m_pNeighborList[1]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 4;
	if (pNode->m_pNeighborList[2] && pNode->m_pNeighborList[2]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 8;
	if (pNode->m_pNeighborList[3] && pNode->m_pNeighborList[3]->m_dwLodLevel < pNode->m_dwLodLevel) dwType += 2;

	pNode->m_dwLodType = dwType;
	return dwType;
}
void  TLodMap::SetLOD(DWORD dwWidth, int iNumDepth)
{
	m_iNumCell = (dwWidth - 1) / pow(2.0f, (float)iNumDepth);
	// LOD 레벨 개수( z = pow( x,y )에서 y = log(z) / log(x) ) 
	m_iPatchLodCount = (log((float)m_iNumCell) / log(2.0f));
}
bool TLodMap::CreateIndexBufferLOD(ID3D11Device* pd3dDevice, int iLod, int iNumWidth, int iNumHeight)
{
	DWORD dwCurrentIndex = 0;
	vector<DWORD>  indexlist;

	int iNumCols = iNumWidth;
	int iNumRows = iNumHeight;
	int iNumVertex = iNumRows * iNumCols;
	int iNumSellRows = iNumRows - 1;
	int iNumSellCols = iNumCols - 1;
	int iLodStep = pow(2, iLod);
	int iNumFace = (iNumSellRows/ iLodStep) *
					(iNumSellCols / iLodStep) * 2;
	
	indexlist.resize(iNumFace * 3);

	for (DWORD iRow = 0; iRow < iNumSellRows; iRow += iLodStep)
	{
		for (DWORD iCol = 0; iCol < iNumSellCols; iCol += iLodStep)
		{
			indexlist[dwCurrentIndex + 0] = iRow * iNumRows + iCol;
			indexlist[dwCurrentIndex + 1] = iRow * iNumRows + iCol + iLodStep;
			indexlist[dwCurrentIndex + 2] = (iRow + iLodStep) * iNumRows + iCol;

			indexlist[dwCurrentIndex + 3] = indexlist[dwCurrentIndex + 2];
			indexlist[dwCurrentIndex + 4] = indexlist[dwCurrentIndex + 1];
			indexlist[dwCurrentIndex + 5] = (iRow + iLodStep) * iNumRows + iCol + iLodStep;
			dwCurrentIndex += 6;
		}
	}
	m_NumIndexStatic[iLod] = indexlist.size();
	m_StaticIB[iLod].Create(pd3dDevice,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_INDEX_BUFFER,
		sizeof(DWORD),
		indexlist.size(),
		&indexlist.at(0));
	
	indexlist.clear();
	return true;
}
bool TLodMap::Start(ID3D11Device* pd3dDevice, int iNumWidth, int iNumHeight)
{
	CreateIndexBufferLOD(pd3dDevice, 0, iNumWidth, iNumHeight);
	CreateIndexBufferLOD(pd3dDevice, 1, iNumWidth, iNumHeight);
	CreateIndexBufferLOD(pd3dDevice, 2, iNumWidth, iNumHeight);
	CreateIndexBufferLOD(pd3dDevice, 3, iNumWidth, iNumHeight);
	return true;
}
bool	TLodMap::CreateBuffer(TNode* pNode)
{
	_ASSERT(pNode);
	DWORD dwStartRow = pNode->m_dwConer[0] / m_iNumCols;
	DWORD dwEndRow = pNode->m_dwConer[3] / m_iNumCols;
	DWORD dwStartCol = pNode->m_dwConer[0] % m_iNumCols;
	DWORD dwEndCol = pNode->m_dwConer[1] % m_iNumCols;
	DWORD dwCurrentIndex = 0;
	//int iNumFaces = 0;
	/////////////////////////////////////////////////////////////////////
	// node 단위로 인덱스 버퍼 1개씩 생성시 사용된다.
	/////////////////////////////////////////////////////////////////////
	//int iSize = (dwEndRow - dwStartRow) * (dwEndCol- dwStartCol) * 6;
	//pNode->m_IndexList.resize(iSize);

	//for (DWORD iRow = dwStartRow; iRow < dwEndRow; iRow++)
	//{
	//	for (DWORD iCol = dwStartCol; iCol < dwEndCol; iCol++)
	//	{
	//		pNode->m_IndexList[dwCurrentIndex + 0] = iRow * m_iNumCols + iCol;
	//		pNode->m_IndexList[dwCurrentIndex + 1] = iRow * m_iNumCols + iCol + 1;
	//		pNode->m_IndexList[dwCurrentIndex + 2] = (iRow + 1) * m_iNumCols + iCol;

	//		pNode->m_IndexList[dwCurrentIndex + 3] = pNode->m_IndexList[dwCurrentIndex + 2];
	//		pNode->m_IndexList[dwCurrentIndex + 4] = pNode->m_IndexList[dwCurrentIndex + 1];
	//		pNode->m_IndexList[dwCurrentIndex + 5] = (iRow + 1) * m_iNumCols + iCol + 1;
	//		dwCurrentIndex += 6;
	//		iNumFaces += 2;
	//	}
	//}
	//pNode->m_dwFaces = iNumFaces;

	DWORD dwVertexCnt = (dwEndRow - dwStartRow + 1) * (dwEndCol - dwStartCol +1);
	pNode->m_VertexList.resize(dwVertexCnt);
	
	dwVertexCnt = 0;
	for (DWORD iRow = dwStartRow; iRow < dwEndRow+1; iRow++)
	{
		for (DWORD iCol = dwStartCol; iCol < dwEndCol+1; iCol++)
		{
			pNode->m_VertexList[dwVertexCnt++] =
				m_pnctVertexList[iRow * m_iNumCols + iCol];
		}
	}
	pNode->m_VB.Create(m_pd3dDevice, D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER, 
		sizeof(PNCT_VERTEX), 
		pNode->m_VertexList.size(),
		&pNode->m_VertexList.at(0));
	//pNode->m_IB.Create(m_pd3dDevice,
	//	D3D11_USAGE_DEFAULT, 
	//	D3D11_BIND_INDEX_BUFFER, 
	//	sizeof(DWORD), 
	//	pNode->m_IndexList.size(),
	//	&pNode->m_IndexList.at(0));

	return true;
}
void   TLodMap::InitLOD(TNode* pNode, UINT iDepth)
{
	m_LevelList.resize(iDepth);//iDepth=3
	for (int iLevel = 1; iLevel < iDepth; iLevel++)
	{
		m_LevelList[iLevel].resize(pow(4.0f, iLevel));
	}
	m_LevelList[0].push_back(pNode);
}
void   TLodMap::FindNeighborNode(TNode* pNode)
{
	// 최대의 패치 갯수를 계산한다.
	DWORD dwNumPatchCount = pow(2.0f, (float)pNode->m_dwDepth);

	DWORD dwNeighborCol, dwNeighborRow;
	if (pNode->m_dwPositionIndex[1] > 0)  // 상
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0];
		dwNeighborRow = (pNode->m_dwPositionIndex[1] - 1) * dwNumPatchCount;
		pNode->m_pNeighborList[0] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	}
	if (pNode->m_dwPositionIndex[1] < dwNumPatchCount - 1) // 하
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0];
		dwNeighborRow = (pNode->m_dwPositionIndex[1] + 1) * dwNumPatchCount;
		pNode->m_pNeighborList[1] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	}
	if (pNode->m_dwPositionIndex[0] > 0) // 좌
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0] - 1;
		dwNeighborRow = pNode->m_dwPositionIndex[1] * dwNumPatchCount;
		pNode->m_pNeighborList[2] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	}
	if (pNode->m_dwPositionIndex[0] < dwNumPatchCount - 1) // 우
	{
		dwNeighborCol = pNode->m_dwPositionIndex[0] + 1;
		dwNeighborRow = pNode->m_dwPositionIndex[1] * dwNumPatchCount;
		pNode->m_pNeighborList[3] = m_LevelList[pNode->m_dwDepth][dwNeighborRow + dwNeighborCol];
	}
}
void   TLodMap::SetNeighborNode(TNode* pNode)
{
	if (pNode == NULL) return;
	FindNeighborNode(pNode);
	for (int iNode = 0; iNode < 4; iNode++)
	{
		SetNeighborNode(pNode->m_pChild[iNode]);
	}
};
void TLodMap::DrawFindNode(TNode* pNode)
{
	if (pNode == NULL) return;
	T_POSITION pos = m_pCamera->m_Frustum.CheckPositionOBBInPlane(&pNode->m_box);
	// 완전 제외 되었으면
	if (pos == P_BACK)
	{
		return;
	}
	// 리프노드이면서 걸쳐 있으면
	if (pNode->m_bLeafNode && pos != P_BACK)
	{
		m_RenderList.push_back(pNode);
		GetLodSubIndex(pNode);
		return;
	}
	// 리프노드가 아니면서 걸쳐 있으면 spanning
	for (int iChild = 0; iChild < 4; iChild++)
	{
		DrawFindNode(pNode->m_pChild[iChild]);
	}
}
bool TLodMap::Frame()
{
	m_RenderList.clear();
	DrawFindNode(m_pRootNode);
	//m_iNumCollFace = 0;
	//for (int iNode = 0; iNode < m_RenderList.size(); iNode++)
	//{
	//	m_iNumCollFace += UpdateIndexList(
	//		m_iNumCollFace * 3,
	//		m_RenderList[iNode]->m_dwConer[0],
	//		m_RenderList[iNode]->m_dwConer[1],
	//		m_RenderList[iNode]->m_dwConer[2],
	//		m_RenderList[iNode]->m_dwConer[3]);
	//}
	//m_pImmediateContext->UpdateSubresource(m_IB.m_pBuffer,
	//	0, NULL,
	//	&m_IndexListCull.at(0),
	//	//m_pIndexListCull,
	//	m_iNumCollFace * sizeof(DWORD),
	//	0);
	return true;
}
bool TLodMap::Render()
{
	HRESULT hr;
	m_pImmediateContext->PSSetSamplers(0, 1, &TDxState::g_pTexSS);
	m_pImmediateContext->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);

	I_Texture.GetPtr(m_iTexIndex)->Apply(m_pImmediateContext);
	m_pImmediateContext->IASetInputLayout(m_pIL);
	m_pVS->ApplyVS(m_pImmediateContext);
	m_pPS->ApplyPS(m_pImmediateContext);

	//m_VB.Apply(m_pImmediateContext);
	m_CB.Apply(m_pImmediateContext);		
	
	for (int iNode = 0; iNode < m_RenderList.size(); iNode++)
	{		
		GetLodType(m_RenderList[iNode]);
		DWORD dwLod = m_RenderList[iNode]->m_dwLodLevel;
		m_StaticIB[dwLod].Apply(m_pImmediateContext);
		m_RenderList[iNode]->m_VB.Apply(m_pImmediateContext);	
		m_pImmediateContext->DrawIndexed(m_NumIndexStatic[dwLod], 0, 0);
	}
	return true;
}

TLodMap::TLodMap()
{
}


TLodMap::~TLodMap()
{
	m_LodVB.Release();
	for (int iIndex = 0; iIndex < g_iLodLevel; iIndex++)
	{
		m_StaticIB[iIndex].Release();
	}	
}
