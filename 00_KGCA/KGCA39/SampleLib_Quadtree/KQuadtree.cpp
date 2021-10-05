#include "KQuadtree.h"
//노드를 받아와서 인덱스를 업데이트한다. 
bool KQuadtree::UpdateIndexList(KNode* pNode)
{
	int iNumCols = m_pMap->m_info.m_iNumCol;
	int iStartRow = pNode->m_CornerList[0] / iNumCols;
	int iEndRow = pNode->m_CornerList[2] / iNumCols;
	int iStartCol = pNode->m_CornerList[0] % iNumCols;
	int iEndCol = pNode->m_CornerList[1] % iNumCols;

	int iNumColCell = iEndCol - iStartCol;
	int iNumRowCell = iEndRow - iStartRow;
	pNode->m_IndexList.resize(iNumColCell * iNumRowCell * 2 * 3);

	int iIndex = 0;
	for (int iRow = iStartRow; iRow < iEndRow; iRow++)
	{
		for (int iCol = iStartCol; iCol < iEndCol; iCol++)
		{
			int iCurrentIndex = iRow * iNumCols + iCol;
			int iNextRow = (iRow + 1) * iNumCols + iCol;
			pNode->m_IndexList[iIndex + 0] = iCurrentIndex;
			pNode->m_IndexList[iIndex + 1] = iCurrentIndex + 1;
			pNode->m_IndexList[iIndex + 2] = iNextRow;

			pNode->m_IndexList[iIndex + 3] = pNode->m_IndexList[iIndex + 2];
			pNode->m_IndexList[iIndex + 4] = pNode->m_IndexList[iIndex + 1];
			pNode->m_IndexList[iIndex + 5] = iNextRow + 1;
			iIndex += 6;
		}
	}
	if (pNode->m_IndexList.size() > 0) return true;
	return false;
}
HRESULT KQuadtree::CreateIndexBuffer(KNode* pNode)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * pNode->m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &pNode->m_IndexList.at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, &pNode->m_pIndexBuffer);
	if (FAILED(hr)) return hr;
	return hr;
}
bool	KQuadtree::Render(ID3D11DeviceContext* pContext)
{
	for (int iNode = 0; iNode < 1/*m_pLeafList.size()*/; iNode++)
	{
		m_pMap->PreRender(pContext);
		pContext->IASetIndexBuffer(m_pLeafList[iNode]->m_pIndexBuffer,
			DXGI_FORMAT_R32_UINT, 0);
		m_pMap->PostRender(pContext, m_pLeafList[iNode]->m_IndexList.size());
	}
	return true;
}
bool	KQuadtree::Frame()
{
	return true;
}
void    KQuadtree::Build(MapObj* pMap)
{
	m_pMap = pMap;
	m_iNumCol = pMap->m_info.m_iNumCol;
	m_iNumRow = pMap->m_info.m_iNumRow;
	m_pRooKNode = CreateNode(nullptr, 0, m_iNumCol - 1,
		(m_iNumRow - 1) * m_iNumCol, m_iNumRow * m_iNumCol - 1);
	Buildtree(m_pRooKNode);
}
bool    KQuadtree::Init()
{

	return true;
}
bool  KQuadtree::SubDivide(KNode* pNode)
{
	if (pNode->m_iDepth < 3 &&
		(pNode->m_CornerList[1] - pNode->m_CornerList[0]) > 2)
	{
		return true;
	}
	return false;
}
//트리 빌드 
void KQuadtree::Buildtree(KNode* pNode)
{
	if (SubDivide(pNode))
	{

		int center = (pNode->m_CornerList[3] + pNode->m_CornerList[0]) / 2;
		int mt = (pNode->m_CornerList[0] + pNode->m_CornerList[1]) / 2;
		int ml = (pNode->m_CornerList[2] + pNode->m_CornerList[0]) / 2;
		int mr = (pNode->m_CornerList[3] + pNode->m_CornerList[1]) / 2;
		int mb = (pNode->m_CornerList[2] + pNode->m_CornerList[3]) / 2;

		pNode->m_pChild[0] = CreateNode(pNode, pNode->m_CornerList[0],
			mt,
			ml,
			center);
		Buildtree(pNode->m_pChild[0]);

		pNode->m_pChild[1] = CreateNode(pNode, mt,
			pNode->m_CornerList[1],
			center,
			mr);
		Buildtree(pNode->m_pChild[1]);

		pNode->m_pChild[2] = CreateNode(pNode, ml,
			center,
			pNode->m_CornerList[2],
			mb);
		Buildtree(pNode->m_pChild[2]);

		pNode->m_pChild[3] = CreateNode(pNode, center,
			mr,
			mb,
			pNode->m_CornerList[3]);
		Buildtree(pNode->m_pChild[3]);
	}
	else
	{
		pNode->m_bLeaf = true;
		if (UpdateIndexList(pNode))
		{
			CreateIndexBuffer(pNode);
		}
		m_pLeafList.push_back(pNode);
	}
}
KNode* KQuadtree::FindNode(KNode* pNode, KVector2 pos)
{
	do {
		for (int iNode = 0; iNode < 4; iNode++)
		{
			if (pNode->m_pChild[iNode] != nullptr &&
				pNode->m_pChild[iNode]->IsRect(pos))
			{
				m_Queue.push(pNode->m_pChild[iNode]);
				break;
			}
		}
		if (m_Queue.empty())break;
		pNode = m_Queue.front();
		m_Queue.pop();
	} while (pNode);
	return pNode;
}
bool    KQuadtree::AddObject(KVector2 pos)
{
	KNode* pFindNode = FindNode(m_pRooKNode, pos);
	if (pFindNode != nullptr)
	{
		pFindNode->AddObject(pos);
		return true;
	}
	return false;
}
KNode* KQuadtree::FindPlayerNode(KVector2 pos)
{
	KNode* pFindNode = FindNode(m_pRooKNode, pos);
	if (pFindNode != nullptr)
	{
		return pFindNode;
	}
	return nullptr;
}
bool KQuadtree::Release()
{
	delete m_pRooKNode;
	m_pRooKNode = nullptr;
	return true;
}
KNode* KQuadtree::CreateNode(KNode* pParent, float x, float y, float w, float h)
{
	KNode* pNode = new KNode(x, y, w, h);
	if (pParent != nullptr)
	{
		pNode->m_iDepth = pParent->m_iDepth + 1;
		pNode->m_pParent = pParent;
	}
	pNode->m_iIndex = KNode::g_iNewCounter;
	KNode::g_iNewCounter++;
	return pNode;
}
KQuadtree::KQuadtree()
{
	m_pRooKNode = nullptr;
}

KQuadtree::~KQuadtree()
{
}