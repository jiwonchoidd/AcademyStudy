#include "KMap.h"
#include "WICTextureLoader.h"
//2의 2승 + 1
//init 함수는 자동으로 헤이트 맵을 받아 맵을 만든다.
bool KMap::Init(ID3D11DeviceContext* context, std::wstring heightmap)
{
	m_pContext = context;
	m_tex_offset = 4.0f;
	CreateHeightMap(heightmap);
	CreateMap(m_num_row, m_num_col, 1.0f);
	//CreateMap(512,512, 2.0f);
	return true;
}
bool KMap::CreateMap(UINT width, UINT height, float distance)
{
	m_cell_distance = distance;
	m_num_row = width;
	m_num_col = height;
	m_num_vertex = m_num_col * m_num_row;
	m_num_cell_col = m_num_col - 1;
	m_num_cell_row = m_num_row - 1;
	m_num_face = m_num_cell_col * m_num_cell_row * 2;
	return true;
}
//context가 로드되어야 실행됨
bool KMap::CreateHeightMap(std::wstring heightmap)
{
	if (heightmap.empty()&&m_pContext!=nullptr)
	{
		return false;
	}
	HRESULT hr;
	ID3D11ShaderResourceView* pSRV = nullptr;
	wrl::ComPtr<ID3D11Resource> pTexture;
	size_t maxsize = 0;
	if (FAILED(hr = CreateWICTextureFromFileEx(g_pd3dDevice,
		heightmap.c_str(),
		maxsize, 
		D3D11_USAGE_STAGING,
		NULL,
		D3D11_CPU_ACCESS_WRITE|D3D11_CPU_ACCESS_READ,
		NULL,
		WIC_LOADER_DEFAULT,
		pTexture.GetAddressOf(), nullptr)))
	{
		return false;
	}
	ID3D11Texture2D* pTexture2D = NULL;
	if (FAILED(pTexture->QueryInterface(__uuidof(ID3D11Texture2D), (LPVOID*)&pTexture2D)))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	pTexture2D->GetDesc(&desc);

	m_HeightList.resize(desc.Height * desc.Width);

	if (pTexture2D)
	{
		D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
		//크리티칼 섹션처럼 unmap 하기전까지 접근 못함
		if (SUCCEEDED(m_pContext->Map((ID3D11Resource*)pTexture2D, D3D11CalcSubresource(0, 0, 1), D3D11_MAP_READ, 0, &MappedFaceDest)))
		{
			UCHAR* pTexels = (UCHAR*)MappedFaceDest.pData;
			PNCT_VERTEX	v;
			for (UINT row = 0; row < desc.Height; row++)
			{
				UINT rowStart = row * MappedFaceDest.RowPitch;
				for (UINT col = 0; col < desc.Width; col++)
				{
					UINT colStart = col * 4;
					UINT byte_height = pTexels[rowStart + colStart + 0];
					//byte에 저장할수있는 최대값은 0~255
					//따라서 높이를 조절하려면 나눗셈
					m_HeightList[row * desc.Width + col] = (static_cast<float>(byte_height)/8.0f)-4.0f;	/// DWORD이므로 pitch/4	
				}
			}
			m_pContext->Unmap(pTexture2D, D3D11CalcSubresource(0, 0, 1)); 
		}
	}

	m_num_row = desc.Height;
	m_num_col = desc.Width;

	if (pTexture2D) pTexture2D->Release();
	return true;
}
bool KMap::CreateVertexData()
{
	bool bHasHeightMap = false;
	m_VertexList.resize(m_num_vertex);

	if (m_HeightList.size() > 0)bHasHeightMap = true;

	float  hHalfCol = (m_num_col - 1) / 2.0f;
	float  hHalfRow = (m_num_row - 1) / 2.0f;
	//오프셋 조절해서 텍스쳐 크기 조절 가능함
	float  offsetU = m_tex_offset / (m_num_col - 1);
	float  offsetV = m_tex_offset / (m_num_row - 1);
	for (int iRow = 0; iRow < m_num_row; iRow++)
	{
		for (int iCol = 0; iCol < m_num_col; iCol++)
		{
			int index = iRow * m_num_col + iCol;
			m_VertexList[index].pos.x = (iCol - hHalfCol) * m_cell_distance;

			if (bHasHeightMap)
			{
				m_VertexList[index].pos.y = m_HeightList[index];
			}
			else
			{
				m_VertexList[index].pos.y = 0;
			}
			m_VertexList[index].pos.z = -((iRow - hHalfRow)* m_cell_distance);
			m_VertexList[index].color = KVector4(1, 1, 1, 1);
			m_VertexList[index].tex =KVector2(offsetU * iCol, offsetV * iRow);
		}
	}


	return true;
}

