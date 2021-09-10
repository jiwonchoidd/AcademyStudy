#include "THeightMap.h"
#include "TShader.h"
#include "TTexture.h"
#include "TDxState.h"
#include "TSys.h"

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

bool        THeightMap::LoadAdd(ID3D11Device* pd3dDevice,
	ID3D11DeviceContext*	pImmediateContext,
	const TCHAR* pVSShaderName,
	const TCHAR* pPShaderName)
{
	// (2)정점 및 픽쉘 쉐이더 컴파일
	m_iReflectVS = I_Shader.Load(pd3dDevice, pVSShaderName, "VS_Reflect", "vs_5_0", t_VS);
	m_iReflectPS = I_Shader.Load(pd3dDevice, pPShaderName, "PS_Reflect", "ps_5_0", t_PS);
	m_pReflectVS = I_Shader.GetPtr(m_iReflectVS);
	m_pReflectPS = I_Shader.GetPtr(m_iReflectPS);

	m_iRefractVS = I_Shader.Load(pd3dDevice, pVSShaderName, "VS_Refract", "vs_5_0", t_VS);
	m_iRefractPS = I_Shader.Load(pd3dDevice, pPShaderName, "PS_Refract", "ps_5_0", t_PS);
	m_pRefractVS = I_Shader.GetPtr(m_iRefractVS);
	m_pRefractPS = I_Shader.GetPtr(m_iRefractPS);

	// 상수 버퍼
	ZeroMemory(&m_WaterCList, sizeof(VSPS_WATER_CBUFFER));
	m_WaterCB.Create(pd3dDevice, D3D11_USAGE_DYNAMIC,
		D3D11_BIND_CONSTANT_BUFFER, sizeof(VSPS_WATER_CBUFFER), 1,
		&m_WaterCList);
	return true;
};
float		THeightMap::GetHeightOfVertex(UINT iIndex)
{
	return m_fVertexList[iIndex];
}
bool		THeightMap::RenderRefract()
{
	HRESULT hr;
	m_WaterCList.x = 0.0f;
	m_WaterCList.y = -1.0f;
	m_WaterCList.z = 0.0f;
	m_WaterCList.time = g_fAccumulation;
	m_WaterCB.Update(m_pImmediateContext,
		(void*)&m_WaterCList,
		sizeof(VSPS_WATER_CBUFFER));

	m_pImmediateContext->PSSetSamplers(0, 1, &TDxState::g_pTexSS);
	m_pImmediateContext->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);

	I_Texture.GetPtr(m_iTexIndex)->Apply(m_pImmediateContext);
	m_pImmediateContext->IASetInputLayout(m_pIL);
	m_pRefractVS->ApplyVS(m_pImmediateContext);
	m_pRefractPS->ApplyPS(m_pImmediateContext);

	m_VB.Apply(m_pImmediateContext);
	m_IB.Apply(m_pImmediateContext);
	m_CB.Apply(m_pImmediateContext);
	m_WaterCB.Apply(m_pImmediateContext, 1, 0);

	m_pImmediateContext->DrawIndexed(m_IndexList.size(), 0, 0);
	return true;
};
bool		THeightMap::RenderReflect()
{
	HRESULT hr;
	m_WaterCList.x = 0.0f;
	m_WaterCList.y = 1.0f;
	m_WaterCList.z = 0.0f;
	m_WaterCList.time = g_fAccumulation;
	m_WaterCB.Update(m_pImmediateContext,
		(void*)&m_WaterCList,
		sizeof(VSPS_WATER_CBUFFER));

	m_pImmediateContext->PSSetSamplers(0, 1, &TDxState::g_pTexSS);
	m_pImmediateContext->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);

	I_Texture.GetPtr(m_iTexIndex)->Apply(m_pImmediateContext);
	m_pImmediateContext->IASetInputLayout(m_pIL);
	m_pReflectVS->ApplyVS(m_pImmediateContext);
	m_pReflectPS->ApplyPS(m_pImmediateContext);

	m_VB.Apply(m_pImmediateContext);
	m_IB.Apply(m_pImmediateContext);
	m_CB.Apply(m_pImmediateContext);
	m_WaterCB.Apply(m_pImmediateContext, 1, 0);

	m_pImmediateContext->DrawIndexed(m_IndexList.size(), 0, 0);

	return true;
};
THeightMap::THeightMap()
{
}


THeightMap::~THeightMap()
{
	m_fVertexList.clear();
	m_WaterCB.Release();
}
