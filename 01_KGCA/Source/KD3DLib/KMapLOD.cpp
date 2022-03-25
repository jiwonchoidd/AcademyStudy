#include "KMapLOD.h"
#include "ImGuiManager.h"
template <typename OutputIterator>
void KMapLOD::Tokenize(const std::wstring& text, const std::wstring& delimiters, OutputIterator first)
{
	size_t start, stop, n = text.length();

	for (start = text.find_first_not_of(delimiters); 0 <= start && start < n;
		start = text.find_first_not_of(delimiters, stop + 1))
	{
		stop = text.find_first_of(delimiters, start);
		if (stop < 0 || stop > n)
		{
			stop = n;
		}
		*first++ = text.substr(start, stop - start);
	}
}
bool  KMapLOD::LoadLODFile(std::wstring filename)
{
	FILE* fp = nullptr;
	_tfopen_s(&fp, filename.c_str(), _T("rt"));
	if (fp == NULL)
	{
		return false;
	}

	TCHAR data[256] = { 0, };

	TCHAR buffer[256] = { 0, };
	int iVersion = 0;
	_fgetts(buffer, 256, fp);
	TCHAR tmp[256] = { 0, };
	_stscanf_s(buffer, _T("%s%d"), tmp, _countof(tmp), &iVersion);

	int iNumPatch = 0;
	_fgetts(buffer, 256, fp);
	_stscanf_s(buffer, _T("%s%d"), tmp, _countof(tmp), &iNumPatch);

	int index = 0;

	for (int iLod = 0; iLod < iNumPatch; iLod++)
	{
		KLodPatch lod;
		lod.iLodLevel = iLod;
		for (int iCode = 0; iCode < 16; iCode++)
		{
			std::vector<std::wstring>	ListTokens;
			_fgetts(buffer, 256, fp);
			_stscanf_s(buffer, _T("%d %s"), &index, data, _countof(data));

			std::wstring sentence = data;
			Tokenize(sentence, L",", std::back_inserter(ListTokens));
			int iMaxCnt = (int)ListTokens.size();
			lod.IndexList[iCode].resize(iMaxCnt);
			for (int i = 0; i < iMaxCnt; i++)
			{
				lod.IndexList[iCode][i] = (DWORD)(_tstoi(ListTokens[i].c_str()));
			}
		}
		m_LodPatchList.push_back(lod);

		_fgetts(buffer, 256, fp);
	}
	for (int iLod = 0; iLod < iNumPatch; iLod++)
	{
		for (int iCode = 0; iCode < 16; iCode++)
		{
			CreateIndexBuffer(m_LodPatchList[iLod], iCode);
		}
	}
	fclose(fp);
	return true;
}
bool KMapLOD::Build(KMap* pmap)
{
	//쿼드트리 크기 지정, 쿼드트리 생성
	if (LoadLODFile(L"../../data/script/StaticLod.txt"))
	{
		m_pMap = pmap;
		m_width = pmap->m_num_row;
		m_height = pmap->m_num_col;
		
		m_pRootNode = CreateNode(nullptr, 0, m_pMap->m_num_col - 1,
			(m_pMap->m_num_row - 1) * m_pMap->m_num_col, m_pMap->m_num_row * m_pMap->m_num_col - 1);
		Buildtree(m_pRootNode);
		SetNeighborNode();
		//129/4 =5
		m_iNumCell = (int)((m_pMap->m_num_col - 1) / pow(2.0f, (float)m_maxDepth));
		// LOD 레벨 개수( z = pow( x,y )에서 y = log(z) / log(x) ) 
		m_iPatchLodCount = (int)((log((float)m_iNumCell) / log(2.0f)));
		SetLOD();
		if (m_iPatchLodCount > 0)
		{
			m_LodPatchList.resize(m_iPatchLodCount);
		}
		if (UpdateIndexList(m_pLeafList[0]))
		{
			CreateIndexBuffer(m_pLeafList[0]);
		}
		return true;
	}

	return false;
}

bool KMapLOD::SetLOD()
{
	return true;
}

bool KMapLOD::ComputeStaticLodIndex(int numcell)
{
	m_LodPatchList.reserve(numcell);
	return true;
}