bool KMap::CreateIndexData()
{
	m_IndexList.resize(m_num_face * 3);
	UINT iIndex = 0;
	for (int iRow = 0; iRow < m_num_cell_row; iRow++)
	{
		for (int iCol = 0; iCol < m_num_cell_col; iCol++)
		{
			m_IndexList[iIndex + 0] = iRow * m_num_col + iCol;
			m_IndexList[iIndex + 1] = (iRow * m_num_col + iCol) + 1;
			m_IndexList[iIndex + 2] = (iRow + 1) * m_num_col + iCol;

			m_IndexList[iIndex + 3] = m_IndexList[iIndex + 2];
			m_IndexList[iIndex + 4] = m_IndexList[iIndex + 1];
			m_IndexList[iIndex + 5] = m_IndexList[iIndex + 2] + 1;

			iIndex += 6;
		}
	}

	for (int triangle = 0; triangle < m_IndexList.size(); triangle += 3)
	{
		KVector3 T, B, N;

		K3DAsset::CreateTangentSpace(&m_VertexList[m_IndexList[triangle]].pos, &m_VertexList[m_IndexList[triangle+1]].pos, &m_VertexList[m_IndexList[triangle + 2]].pos,
			&m_VertexList[m_IndexList[triangle]].tex, &m_VertexList[m_IndexList[triangle+1]].tex, &m_VertexList[m_IndexList[triangle+2]].tex, &N, &T, &B);
		m_VertexList[m_IndexList[triangle]].tangent = T;
		m_VertexList[m_IndexList[triangle]].binormal= B;
		m_VertexList[m_IndexList[triangle]].normal	= N;

		K3DAsset::CreateTangentSpace(&m_VertexList[m_IndexList[triangle+1]].pos, &m_VertexList[m_IndexList[triangle+2]].pos, &m_VertexList[m_IndexList[triangle]].pos,
			&m_VertexList[m_IndexList[triangle+1]].tex, &m_VertexList[m_IndexList[triangle+2]].tex, &m_VertexList[m_IndexList[triangle]].tex, &N, &T, &B);
		m_VertexList[m_IndexList[triangle+1]].tangent  = T;
		m_VertexList[m_IndexList[triangle+1]].binormal = B;
		m_VertexList[m_IndexList[triangle+1]].normal   = N;

		K3DAsset::CreateTangentSpace(&m_VertexList[m_IndexList[triangle+2]].pos, &m_VertexList[m_IndexList[triangle]].pos, &m_VertexList[m_IndexList[triangle+1]].pos,
			&m_VertexList[m_IndexList[triangle+2]].tex, &m_VertexList[m_IndexList[triangle]].tex, &m_VertexList[m_IndexList[triangle+1]].tex, &N, &T, &B);
		m_VertexList[m_IndexList[triangle+2]].tangent  = T;
		m_VertexList[m_IndexList[triangle+2]].binormal = B;
		m_VertexList[m_IndexList[triangle+2]].normal   = N;
		int k = 0;
	}

	return true;
}

KMap::KMap()
{
	m_num_col = 0;
	m_num_row = 0;
	m_num_vertex = 0;
	m_num_cell_col = 0;
	m_num_cell_row = 0;
	m_num_face = 0;
	m_num_face=0;
}

bool KMap::Render(ID3D11DeviceContext* context)
{
	context->IASetInputLayout(m_pVertexLayout.Get());
	UINT pStrides = m_iVertexSize;
	UINT pOffsets = 0;
	context->IASetVertexBuffers(1, 1, m_pVertexBuffer.GetAddressOf(),
		&pStrides, &pOffsets);
	KObject::Render(context);
	return true;
}



KMap::~KMap()
{
}


