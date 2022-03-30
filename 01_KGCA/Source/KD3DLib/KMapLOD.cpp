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
bool KMapLOD::Build(KMap* pmap, KCamera* pCamera)
{
	//쿼드트리 크기 지정, 쿼드트리 생성
	if (LoadLODFile(L"../../data/script/StaticLod.txt"))
	{
		m_pMap = pmap;
		m_width = pmap->m_num_row;
		m_height = pmap->m_num_col;
		m_pCamera = pCamera;
		
		m_pRootNode = CreateNode(nullptr, 0, m_pMap->m_num_col - 1,
			(m_pMap->m_num_row - 1) * m_pMap->m_num_col, m_pMap->m_num_row * m_pMap->m_num_col - 1);
		Buildtree(m_pRootNode);
		SetNeighborNode();
		//129/4 =5
		m_iNumCell = (int)((m_pMap->m_num_col - 1) / pow(2.0f, (float)m_maxDepth));
		// LOD 레벨 개수( z = pow( x,y )에서 y = log(z) / log(x) ) 
		m_iPatchLodCount = (int)((log((float)m_iNumCell) / log(2.0f)));
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
	pNode->SetRect(vLT.x, (vRB.y- vLT.y), vLT.z, vRT.x - vLT.x, vLT.z - vLB.z);
	//리프 노드의 바운딩 박스를 만든다.
	pNode->SetBox(vLT.x, vLT.y, vLT.z, vRT.x - vLT.x, vLT.z - vLB.z);
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

//패치 인덱스
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
//전체 노드의 LOD레벨을 저장
bool KMapLOD::SetLOD(KVector3* vCamera)
{
	for (int iNode = 0; iNode < m_pLeafList.size(); iNode++)
	{
		int iLodLevel = 0;
		float fDistance = (m_pLeafList[iNode]->m_Center - *vCamera).Length();
		//가장 가까울수록 최상단의 LOD 높을수록 복잡한 버텍스
		if (fDistance < 65.0f)
		{
			m_pLeafList.at(iNode)->m_LodLevel = 2;
		}
		//두번째 LOD 중간 버텍스
		else if (fDistance < 130.0f)
		{
			m_pLeafList.at(iNode)->m_LodLevel = 1;
		}
		//제일 낮은 LOD 값
		else
			m_pLeafList.at(iNode)->m_LodLevel = 0;
	}
	return true;
}
DWORD KMapLOD::SetLODType(KNode* pNode)
{
	int iRenderCode = 0;
	// 동서남북
	if (pNode->m_pNeighborlist[0] &&
		pNode->m_LodLevel < pNode->m_pNeighborlist[0]->m_LodLevel)
	{
		iRenderCode += 2;
	}
	if (pNode->m_pNeighborlist[1] &&
		pNode->m_LodLevel < pNode->m_pNeighborlist[1]->m_LodLevel)
	{
		iRenderCode += 8;
	}
	if (pNode->m_pNeighborlist[2] &&
		pNode->m_LodLevel < pNode->m_pNeighborlist[2]->m_LodLevel)
	{
		iRenderCode += 4;
	}
	if (pNode->m_pNeighborlist[3] &&
		pNode->m_LodLevel < pNode->m_pNeighborlist[3]->m_LodLevel)
	{
		iRenderCode += 1;
	}
	pNode->m_LodType = iRenderCode;
	return iRenderCode;
}
//보여지는 리프 노드 갱신

bool KMapLOD::Render(ID3D11DeviceContext* pContext)
{
	DrawableUpdate();

	//전체 리프노드 LOD 레벨을 저장
	SetLOD(m_pCamera->GetCameraPos());

	//프로스텀에 있는 보이는 리프노드만 타입을 정해줌

	for (int iNode = 0; iNode < m_pDrawableLeafList.size(); iNode++)
	{
		KNode* pNode = m_pDrawableLeafList[iNode];

		SetLODType(pNode);

		m_pMap->PreRender(pContext);
		pContext->IASetInputLayout(m_pMap->m_pVertexLayout.Get());
		UINT pOffsets = 0;
		UINT pStrides = sizeof(PNCT_VERTEX);

		//노말맵을 위한 버텍스 정보
		pContext->IASetVertexBuffers(1, 1, m_pMap->m_pVertexBuffer.GetAddressOf(),
			&pStrides, &pOffsets);
		//----------------------------------------

		//노드 당 갖고 있는 버텍스 정보
		pContext->IASetVertexBuffers(0, 1, pNode->m_pVertexBuffer.GetAddressOf(), &pStrides, &pOffsets);
		//----------------------------------------
		
		//패치 인덱스
		if (pNode->m_LodLevel < 2)
		{
			pContext->IASetIndexBuffer(m_LodPatchList[pNode->m_LodLevel].IndexBufferList[pNode->m_LodType].Get(), DXGI_FORMAT_R32_UINT, 0);
			m_pMap->PostRender(pContext, (UINT)m_LodPatchList[pNode->m_LodLevel].IndexList[pNode->m_LodType].size());
		}
		else
		{
			pContext->IASetIndexBuffer(m_pLodIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			m_pMap->PostRender(pContext, (UINT)m_IndexList.size());
		}

		//----------------------------------------
	}

	//맵 오브젝트 렌더
	/*for (auto obj : m_ObjectList)
	{
		obj->obj_pObject->SetMatrix(&obj->obj_matWorld,
			&m_pMap->m_cbData.matView,
			&m_pMap->m_cbData.matProj);
		obj->obj_pObject->Render(pContext);
	}*/

	//todo:디버깅 박스, 플래그 설정
	if(ImGui::Begin(u8"지형 렌더링"))
	{
		ImGui::Text(u8"감지된 리프노드 %d", m_pDrawableLeafList.size());
		if (ImGui::Button("Terrian Box Enable"))
		{
			m_bDebug = !m_bDebug;
		}
	}ImGui::End();
	if (m_bDebug)
	{
		for (int iNode = 0; iNode < m_pDrawableLeafList.size(); iNode++)
		{
			DrawDebugRender(&m_pDrawableLeafList[iNode]->m_node_box, pContext);
		}
	}
	return true;
}


void KMapLOD::DrawableUpdate()
{
	m_pDrawableLeafList.clear();
	m_ObjectList.clear();
	RenderTile(m_pRootNode);
}
void KMapLOD::RenderTile(KNode* pNode)
{
	if (pNode == nullptr) return;
	if (m_pCamera->ClassifyOBB(&pNode->m_node_box) == TRUE)
	{
		for (auto obj : pNode->m_StaticObjectList)
		{
			if (m_pCamera->ClassifyOBB(&obj->obj_box) == TRUE)
			{
				m_ObjectList.push_back(obj);
			}
		}
		if (pNode->m_bLeaf == true)
		{
			m_pDrawableLeafList.push_back(pNode);
			return;
		}

		for (int iNode = 0; iNode < pNode->m_pChildlist.size(); iNode++)
		{
			RenderTile(pNode->m_pChildlist[iNode]);
		}
	}
}
bool KMapLOD::AddObject(KMapObject* obj)
{
	//노드 위치를 찾고 오브젝트를 추가한다.
	KNode* pFindNode = FindNode(m_pRootNode, obj->obj_box);
	if (pFindNode != nullptr)
	{
		pFindNode->AddObject(obj);
		return true;
	}
	return false;
}

bool KMapLOD::AddDynamicObject(KMapObject* obj)
{
	KNode* pFindNode =
		FindNode(m_pRootNode, obj->obj_box);
	if (pFindNode != nullptr)
	{
		//obj->m_iNodeIndex = pFindNode->m_iIndex;
		pFindNode->AddDynamicObject(obj);
		return true;
	}
	return false;
}

/// <summary>
/// Temporary Debuging Object
/// </summary>
void KMapLOD::DrawDebugInit(ID3D11DeviceContext* pContext)
{
	if (m_Debug_Box.Init(L"../../data/shader/VSPS_Frustum.hlsl", L"../../data/shader/VSPS_Frustum.hlsl"))
	{
		//debug object created
		m_Debug_Box.m_pContext = pContext;
	}
}
void KMapLOD::DrawDebugRender(KBox* pBox, ID3D11DeviceContext* pContext)
{
	//add list
	
	/// <summary> 모든 정육면체 이렇게 통일
	///		6		7
	/// 2		3
	///  
	///		4		5
	/// 0		1
	/// </summary> 0 1 2 3 -> 5 4 7 6

	pBox->List[0] = KVector3(pBox->min.x,
		pBox->min.y,
		pBox->max.z);
	pBox->List[1] = KVector3(pBox->max.x,
		pBox->min.y,
		pBox->max.z);
	pBox->List[2] = KVector3(pBox->min.x,
		pBox->max.y,
		pBox->max.z);
	pBox->List[3] = KVector3(pBox->max.x,
		pBox->max.y,
		pBox->max.z);

	pBox->List[4] = KVector3(pBox->min.x,
		pBox->min.y,
		pBox->min.z);
	pBox->List[5] = KVector3(pBox->max.x,
		pBox->min.y,
		pBox->min.z);
	pBox->List[6] = KVector3(pBox->min.x,
		pBox->max.y,
		pBox->min.z);
	pBox->List[7] = KVector3(pBox->max.x,
		pBox->max.y,
		pBox->min.z);

	// 정면
	m_Debug_Box.m_VertexList[0].pos = pBox->List[0];
	m_Debug_Box.m_VertexList[1].pos = pBox->List[1];
	m_Debug_Box.m_VertexList[2].pos = pBox->List[2];
	m_Debug_Box.m_VertexList[3].pos = pBox->List[3];
	// 뒷면
	m_Debug_Box.m_VertexList[4].pos = pBox->List[5];
	m_Debug_Box.m_VertexList[5].pos = pBox->List[4];
	m_Debug_Box.m_VertexList[6].pos = pBox->List[7];
	m_Debug_Box.m_VertexList[7].pos = pBox->List[6];
	// 오른쪽면
	m_Debug_Box.m_VertexList[8].pos = pBox->List[1];
	m_Debug_Box.m_VertexList[9].pos = pBox->List[5];
	m_Debug_Box.m_VertexList[10].pos = pBox->List[3];
	m_Debug_Box.m_VertexList[11].pos = pBox->List[7];
	// 왼쪽면
	m_Debug_Box.m_VertexList[12].pos = pBox->List[4];
	m_Debug_Box.m_VertexList[13].pos = pBox->List[0];
	m_Debug_Box.m_VertexList[14].pos = pBox->List[6];
	m_Debug_Box.m_VertexList[15].pos = pBox->List[2];
	// 윗쪽면
	m_Debug_Box.m_VertexList[16].pos = pBox->List[2];
	m_Debug_Box.m_VertexList[17].pos = pBox->List[3];
	m_Debug_Box.m_VertexList[18].pos = pBox->List[6];
	m_Debug_Box.m_VertexList[19].pos = pBox->List[7];
	// 아랫면
	m_Debug_Box.m_VertexList[20].pos = pBox->List[1];
	m_Debug_Box.m_VertexList[21].pos = pBox->List[0];
	m_Debug_Box.m_VertexList[22].pos = pBox->List[5];
	m_Debug_Box.m_VertexList[23].pos = pBox->List[4]; 

	for (int index = 0; index < m_Debug_Box.m_VertexList.size(); index++)
	{
		m_Debug_Box.m_VertexList[index].color = KVector4(pBox->max.y * 0.015f, 0.2f, 0.2f, 1.0f);
	}
	m_Debug_Box.SetMatrix(NULL, &m_pCamera->m_matView, &m_pCamera->m_matProj);
	pContext->UpdateSubresource(
		m_Debug_Box.m_pVertexBuffer.Get(), 0, NULL, &m_Debug_Box.m_VertexList.at(0), 0, 0);
	m_Debug_Box.Render(pContext);
}

bool KMapLOD::Release()
{
	for (int iPatch = 0; iPatch < m_LodPatchList.size(); iPatch++)
	{
		m_LodPatchList[iPatch].Release();
	}
	KQuadTree::Release();
	m_Debug_Box.Release();
	return true;
}
KMapLOD::KMapLOD()
{
}

KMapLOD::~KMapLOD()
{
}
