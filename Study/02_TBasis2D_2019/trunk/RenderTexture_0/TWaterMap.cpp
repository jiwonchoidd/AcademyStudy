#include "TWaterMap.h"
#include "TShader.h"
#include "TTexture.h"
#include "TDxState.h"

bool        TWaterMap::Load(ID3D11Device* pd3dDevice,
	ID3D11DeviceContext*	pImmediateContext,
	TMapDesc& desc)
{
	HRESULT hr;
	m_pd3dDevice = pd3dDevice;
	m_pImmediateContext = pImmediateContext;

	int   m_iNumCols = desc.width;
	int   m_iNumRows = desc.height;
	int   m_iNumVertex = m_iNumRows * m_iNumCols;
	int   m_iNumSellRows = m_iNumRows - 1;
	int   m_iNumSellCols = m_iNumCols - 1;
	int   m_iNumFace = m_iNumSellRows * m_iNumSellCols * 2;
	int   m_iSellSize = desc.size;

	m_VertexList.resize(m_iNumVertex);
	m_IndexList.resize(m_iNumFace * 3);

	float fOffsetX = 1.0f / (m_iNumCols - 1);
	float fOffsetY = 1.0f / (m_iNumRows - 1);

	int iHalfCols = m_iNumCols / 2;
	int iHalfRows = m_iNumRows / 2;

	int iCurrent = 0;
	for (int iRow = 0; iRow < m_iNumRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumCols; iCol++)
		{
			iCurrent = iRow * m_iNumCols + iCol;
			m_VertexList[iCurrent].p.x = (iCol - iHalfCols)* m_iSellSize;
			m_VertexList[iCurrent].p.z = -(iRow - iHalfRows)* m_iSellSize;
			m_VertexList[iCurrent].p.y = GetHeightOfVertex(iCurrent);
			m_VertexList[iCurrent].c = D3DXVECTOR4(1, 1, 1, 1);
			m_VertexList[iCurrent].t.x = fOffsetX * iCol;
			m_VertexList[iCurrent].t.y = fOffsetY * iRow;
		}
	}
	// 0, 1, 2,
	// 3, 4, 5,
	// 6, 7, 8,

	// 0,1,3  3,1,4   1,2,4,  4,2,5
	// 3,4,6, 6,4,7   4,5,6,  7,5,8
	iCurrent = 0;

	for (int iRow = 0; iRow < m_iNumSellRows; iRow++)
	{
		for (int iCol = 0; iCol < m_iNumSellCols; iCol++)
		{
			m_IndexList[iCurrent + 0] = iRow * m_iNumCols + iCol;
			m_IndexList[iCurrent + 1] = iRow * m_iNumCols + iCol + 1;
			m_IndexList[iCurrent + 2] = (iRow + 1) * m_iNumCols + iCol;

			m_IndexList[iCurrent + 3] = m_IndexList[iCurrent + 2];
			m_IndexList[iCurrent + 4] = m_IndexList[iCurrent + 1];
			m_IndexList[iCurrent + 5] = (iRow + 1) * m_iNumCols + iCol + 1;
			iCurrent += 6;
		}
	}

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

	// 상수 버퍼
	ZeroMemory(&m_WaterCList, sizeof(VSPS_WATER_CBUFFER));
	m_WaterCB.Create(pd3dDevice, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, sizeof(VSPS_WATER_CBUFFER), 1,
		&m_WaterCList);

	// (2)정점 및 픽쉘 쉐이더 컴파일
	m_iVS = I_Shader.Load(pd3dDevice, desc.shadername.c_str(), "WaterVS", "vs_5_0", t_VS);
	m_iPS = I_Shader.Load(pd3dDevice, desc.shadername.c_str(), "WaterPS", "ps_5_0", t_PS);
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


	if (desc.texturename.size() > 0)
	{
		m_iTexIndex = I_Texture.Load(pd3dDevice, desc.texturename.c_str());
	}
	return true;
}
TWaterMap::TWaterMap()
{
	m_iNormalMap = 0;
}


TWaterMap::~TWaterMap()
{
	m_WaterCB.Release();
}
