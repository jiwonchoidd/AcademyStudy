#include "TRenderTexture.h"


void TViewPort::Set(FLOAT fTopLeftX,
	FLOAT fTopLeftY,
	FLOAT fWidth,
	FLOAT fHeight,
	FLOAT fMinDepth,
	FLOAT fMaxDepth)
{
	m_VP.TopLeftX = fTopLeftX;
	m_VP.TopLeftY = fTopLeftY;
	m_VP.Width = fWidth;
	m_VP.Height = fHeight;
	m_VP.MinDepth = fMinDepth;
	m_VP.MaxDepth = fMaxDepth;
}
void TViewPort::Set(D3D11_VIEWPORT vp)
{
	m_VP = vp;
}
void TViewPort::Apply(ID3D11DeviceContext* pContext)
{
	pContext->RSSetViewports(1, &m_VP);
}
bool TViewRT::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UINT iWidth,
	UINT iHeight)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = iWidth;
	desc.Height= iHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	if (FAILED(hr = pDevice->CreateTexture2D(&desc, NULL, &m_pTexture)))
	{
		return false;
	}
	if (FAILED(hr = pDevice->CreateShaderResourceView( m_pTexture, NULL, &m_pSRV)))
	{
		return false;
	}
	if (FAILED(hr = pDevice->CreateRenderTargetView(m_pTexture, NULL, &m_pRTV)))
	{
		return false;
	}
	float clearColor[4] = {0,0,0,1};
	pContext->ClearRenderTargetView(m_pRTV, clearColor);
	return true;

};
bool TViewRT::Release()
{
	if(m_pTexture) m_pTexture->Release();
	if (m_pSRV) m_pSRV->Release();
	if (m_pRTV) m_pRTV->Release();
	m_pTexture = NULL;
	m_pSRV = NULL;
	m_pRTV = NULL;
	return true;
}
TViewRT::TViewRT()
{
	m_pTexture = NULL;
	m_pSRV = NULL;
	m_pRTV = NULL;
}
TViewRT::~TViewRT()
{
	if (m_pTexture) m_pTexture->Release();
	if (m_pSRV) m_pSRV->Release();
	if (m_pRTV) m_pRTV->Release();
	m_pTexture = NULL;
	m_pSRV = NULL;
	m_pRTV = NULL;
}

bool TViewDS::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext,
	UINT iWidth,
	UINT iHeight)
{
	HRESULT hr;
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = iWidth;
	desc.Height = iHeight;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	if (FAILED(hr = pDevice->CreateTexture2D(&desc, NULL, &m_pTexture)))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	switch (desc.Format)
	{
		case DXGI_FORMAT_R24G8_TYPELESS:		
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
		{
			dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		}break;
		case DXGI_FORMAT_R32_TYPELESS:
		{
			dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
			srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}break;
	}
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	if (FAILED(hr = pDevice->CreateDepthStencilView(m_pTexture, &dsvDesc, &m_pDSV)))
	{
		return false;
	}
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	if (FAILED(hr = pDevice->CreateShaderResourceView(m_pTexture, &srvDesc, &m_pSRV)))
	{
		return false;
	}
	return true;

};
bool TViewDS::Release()
{
	if (m_pTexture) m_pTexture->Release();
	if (m_pSRV) m_pSRV->Release();
	if (m_pDSV) m_pDSV->Release();
	m_pTexture = 0;
	m_pSRV = 0;
	m_pDSV = 0;
	return true;
}
TViewDS::TViewDS()
{
	m_pTexture = NULL;
	m_pSRV = NULL;
	m_pDSV = NULL;
}
TViewDS::~TViewDS()
{
	if(m_pTexture) m_pTexture->Release();
	if (m_pSRV) m_pSRV->Release();
	if (m_pDSV) m_pDSV->Release();
	m_pTexture = 0;
	m_pSRV = 0;
	m_pDSV = 0;
}
bool    TRenderTexture::Begin(ID3D11DeviceContext* pContext, D3DXVECTOR4 clearColor)
{	
	// 저장
	pContext->RSGetViewports(&m_iRT, &m_OldVP);	
	pContext->OMGetRenderTargets(1, &m_pOldRTV, &m_pOldDSV);

	pContext->OMSetRenderTargets(1, &m_RT.m_pRTV, m_DS.m_pDSV);
	pContext->RSSetViewports(1, &m_VP.m_VP);
	pContext->ClearDepthStencilView(m_DS.m_pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	pContext->ClearRenderTargetView(m_RT.m_pRTV, (float*)clearColor);
	return true;
}
bool    TRenderTexture::End(ID3D11DeviceContext* pContext)
{
	// 복원
	ID3D11RenderTargetView* pNull = NULL;
	pContext->OMSetRenderTargets(0, &pNull, NULL);

	pContext->OMSetRenderTargets(1, &m_pOldRTV, m_pOldDSV);
	pContext->RSSetViewports(m_iRT, &m_OldVP);

	ID3D11ShaderResourceView* pSRVNull[1] = { NULL };
	pContext->PSSetShaderResources(0, 1, pSRVNull);	

	if (m_pOldRTV)m_pOldRTV->Release();
	if (m_pOldDSV)m_pOldDSV->Release();
	return true;
}
bool    TRenderTexture::Create(	ID3D11Device* pDevice, 
								ID3D11DeviceContext* pContext,
								UINT iWidth, UINT iHeight)
{
	m_RT.Create(pDevice, pContext, iWidth, iHeight);
	m_DS.Create(pDevice, pContext, iWidth, iHeight);
	m_VP.Set(0, 0, iWidth, iHeight, 0.0f, 1.0f);
	return true;
}
bool    TRenderTexture::Release()
{
	m_RT.Release();
	m_DS.Release();
	m_VP.Release();
	return true;
}
TRenderTexture::TRenderTexture()
{
}


TRenderTexture::~TRenderTexture()
{
}
