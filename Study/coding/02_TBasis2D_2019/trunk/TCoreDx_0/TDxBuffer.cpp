#include "TDxBuffer.h"

UINT ByteWidth;
D3D11_USAGE Usage;
UINT BindFlags;
UINT CPUAccessFlags;
UINT MiscFlags;
UINT StructureByteStride;
HRESULT    TDxBuffer::Create(ID3D11Device* pDevice,
	D3D11_USAGE Usage, 
	UINT BindFlags, 
	UINT iSize, UINT iCounter, //ByteWidth = iSize* iConter, 
	void *pSysMem)
{
	HRESULT hr;
	m_BindFlags  = BindFlags;
	m_iSize = iSize;
	// (1)정점버퍼 생성
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = Usage;
	bd.BindFlags = BindFlags;// 용도
	bd.ByteWidth = iSize* iCounter;
	if (Usage == D3D11_USAGE_DYNAMIC)
	{
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	if (pSysMem != NULL)
	{
		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
		sd.pSysMem = pSysMem;
		hr = pDevice->CreateBuffer(&bd, &sd, &m_pBuffer);
	}
	else
	{
		hr = pDevice->CreateBuffer(&bd, NULL, &m_pBuffer);
	}
	return hr;
}
void	   TDxBuffer::Apply(ID3D11DeviceContext* pContext, 
	int iOffset, int iShaderBind)
{
	if (m_BindFlags == D3D11_BIND_VERTEX_BUFFER)
	{
		UINT stride = m_iSize;
		UINT offset = iOffset;
		pContext->IASetVertexBuffers(0, m_iArrayCount, &m_pBuffer, &stride, &offset);
	}
	if (m_BindFlags == D3D11_BIND_INDEX_BUFFER)
	{
		pContext->IASetIndexBuffer(m_pBuffer, DXGI_FORMAT_R32_UINT, iOffset);
	}
	if (m_BindFlags == D3D11_BIND_CONSTANT_BUFFER)
	{
		if(iShaderBind == 0 )
			pContext->VSSetConstantBuffers(iOffset, m_iArrayCount, &m_pBuffer);
		if (iShaderBind == 1)
			pContext->PSSetConstantBuffers(iOffset, m_iArrayCount, &m_pBuffer);
	}
}
bool TDxBuffer::Update(ID3D11DeviceContext* pContext, void* pData, UINT iSize)
{
	HRESULT hr;
	//GPU메모리 접근
	D3D11_MAPPED_SUBRESOURCE mapResource;
	if (SUCCEEDED(hr = pContext->Map(m_pBuffer, 
		0, 
		D3D11_MAP_WRITE_DISCARD, 0,
		&mapResource)))
	{
		//VS_CONSTANT_BUFFER* pBuffer = (VS_CONSTANT_BUFFER*)mapResource.pData;
		memcpy( mapResource.pData, pData, iSize);
		pContext->Unmap(m_pBuffer, 0);
	}
	return true;
}
bool TDxBuffer::Release()
{
	SAFE_RELEASE(m_pBuffer);
	return true;
}
TDxBuffer::TDxBuffer()
{
	m_BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_iSize = 0;
	m_pBuffer = NULL;
	m_iArrayCount = 1;
}


TDxBuffer::~TDxBuffer()
{
}