KNode* KMapLOD::CreateNode(KNode* pParent, float x, float y, float w, float h)
{
	KNode* pNode = new KNode(x, y, w, h);
	if (pParent != nullptr)
	{
		pNode->m_depth = pParent->m_depth + 1;
		pNode->m_pParent = pParent;
	}
	ldiv_t divValue = ldiv(pNode->m_CornerList[0], m_pMap->m_num_col);
	//나머지 x 몫 y 
	pNode->m_Element.x = divValue.rem / (pNode->m_CornerList[1] - pNode->m_CornerList[0]);
	pNode->m_Element.y = divValue.quot / (pNode->m_CornerList[1] - pNode->m_CornerList[0]);

	DWORD dwNumPatchCount = (DWORD)pow(2.0f, (float)pNode->m_depth);
	pNode->m_index = pNode->m_Element.y * dwNumPatchCount + pNode->m_Element.x;

	KVector3 vLT = m_pMap->m_VertexList[pNode->m_CornerList[0]].pos;
	KVector3 vRT = m_pMap->m_VertexList[pNode->m_CornerList[1]].pos;
	KVector3 vLB = m_pMap->m_VertexList[pNode->m_CornerList[2]].pos;
	KVector3 vRB = m_pMap->m_VertexList[pNode->m_CornerList[3]].pos;
	pNode->SetRect(vLT.x, (vLT.y-vRB.y)/2.0f, vLT.z, vRT.x - vLT.x, vLT.z - vLB.z);

	return pNode;
}

bool KMapLOD::UpdateVertexList(KNode* pNode)
{
	int iNumCols = m_pMap->m_num_col;
	int iStartRow = pNode->m_CornerList[0] / iNumCols;
	int iEndRow = pNode->m_CornerList[2] / iNumCols;
	int iStartCol = pNode->m_CornerList[0] % iNumCols;
	int iEndCol = pNode->m_CornerList[1] % iNumCols;

	int iNumColCell = iEndCol - iStartCol;
	int iNumRowCell = iEndRow - iStartRow;
	pNode->m_VertexList.resize((iEndCol - iStartCol + 1) * (iEndRow - iStartRow + 1));

	int iIndex = 0;
	for (int iRow = iStartRow; iRow <= iEndRow; iRow++)
	{
		for (int iCol = iStartCol; iCol <= iEndCol; iCol++)
		{
			int iCurrentIndex = iRow * iNumCols + iCol;
			pNode->m_VertexList[iIndex++] = m_pMap->m_VertexList[iCurrentIndex];
		}
	}
	if (pNode->m_VertexList.size() > 0) return true;
	return false;
}
//노드에 버텍스 버퍼 만들어줌.
HRESULT KMapLOD::CreateVertexBuffer(KNode* pNode)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(PNCT_VERTEX) * pNode->m_VertexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &pNode->m_VertexList.at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, pNode->m_pVertexBuffer.GetAddressOf());
	if (FAILED(hr)) return hr;
	return hr;
}

HRESULT KMapLOD::CreateIndexBuffer(KLodPatch& patch, int iCode)
{
	patch.IndexBufferList[iCode] = nullptr;
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD) * patch.IndexList[iCode].size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &patch.IndexList[iCode].at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, patch.IndexBufferList[iCode].GetAddressOf());
	if (FAILED(hr)) return hr;
	return hr;
}
HRESULT KMapLOD::CreateIndexBuffer(KNode* pNode)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(DWORD)*m_IndexList.size();
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = &m_IndexList.at(0);
	hr = g_pd3dDevice->CreateBuffer(&bd, &data, m_pLodIndexBuffer.GetAddressOf());
	if (FAILED(hr)) return hr;
	return hr;
}
bool KMapLOD::UpdateIndexList(KNode* pNode)
{
	int iNumCols = m_pMap->m_num_col;
	int iStartRow = pNode->m_CornerList[0] / iNumCols;
	int iEndRow = pNode->m_CornerList[2] / iNumCols;
	int iStartCol = pNode->m_CornerList[0] % iNumCols;
	int iEndCol = pNode->m_CornerList[1] % iNumCols;
	int iNumColCell = iEndCol - iStartCol;
	int iNumRowCell = iEndRow - iStartRow;
	m_IndexList.resize(iNumColCell * iNumRowCell * 2 * 3);

	int iIndex = 0;
	for (int iRow = 0; iRow < iNumRowCell; iRow++)
	{
		for (int iCol = 0; iCol < iNumColCell; iCol++)
		{
			int iCurrentIndex = iRow * (iNumColCell + 1) + iCol;
			int iNextRow = (iRow + 1) * (iNumColCell + 1) + iCol;
			m_IndexList[iIndex + 0] = iCurrentIndex;
			m_IndexList[iIndex + 1] = iCurrentIndex + 1;
			m_IndexList[iIndex + 2] = iNextRow;

			m_IndexList[iIndex + 3] = m_IndexList[iIndex + 2];
			m_IndexList[iIndex + 4] = m_IndexList[iIndex + 1];
			m_IndexList[iIndex + 5] = iNextRow + 1;
			iIndex += 6;
		}
	}
	if (m_IndexList.size() > 0) return true;
	return false;
}
bool KMapLOD::Init()
{
	return false;
}

