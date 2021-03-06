#include "KMap.h"
#include "WICTextureLoader.h"
//2의 2승 + 1
//init 함수는 자동으로 헤이트 맵을 받아 맵을 만든다.
bool KMap::Init(ID3D11DeviceContext* context, std::wstring heightmap)
{
	m_pContext = context;
	m_tex_offset = 3.0f;
	CreateHeightMap(heightmap);
	CreateMap(m_num_row, m_num_col, 8.0f);
	//CreateMap(33,33, 2.0f);
	return true;
}
bool KMap::CreateMap(UINT width, UINT height, float distance)
{
	m_cell_distance = distance;
	m_num_row = height;
	m_num_col = width;
	m_num_vertex = m_num_col * m_num_row;
	m_num_cell_col = m_num_col - 1;
	m_num_cell_row = m_num_row - 1;
	m_num_face = m_num_cell_col * m_num_cell_row * 2;

	m_BoxCollision.max.x = (m_num_col / 2 * m_cell_distance);
	m_BoxCollision.min.x = -m_BoxCollision.max.x;
	m_BoxCollision.max.z = (m_num_row / 2 * m_cell_distance);
	m_BoxCollision.min.z = -m_BoxCollision.max.z;
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

	//텍스쳐 크기가 곧 맵의 크기.
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
					//따라서 높이를 조절하려면 나눗셈 DWORD이므로 pitch/4
					m_HeightList[row * desc.Width + col] = (static_cast<float>(byte_height))-30.0f;
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
	m_BTList.resize(m_num_vertex);
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
			m_VertexList[index].pos.x = (iCol - hHalfCol) * m_cell_distance; //

			if (bHasHeightMap)
			{
				m_VertexList[index].pos.y = m_HeightList[index];
			}
			else
			{
				m_VertexList[index].pos.y = 0;
			}
			m_VertexList[index].pos.z = -((iRow - hHalfRow)* m_cell_distance);//
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
		m_BTList[m_IndexList[triangle]].tangent = T;
		m_BTList[m_IndexList[triangle]].binormal= B;
		m_VertexList[m_IndexList[triangle]].normal	= N;

		K3DAsset::CreateTangentSpace(&m_VertexList[m_IndexList[triangle+1]].pos, &m_VertexList[m_IndexList[triangle+2]].pos, &m_VertexList[m_IndexList[triangle]].pos,
			&m_VertexList[m_IndexList[triangle+1]].tex, &m_VertexList[m_IndexList[triangle+2]].tex, &m_VertexList[m_IndexList[triangle]].tex, &N, &T, &B);
		m_BTList[m_IndexList[triangle+1]].tangent = T;
		m_BTList[m_IndexList[triangle+1]].binormal = B;
		m_VertexList[m_IndexList[triangle+1]].normal   = N;

		K3DAsset::CreateTangentSpace(&m_VertexList[m_IndexList[triangle+2]].pos, &m_VertexList[m_IndexList[triangle]].pos, &m_VertexList[m_IndexList[triangle+1]].pos,
			&m_VertexList[m_IndexList[triangle+2]].tex, &m_VertexList[m_IndexList[triangle]].tex, &m_VertexList[m_IndexList[triangle+1]].tex, &N, &T, &B);
		m_BTList[m_IndexList[triangle+2]].tangent = T;
		m_BTList[m_IndexList[triangle+2]].binormal = B;
		m_VertexList[m_IndexList[triangle+2]].normal   = N;
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
	m_cell_distance = 1.0f;
}

bool KMap::Render(ID3D11DeviceContext* context)
{
	KObject::Render(context);
	return true;
}

float KMap::GetHeight(float xpos, float ypos)
{
	// fPosX/fPosZ의 위치에 해당하는 높이맵셀을 찾는다.
		// m_iNumCols와m_iNumRows은 가로/세로의 실제 크기값임.
	float fcell_x = (float)(m_num_cell_row * m_cell_distance/ 2.0f + xpos);
	float fcell_z = (float)(m_num_cell_col * m_cell_distance / 2.0f - ypos);

	// 셀의 크기로 나누어 0~1 단위의 값으로 바꾸어 높이맵 배열에 접근한다.
	fcell_x /= (float)m_cell_distance;
	fcell_z /= (float)m_cell_distance;

	// fCellX, fCellZ 값보다 작거나 같은 최대 정수( 소수부분을 잘라낸다.)
	float fVertexCol = ::floorf(fcell_x);
	float fVertexRow = ::floorf(fcell_z);

	// 높이맵 범위를 벗어나면 강제로 초기화 한다.
	if (fVertexCol < 0.0f)  fVertexCol = 0.0f;
	if (fVertexRow < 0.0f)  fVertexRow = 0.0f;
	if ((float)(m_num_col - 2) < fVertexCol)	fVertexCol = (float)(m_num_col - 2);
	if ((float)(m_num_row - 2) < fVertexRow)	fVertexRow = (float)(m_num_row - 2);

	// 계산된 셀의 플랜을 구성하는 4개 정점의 높이값을 찾는다. 
	//  A   B
	//  *---*
	//  | / |
	//  *---*  
	//  C   D
	float A = GetHeightMap((int)fVertexRow, (int)fVertexCol);
	float B = GetHeightMap((int)fVertexRow, (int)fVertexCol + 1);
	float C = GetHeightMap((int)fVertexRow + 1, (int)fVertexCol);
	float D = GetHeightMap((int)fVertexRow + 1, (int)fVertexCol + 1);

	// A정점의 위치에서 떨어진 값(변위값)을 계산한다. 0 ~ 1.0f
	float fDeltaX = fcell_x - fVertexCol;
	float fDeltaZ = fcell_z - fVertexRow;
	// 보간작업를 위한 기준 페잇스를 찾는다. 
	float fHeight = 0.0f;
	// 윗페이스를 기준으로 보간한다.
	// fDeltaZ + fDeltaX < 1.0f
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
float KMap::Lerp(float start, float end, float tangent)
{
	return start - (start * tangent) + (end * tangent);
}

float KMap::GetHeightMap(int row, int col)
{
	return m_VertexList[row * m_num_row + col].pos.y;
}

KMap::~KMap()
{
}


