#include "KDepthStencil.h"

ID3D11Texture2D* KDepthStencil::CreateTexture(UINT Width, UINT Height)
{
	HRESULT hr = S_OK;
	// 1)텍스처 생성 : 깊이,스텐실 값을 저장하는 버퍼용
	ID3D11Texture2D* pDSTexture = nullptr;
	D3D11_TEXTURE2D_DESC DescDepth;
	DescDepth.Width = Width;
	DescDepth.Height = Height;
	DescDepth.MipLevels = 1;
	DescDepth.ArraySize = 1;
	//RGB는 텍스쳐 리소스 D24는 뎁스
	DescDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DescDepth.SampleDesc.Count = 1;
	DescDepth.SampleDesc.Quality = 0;
	DescDepth.Usage = D3D11_USAGE_DEFAULT;

	// 백 버퍼 깊이 및 스텐실 버퍼 생성
	if (DescDepth.Format == DXGI_FORMAT_D24_UNORM_S8_UINT)
		DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	else // 깊이맵 전용 깊이맵 생성
		DescDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	DescDepth.CPUAccessFlags = 0;
	DescDepth.MiscFlags = 0;
	if (FAILED(hr = g_pd3dDevice->CreateTexture2D(&DescDepth, NULL, &pDSTexture)))
	{
		return nullptr;
	}
	return pDSTexture;
}

HRESULT KDepthStencil::CreateDepthStencilView(UINT Width, UINT Height)
{
	HRESULT hr = S_OK;
	// 1)텍스처 생성 : 깊이,스텐실 값을 저장하는 버퍼용
	m_pTexture = CreateTexture(Width, Height);
	if (m_pTexture == nullptr)
	{
		return E_FAIL;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC svd;
	ZeroMemory(&svd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	svd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	svd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	svd.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(m_pTexture, &svd,
		&m_pDepthStenV);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
	D3D11_SHADER_RESOURCE_VIEW_DESC Desc;
	ZeroMemory(&Desc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	Desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	Desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	Desc.Texture2D.MipLevels = 1;
	hr = g_pd3dDevice->CreateShaderResourceView(m_pTexture,
		&Desc, &m_pTextureSRV);
	if (FAILED(hr))
	{
		m_pTexture->Release();
		return hr;
	}
	return hr;
}
HRESULT KDepthStencil::CreateDepthStenState()
{
	HRESULT hr = S_OK;
    D3D11_DEPTH_STENCIL_DESC dsd;
    ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
    dsd.DepthEnable = TRUE;
    dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    //1.0, 0.6과 0.5를 비교해서 크면 실패
    //제일 앞에 있는 면이 뿌려지면 뒤에있는애들은 렌더 안됨
    dsd.DepthFunc = D3D11_COMPARISON_LESS;
    dsd.StencilEnable = TRUE;
    dsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    dsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    dsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    dsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    dsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    hr=g_pd3dDevice->CreateDepthStencilState(&dsd, &m_pDepthStenS);
	if (FAILED(hr))
	{
		return hr;
	}
	//깊이 스텐실 제일 마지막 결과 기반으로 렌더하는 것이기에 OM

	return hr;
}

HRESULT KDepthStencil::CreateBlendState()
{
	HRESULT hr = S_OK;
    D3D11_BLEND_DESC bd;
    ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
    bd.AlphaToCoverageEnable = false;
    bd.IndependentBlendEnable = false;
    bd.RenderTarget[0].BlendEnable = false;
    bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    hr=g_pd3dDevice->CreateBlendState(&bd, &m_pBlendState);
	if (FAILED(hr))
	{
		return hr;
	}
    // 블렌드 스테이트 오브젝트
    float BlendFactor[] = { 0.0f,0.0f,0.0f,1.0f };
	//m_context->OMSetBlendState(m_pBlendState, BlendFactor, 0xffffff);
	return hr;
}

bool KDepthStencil::Release()
{
	SAFE_RELEASE(m_pTexture);
	SAFE_RELEASE(m_pTextureSRV);
	SAFE_RELEASE(m_pDepthStenV);
	SAFE_RELEASE(m_pDepthStenS);
	return true;
}

KDepthStencil::KDepthStencil()
{
}

KDepthStencil::~KDepthStencil()
{
}