bool KMapLOD::Frame()
{
	return false;
}

bool KMapLOD::Render(ID3D11DeviceContext* pContext, KVector3* vCamera)
{
	for (int iNode = 0; iNode < m_pLeafList.size(); iNode++)
	{
		int iLodLevel = 0;
		float fDistance = (m_pLeafList[iNode]->m_Center - *vCamera).Length();
		//가장 가까울수록 최상단의 LOD 높을수록 복잡한 버텍스
		if (fDistance < 40.0f)
		{
			m_pLeafList.at(iNode)->m_LodLevel = 2;
		}
		//두번째 LOD 중간 버텍스
		else if (fDistance < 80.0f)
		{
			m_pLeafList.at(iNode)->m_LodLevel = 1;
		}
		//제일 낮은 LOD 값
		else
			m_pLeafList.at(iNode)->m_LodLevel = 0;
	}
	for (int iNode = 0; iNode < m_pLeafList.size(); iNode++)
	{
		int iRenderCode = 0;
		// 동서남북
		if (m_pLeafList.at(iNode)->m_pNeighborlist[0] &&
			m_pLeafList.at(iNode)->m_LodLevel < m_pLeafList.at(iNode)->m_pNeighborlist[0]->m_LodLevel)
		{
			iRenderCode += 2;
		}
		if (m_pLeafList.at(iNode)->m_pNeighborlist[1] &&
			m_pLeafList.at(iNode)->m_LodLevel < m_pLeafList.at(iNode)->m_pNeighborlist[1]->m_LodLevel)
		{
			iRenderCode += 8;
		}
		if (m_pLeafList.at(iNode)->m_pNeighborlist[2] &&
			m_pLeafList.at(iNode)->m_LodLevel < m_pLeafList.at(iNode)->m_pNeighborlist[2]->m_LodLevel)
		{
			iRenderCode += 4;
		}
		if (m_pLeafList.at(iNode)->m_pNeighborlist[3] &&
			m_pLeafList.at(iNode)->m_LodLevel < m_pLeafList.at(iNode)->m_pNeighborlist[3]->m_LodLevel)
		{
			iRenderCode += 1;
		}
		UINT iNumIndex = 0;
		ID3D11Buffer* pRenderBuffer = nullptr;
		UINT iLodLevel = m_pLeafList[iNode]->m_LodLevel;
		if (m_pLeafList[iNode]->m_LodLevel == 0)
		{
			iNumIndex = m_LodPatchList[iLodLevel].IndexList[iRenderCode].size();
			pRenderBuffer = m_LodPatchList[iLodLevel].IndexBufferList[iRenderCode].Get();
		}
		else if (m_pLeafList[iNode]->m_LodLevel == 1)
		{
			iNumIndex = m_LodPatchList[iLodLevel].IndexList[iRenderCode].size();
			pRenderBuffer = m_LodPatchList[iLodLevel].IndexBufferList[iRenderCode].Get();
		}
		else if(m_pLeafList[iNode]->m_LodLevel == 2)
		{
			iNumIndex = m_IndexList.size();
			pRenderBuffer = m_pLodIndexBuffer.Get();
		}
		m_pMap->PreRender(pContext);
		pContext->IASetInputLayout(m_pMap->m_pVertexLayout.Get());
		UINT pStrides = m_pMap->m_iVertexSize;
		UINT pOffsets = 0;
		pContext->IASetVertexBuffers(1, 1, m_pMap->m_pVertexBuffer.GetAddressOf(),
			&pStrides, &pOffsets);	
		pContext->IASetVertexBuffers(0, 1, m_pLeafList[iNode]->m_pVertexBuffer.GetAddressOf(), &pStrides, &pOffsets);
		pContext->IASetIndexBuffer(pRenderBuffer, DXGI_FORMAT_R32_UINT, 0);
		m_pMap->PostRender(pContext, iNumIndex);
	}
	return true;
}

bool KMapLOD::Release()
{
	for (int iPatch = 0; iPatch < m_LodPatchList.size(); iPatch++)
	{
		//m_LodPatchList[iPatch].IndexBufferList->Reset();
		m_LodPatchList[iPatch].Release();
	}
	KQuadTree::Release();
	return true;
}

KMapLOD::KMapLOD()
{
}

KMapLOD::~KMapLOD()
{
}
