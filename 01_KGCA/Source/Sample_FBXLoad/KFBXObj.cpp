#include "KFBXObj.h"

bool KFBXObj::Render(ID3D11DeviceContext* context)
{
	context->IASetInputLayout(m_pVertexLayout.Get());
	UINT pStrides = m_iVertexSize;
	UINT pOffsets = 0;
	//노말맵 추가적인 Binormal Tangent
	context->IASetVertexBuffers(1, 1, m_pVertexBuffer.GetAddressOf(),
		&pStrides, &pOffsets);
	KObject::Render(context);
	return true;
}

bool KFBXObj::PostRender(ID3D11DeviceContext* pContext, UINT iNumIndex)
{
	UINT StartSlot;
	UINT NumBuffers;
	UINT Strides = sizeof(PNCT_VERTEX);
	UINT Offsets = 0;

	for (int iMtrl = 0; iMtrl < m_pSubVertexList.size(); iMtrl++)
	{
		if (m_pTextureList.size() > 0 &&
			m_pTextureList[iMtrl] != nullptr)
		{
			m_pContext->PSSetShaderResources(0, 1,
				m_pTextureList[iMtrl]->m_pSRVTexture.GetAddressOf());
		}

		m_pContext->IASetVertexBuffers(
			0, 1, &m_pVBList[iMtrl],
			&Strides, &Offsets);

		if (m_IndexList.size() <= 0)
			m_pContext->Draw(m_pSubVertexList[iMtrl].size(), 0);
		else
			m_pContext->DrawIndexed(m_IndexList.size(), 0, 0);
	}
	return true;
}

bool KFBXObj::Release()
{
	K3DAsset::Release();
	for (int ivb = 0; ivb < m_pVBList.size(); ivb++)
	{
		if (m_pVBList[ivb] != nullptr)
		{
			m_pVBList[ivb]->Release();
		}
	}
	return true;
}

bool KFBXObj::CreateVertexData()
{
	if (m_pSubVertexList.size() > 0)
	{
		m_pVBList.resize(m_pSubVertexList.size());
	}
	return true;
}

HRESULT KFBXObj::CreateVertexBuffer()
{
	if (m_pSubVertexList.size() <= 0) return false;
	HRESULT hr = S_OK;
	for (int iMtrl = 0; iMtrl < m_pSubVertexList.size(); iMtrl++)
	{
		if (m_pSubVertexList[iMtrl].size() <= 0) continue;
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		bd.ByteWidth = sizeof(PNCT_VERTEX) * m_pSubVertexList[iMtrl].size();
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = &m_pSubVertexList[iMtrl].at(0);

		hr = g_pd3dDevice->CreateBuffer(&bd, &sd, &m_pVBList[iMtrl]);
		if (FAILED(hr))return hr;
	}
	return hr;
}

bool KFBXObj::CreateIndexData()
{
	return true;
}

KFBXObj::KFBXObj()
{
}

KFBXObj::~KFBXObj()
{
}
