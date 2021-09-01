#include "TMap.h"
#include "TShader.h"
#include "TTexture.h"
#include "TDxState.h"
float TMap::Lerp(float fStart, float fEnd, float fTangent)
{
	return fStart - (fStart*fTangent) + (fEnd*fTangent);
}
float TMap::GetHeightmap(int row, int col)
{
	return m_pnctVertexList[row * m_iNumRows + col].p.y;
}
float TMap::GetHeight( float fPosX, float fPosZ)
{
	// fPosX/fPosZ의 위치에 해당하는 높이맵셀을 찾는다.
	// m_iNumCols와m_iNumRows은 가로/세로의 실제 크기값임.
	float fCellX = ((float)(m_iNumCols - 1) / 2.0f)*m_iSellSize + fPosX;
	float fCellZ = ((float)(m_iNumRows - 1) / 2.0f)*m_iSellSize - fPosZ;

	// 셀의 크기로 나누어 1단위의 값으로 바꾸어 높이맵 배열에 접근한다.
	fCellX /= (float)m_iSellSize;
	fCellZ /= (float)m_iSellSize;

	// fCellX, fCellZ 값보다 작거나 같은 최대 정수( 소수부분을 잘라낸다.)
	float fVertexCol = ::floorf(fCellX);
	float fVertexRow = ::floorf(fCellZ);

	// 높이맵 범위를 벗어나면 강제로 초기화 한다.
	if (fVertexCol < 0.0f)  fVertexCol = 0.0f;
	if (fVertexRow < 0.0f)  fVertexRow = 0.0f;
	if ((float)(m_iNumCols - 2)< fVertexCol)	fVertexCol = (float)(m_iNumCols - 2);
	if ((float)(m_iNumRows - 2)< fVertexRow)	fVertexRow = (float)(m_iNumRows - 2);

	// 계산된 셀의 플랜을 구성하는 4개 정점의 높이값을 찾는다. 
	//  A   B
	//  *---*
	//  | / |
	//  *---*  
	//  C   D
	float A = GetHeightmap((int)fVertexRow, (int)fVertexCol);
	float B = GetHeightmap((int)fVertexRow, (int)fVertexCol + 1);
	float C = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol);
	float D = GetHeightmap((int)fVertexRow + 1, (int)fVertexCol + 1);

	// A정점의 위치에서 떨어진 값(변위값)을 계산한다.
	float fDeltaX = fCellX - fVertexCol;
	float fDeltaZ = fCellZ - fVertexRow;
	// 보간작업를 위한 기준 페잇스를 찾는다. 
	float fHeight = 0.0f;
	// 윗페이스를 기준으로 보간한다.
	if (fDeltaZ < (1.0f - fDeltaX))  //ABC
	{
		float uy = B - A; // A->B
		float vy = C - A; // A->C	
						  // 두 정점의 높이값의 차이를 비교하여 델타X의 값에 따라 보간값을 찾는다.		
		fHeight = A + Lerp(0.0f, uy, fDeltaX) + Lerp(0.0f, vy, fDeltaZ);
	}
	// 아래페이스를 기준으로 보간한다.
	else // DCB
	{
		float uy = C - D; // D->C
		float vy = B - D; // D->B
						  // 두 정점의 높이값의 차이를 비교하여 델타Z의 값에 따라 보간값을 찾는다.		
		fHeight = D + Lerp(0.0f, uy, 1.0f - fDeltaX) + Lerp(0.0f, vy, 1.0f - fDeltaZ);
	}
	return fHeight;
}
bool TMap::CreateHeightMap(ID3D11Device* pd3dDevice,
	ID3D11DeviceContext*	pImmediateContext,
	T_STR name)
{
	HRESULT hr;
	ID3D11Resource* pResource = NULL;
	D3DX11_IMAGE_INFO imageinfo;
	D3DX11_IMAGE_LOAD_INFO info;
	ZeroMemory(&info, sizeof(D3DX11_IMAGE_LOAD_INFO));
	info.Usage = D3D11_USAGE_STAGING;
	info.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	info.Format = DXGI_FORMAT_FROM_FILE;
	info.pSrcInfo = &imageinfo;

	if (FAILED(hr = D3DX11CreateTextureFromFile(
		pd3dDevice,
		name.c_str(), &info, NULL, &pResource, NULL)))
	{
		return false;
	}
	ID3D11Texture2D* pTex2D = NULL;
	if (FAILED(hr = pResource->QueryInterface(__uuidof(ID3D11Texture2D),
		(LPVOID*)&pTex2D)))
	{
		return false;
	}
	SAFE_RELEASE(pResource);

	// 텍스쳐 정보 얻기
	pTex2D->GetDesc(&m_Desc);
	m_fVertexList.resize(m_Desc.Width*m_Desc.Height);

	D3D11_MAPPED_SUBRESOURCE mapdest;
	if (SUCCEEDED(hr = pImmediateContext->Map(pTex2D, 0,
		D3D11_MAP_READ, 0, &mapdest)))
	{
		UCHAR* pTexels = (UCHAR*)mapdest.pData;
		for (int iRow = 0; iRow < m_Desc.Height; iRow++)
		{
			UINT rowStart = iRow * mapdest.RowPitch;
			for (int iCol = 0; iCol <m_Desc.Width; iCol++)
			{
				UINT colStart = iCol * 4;
				m_fVertexList[iRow*m_Desc.Width + iCol] = (pTexels[rowStart + colStart + 0] / 25.0f);
			}
		}
		pImmediateContext->Unmap(pTex2D, 0);
	}
	SAFE_RELEASE(pTex2D);
	return true;
}
float		TMap::GetHeightOfVertex(UINT iIndex)
{
	if (m_fVertexList.size() <= 0) return 0.0f;
	return m_fVertexList[iIndex];
}
bool        TMap::Load(ID3D11Device* pd3dDevice,
	ID3D11DeviceContext*	pImmediateContext,
	TCamera* pCamera,
	TMapDesc& desc)
{
	HRESULT hr;
	m_pd3dDevice = pd3dDevice;
	m_pImmediateContext = pImmediateContext;
	m_pCamera = pCamera;

	m_iNumCols = desc.width;
	m_iNumRows = desc.height;
	m_iNumVertex = m_iNumRows * m_iNumCols;
	m_iNumSellRows = m_iNumRows-1;
	m_iNumSellCols = m_iNumCols-1;
	m_iNumFace = m_iNumSellRows * m_iNumSellCols * 2;
	m_iNumCollFace = m_iNumFace;
	m_iSellSize = desc.size;

	m_pnctVertexList.resize(m_iNumVertex);
	m_IndexList.resize(m_iNumFace * 3);
	m_IndexListCull.resize(m_iNumFace * 3);

	float fOffsetX = 1.0f / (m_iNumCols - 1);
	float fOffsetY = 1.0f / (m_iNumRows - 1);

	int iHalfCols = m_iNumCols / 2;
	int iHalfRows = m_iNumRows / 2;

	int iCurrent = 0;
	m_Box.vMax = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	m_Box.vMin = D3DXVECTOR3( 10000.0f, 10000.0f, 10000.0f);
	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			iCurrent = iRow * m_iNumCols + iCol;
			m_pnctVertexList[iCurrent].p.x = (iCol-iHalfCols)* m_iSellSize;
			m_pnctVertexList[iCurrent].p.z = -(iRow-iHalfRows)* m_iSellSize;
			m_pnctVertexList[iCurrent].p.y = GetHeightOfVertex(iCurrent);
			m_pnctVertexList[iCurrent].c = D3DXVECTOR4(1, 1, 1, 1);
			m_pnctVertexList[iCurrent].n = D3DXVECTOR3(0, 0, 0);
			m_pnctVertexList[iCurrent].t.x = fOffsetX * iCol;
			m_pnctVertexList[iCurrent].t.y = fOffsetY * iRow;
			//BoundBox
			if (m_Box.vMax.x < m_pnctVertexList[iCurrent].p.x)
				m_Box.vMax.x = m_pnctVertexList[iCurrent].p.x;
			if (m_Box.vMax.y < m_pnctVertexList[iCurrent].p.y)
				m_Box.vMax.y = m_pnctVertexList[iCurrent].p.y;
			if (m_Box.vMax.z < m_pnctVertexList[iCurrent].p.z)
				m_Box.vMax.z = m_pnctVertexList[iCurrent].p.z;
			if (m_Box.vMin.x > m_pnctVertexList[iCurrent].p.x)
				m_Box.vMin.x = m_pnctVertexList[iCurrent].p.x;
			if (m_Box.vMin.y > m_pnctVertexList[iCurrent].p.y)
				m_Box.vMin.y = m_pnctVertexList[iCurrent].p.y;
			if (m_Box.vMin.z > m_pnctVertexList[iCurrent].p.z)
				m_Box.vMin.z = m_pnctVertexList[iCurrent].p.z;
		}
	}
	m_Sphere.vCenter = (m_Box.vMax + m_Box.vMin ) * 0.5f;
	m_Sphere.fRadius = D3DXVec3Length(&(m_Box.vMax - m_Box.vMin)) * 0.5f;

	// 0, 1, 2,
	// 3, 4, 5,
	// 6, 7, 8,

	// 0,1,3  3,1,4   1,2,4,  4,2,5
	// 3,4,6, 6,4,7   4,5,6,  7,5,8
	iCurrent = 0;
	D3DXVECTOR3 vFaceNormal;
	for (int iRow = 0; iRow < m_iNumSellRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumSellCols; iCol++)
		{
			m_IndexList[iCurrent + 0] = iRow * m_iNumCols + iCol;
			m_IndexList[iCurrent + 1] = iRow * m_iNumCols + iCol + 1;
			m_IndexList[iCurrent + 2] = (iRow+1) * m_iNumCols + iCol;
			vFaceNormal = ComputeFaceNormal(m_IndexList[iCurrent + 0],
				m_IndexList[iCurrent + 1],
				m_IndexList[iCurrent + 2]);
			m_pnctVertexList[m_IndexList[iCurrent + 0]].n += vFaceNormal;
			m_pnctVertexList[m_IndexList[iCurrent + 1]].n += vFaceNormal;
			m_pnctVertexList[m_IndexList[iCurrent + 2]].n += vFaceNormal;

			m_IndexList[iCurrent + 3] = m_IndexList[iCurrent + 2];
			m_IndexList[iCurrent + 4] = m_IndexList[iCurrent + 1];
			m_IndexList[iCurrent + 5] = (iRow + 1) * m_iNumCols + iCol + 1;

			vFaceNormal = ComputeFaceNormal(m_IndexList[iCurrent + 3],
				m_IndexList[iCurrent + 4],
				m_IndexList[iCurrent + 5]);
			m_pnctVertexList[m_IndexList[iCurrent + 3]].n += vFaceNormal;
			m_pnctVertexList[m_IndexList[iCurrent + 4]].n += vFaceNormal;
			m_pnctVertexList[m_IndexList[iCurrent + 5]].n += vFaceNormal;
			iCurrent += 6;
		}
	}
	// 합산된 정점 노말의 정규화 
	D3DXVECTOR3 vLight = { 100.0f, 100.0f, -100.0f};
	D3DXVec3Normalize(&vLight, &vLight);
	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			iCurrent = iRow * m_iNumCols + iCol;
			D3DXVec3Normalize(	&m_pnctVertexList[iCurrent].n,
								&m_pnctVertexList[iCurrent].n);
			float fDot = max(D3DXVec3Dot(&m_pnctVertexList[iCurrent].n,
				                      &vLight), 0.3f);
			m_pnctVertexList[iCurrent].c = D3DXVECTOR4(
				fDot, fDot, fDot, 1.0f);
		}
	}
	/*m_IndexList.push_back(0);
	m_IndexList.push_back(m_iNumCols-1);
	m_IndexList.push_back(m_iNumSellRows * m_iNumCols );

	m_IndexList.push_back(m_iNumSellRows * m_iNumCols);
	m_IndexList.push_back(m_iNumCols - 1);
	m_IndexList.push_back(m_iNumSellRows * m_iNumCols + m_iNumCols - 1);
	*/// (1)정점버퍼 생성
	m_VB.Create(pd3dDevice, D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER, sizeof(PNCT_VERTEX),
		m_pnctVertexList.size(),
		&m_pnctVertexList.at(0));
	m_IB.Create(pd3dDevice, D3D11_USAGE_DEFAULT,
		D3D11_BIND_INDEX_BUFFER, sizeof(DWORD),
		m_IndexList.size(),
		&m_IndexList.at(0));

	// 상수 버퍼
	m_CB.Create(pd3dDevice, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_VS_PER_OBJECT), 1,
		NULL);

	// (2)정점 및 픽쉘 쉐이더 컴파일
	m_iVS = I_Shader.Load(pd3dDevice, desc.shadername.c_str(), "VS", "vs_5_0", t_VS);
	m_iPS = I_Shader.Load(pd3dDevice, desc.shadername.c_str(), "PS", "ps_5_0", t_PS);
	m_pVS = I_Shader.GetPtr(m_iVS);
	m_pPS = I_Shader.GetPtr(m_iPS);

	// (3) 정점 레이아웃( 정점성분의 구조도 )
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	int iNumElement = sizeof(ied) / sizeof(ied[0]);
	hr = pd3dDevice->CreateInputLayout(ied, iNumElement,
		m_pVS->m_pBlob->GetBufferPointer(),
		m_pVS->m_pBlob->GetBufferSize(),
		&m_pIL);
	if (FAILED(hr))
	{
		return false;
	}


	if (desc.texturename.size() > 0 )
	{
		m_iTexIndex = I_Texture.Load(pd3dDevice, desc.texturename.c_str());
	}
	return true;
}
D3DXVECTOR3 TMap::ComputeFaceNormal(DWORD i0, DWORD i1, DWORD i2)
{
	D3DXVECTOR3 vFaceNormal;
	D3DXVECTOR3 v0 = m_pnctVertexList[i1].p - m_pnctVertexList[i0].p;
	D3DXVECTOR3 v1 = m_pnctVertexList[i2].p - m_pnctVertexList[i0].p;
	D3DXVec3Cross(&vFaceNormal, &v0, &v1);
	//D3DXVec3Normalize(&vFaceNormal, &vFaceNormal));
	return vFaceNormal;
}
bool TMap::CreateVertexBuffer(
	D3DXVECTOR3 vMan, D3DXVECTOR3 vMin)
{
	m_VertexList.resize(24);
	m_IndexList.resize(36);
	// FRONT
	m_VertexList[0].p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	m_VertexList[1].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	m_VertexList[2].p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	m_VertexList[3].p = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	m_VertexList[0].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[1].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[2].c = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[3].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[0].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[1].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[2].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VertexList[3].t = D3DXVECTOR2(1.0f, 1.0f);
	m_IndexList[0] = 0; m_IndexList[1] = 1; m_IndexList[2] = 2;
	m_IndexList[3] = 2; m_IndexList[4] = 1; m_IndexList[5] = 3;
	// LEFT
	m_VertexList[4].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	m_VertexList[5].p = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_VertexList[6].p = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	m_VertexList[7].p = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	m_VertexList[4].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[5].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[6].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[7].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[4].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[5].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[6].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VertexList[7].t = D3DXVECTOR2(1.0f, 1.0f);
	m_IndexList[6] = 4; m_IndexList[7] = 5; m_IndexList[8] = 6;
	m_IndexList[9] = 6; m_IndexList[10] = 5; m_IndexList[11] = 7;
	// BACK
	m_VertexList[8].p = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_VertexList[9].p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_VertexList[10].p = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	m_VertexList[11].p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	m_VertexList[8].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[9].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[10].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[11].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[8].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[9].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[10].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VertexList[11].t = D3DXVECTOR2(1.0f, 1.0f);
	m_IndexList[12] = 8; m_IndexList[13] = 9; m_IndexList[14] = 10;
	m_IndexList[15] = 10; m_IndexList[16] = 9; m_IndexList[17] = 11;
	// RIGHT
	m_VertexList[12].p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_VertexList[13].p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	m_VertexList[14].p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	m_VertexList[15].p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	m_VertexList[12].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[13].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[14].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[15].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[12].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[13].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[14].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VertexList[15].t = D3DXVECTOR2(1.0f, 1.0f);
	m_IndexList[18] = 12; m_IndexList[19] = 13; m_IndexList[20] = 14;
	m_IndexList[21] = 14; m_IndexList[22] = 13; m_IndexList[23] = 15;
	// UP
	m_VertexList[16].p = D3DXVECTOR3(-1.0f, 1.0f, 1.0f);
	m_VertexList[17].p = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_VertexList[18].p = D3DXVECTOR3(-1.0f, 1.0f, -1.0f);
	m_VertexList[19].p = D3DXVECTOR3(1.0f, 1.0f, -1.0f);
	m_VertexList[16].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[17].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[18].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[19].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[16].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[17].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[18].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VertexList[19].t = D3DXVECTOR2(1.0f, 1.0f);
	m_IndexList[24] = 16; m_IndexList[25] = 17; m_IndexList[26] = 18;
	m_IndexList[27] = 18; m_IndexList[28] = 17; m_IndexList[29] = 19;
	// DOWN
	m_VertexList[20].p = D3DXVECTOR3(1.0f, -1.0f, 1.0f);
	m_VertexList[21].p = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
	m_VertexList[22].p = D3DXVECTOR3(1.0f, -1.0f, -1.0f);
	m_VertexList[23].p = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
	m_VertexList[20].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);
	m_VertexList[21].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[22].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);
	m_VertexList[23].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	m_VertexList[20].t = D3DXVECTOR2(0.0f, 0.0f);
	m_VertexList[21].t = D3DXVECTOR2(1.0f, 0.0f);
	m_VertexList[22].t = D3DXVECTOR2(0.0f, 1.0f);
	m_VertexList[23].t = D3DXVECTOR2(1.0f, 1.0f);
	m_IndexList[30] = 20; m_IndexList[31] = 21; m_IndexList[32] = 22;
	m_IndexList[33] = 22; m_IndexList[34] = 21; m_IndexList[35] = 23;
	return true;
}
bool TMap::Create(ID3D11Device* pd3dDevice,
	ID3D11DeviceContext*	pImmediateContext,
	const TCHAR* pTextureName,
	const TCHAR* pVShaderName,
	const TCHAR* pPShaderName)
{
	HRESULT hr;
	m_pd3dDevice = pd3dDevice;
	m_pImmediateContext = pImmediateContext;

	CreateVertexBuffer(D3DXVECTOR3(-1.0f, -1.0f, -1.0f),
		D3DXVECTOR3(1.0f, 1.0f, 1.0f));

	// (1)정점버퍼 생성
	m_VB.Create(pd3dDevice, D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER, sizeof(PCT_VERTEX),
		m_VertexList.size(),
		&m_VertexList.at(0));
	m_IB.Create(pd3dDevice, D3D11_USAGE_DEFAULT,
		D3D11_BIND_INDEX_BUFFER, sizeof(DWORD),
		m_IndexList.size(),
		&m_IndexList.at(0));

	// 상수 버퍼
	m_CB.Create(pd3dDevice, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, sizeof(CB_VS_PER_OBJECT), 1,
		NULL);

	// (2)정점 및 픽쉘 쉐이더 컴파일
	m_iVS = I_Shader.Load(pd3dDevice, pVShaderName, "VS", "vs_5_0", t_VS);
	m_iPS = I_Shader.Load(pd3dDevice, pPShaderName, "PS", "ps_5_0", t_PS);
	m_pVS = I_Shader.GetPtr(m_iVS);
	m_pPS = I_Shader.GetPtr(m_iPS);

	// (3) 정점 레이아웃( 정점성분의 구조도 )
	D3D11_INPUT_ELEMENT_DESC ied[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	int iNumElement = sizeof(ied) / sizeof(ied[0]);
	hr = pd3dDevice->CreateInputLayout(ied, iNumElement,
		m_pVS->m_pBlob->GetBufferPointer(),
		m_pVS->m_pBlob->GetBufferSize(),
		&m_pIL);
	if (FAILED(hr))
	{
		return false;
	}


	if (pTextureName != NULL)
	{
		m_iTexIndex = I_Texture.Load(pd3dDevice, pTextureName);
	}
	return true;
}
bool TMap::Render()
{
	HRESULT hr;
	m_pImmediateContext->PSSetSamplers(0, 1, &TDxState::g_pTexSS);
	m_pImmediateContext->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);

	I_Texture.GetPtr(m_iTexIndex)->Apply(m_pImmediateContext);	
	m_pImmediateContext->IASetInputLayout(m_pIL);
	m_pVS->ApplyVS(m_pImmediateContext);
	m_pPS->ApplyPS(m_pImmediateContext);
	
	m_VB.Apply(m_pImmediateContext);
	m_IB.Apply(m_pImmediateContext);
	m_CB.Apply(m_pImmediateContext);

	m_pImmediateContext->DrawIndexed(m_iNumCollFace *3, 0, 0);
	return true;
}
bool	TMap::Release()
{
	SAFE_DELETE_ARRAY(m_pIndexListCull);
	SAFE_RELEASE(m_pIL);
	m_VB.Release();
	m_IB.Release();
	m_CB.Release();
	return true;
}
bool    TMap::Frame()
{
	//m_IndexListCull.clear();
	if (m_pCamera)
	{	
		int iIndex = 0;
		for (int iFace = 0; iFace < m_IndexList.size() / 3; iFace++)
		{
			DWORD i0 = m_IndexList[iFace * 3 + 0];
			DWORD i1 = m_IndexList[iFace * 3 + 1];
			DWORD i2 = m_IndexList[iFace * 3 + 2];

			for (int iVertex = 0; iVertex < 3; iVertex++)
			{
				D3DXVECTOR3 v = m_VertexList[m_IndexList[iFace * 3 + iVertex]].p;
				if (m_pCamera->m_Frustum.ClassifyPoint(v))
				{
					// 인덱스 버퍼에 추가
					m_IndexListCull[iIndex++] = i0;
					m_IndexListCull[iIndex++] = i1;
					m_IndexListCull[iIndex++] = i2;
		
					break;
				}
			}
		}
		m_iNumFace = iIndex / 3;
		m_pImmediateContext->UpdateSubresource(m_IB.m_pBuffer,
			0, NULL,
			&m_IndexListCull.at(0),
			//m_pIndexListCull,
			m_iNumFace * sizeof(DWORD),			
			0);
	}
	return true;
}
TMap::TMap()
{
}


TMap::~TMap()
{
}
