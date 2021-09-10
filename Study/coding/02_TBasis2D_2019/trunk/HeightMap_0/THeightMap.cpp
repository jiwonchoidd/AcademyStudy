#include "THeightMap.h"


bool THeightMap::CreateHeightMap(ID3D11Device* pd3dDevice, 
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
				m_fVertexList[iRow*m_Desc.Width+iCol] = pTexels[rowStart+colStart+0] / 5.0f;
			}
		}
		pImmediateContext->Unmap(pTex2D, 0);
	}
	SAFE_RELEASE(pTex2D);
	return true;
}
float		THeightMap::GetHeightOfVertex(UINT iIndex)
{
	return m_fVertexList[iIndex];
}
THeightMap::THeightMap()
{
}


THeightMap::~THeightMap()
{
	m_fVertexList.clear();
}
