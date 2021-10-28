#include "KDepthStencil.h"

bool KDepthStencil::Init(ID3D11DeviceContext* context, ID3D11RenderTargetView* target)
{
	m_context = context;
	m_target = target;
	CreateDepthStenView();
	CreateDepthStenState();
	CreateBlendState();
	return true;
}

bool KDepthStencil::Frame()
{
	return true;
}

bool KDepthStencil::PreRender()
{
	m_context->ClearDepthStencilView(
		m_pDepthStenV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	return true;
}

bool KDepthStencil::Release()
{
	SAFE_RELEASE(m_pDepthStenV);
	SAFE_RELEASE(m_pDepthStenS);
	SAFE_RELEASE(m_pBlendState);
	return true;
}

HRESULT KDepthStencil::CreateDepthStenView()
{
	//rc.right-rc.left,
	//rc.bottom - rc.top,
	HRESULT hr = S_OK;
	//텍스처 생성 : 깊이,스텐실 값을 저장하는 버퍼용 가상의 텍스쳐
	ID3D11Texture2D* pTexture = nullptr;
	D3D11_TEXTURE2D_DESC td;
	td.Width = g_rtClient.right- g_rtClient.left;
	td.Height = g_rtClient.bottom - g_rtClient.top;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	hr = g_pd3dDevice->CreateTexture2D(&td, NULL, &pTexture);
	if (FAILED(hr))
	{
		return hr;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC svd;
	ZeroMemory(&svd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	svd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	svd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	svd.Texture2D.MipSlice = 0;
	hr = g_pd3dDevice->CreateDepthStencilView(pTexture, &svd,
		&m_pDepthStenV);
	if (FAILED(hr))
	{
		return hr;
	}
	//바로 릴리즈
	SAFE_RELEASE(pTexture);

	m_context->OMSetRenderTargets(1,
		&m_target, m_pDepthStenV);
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
	m_context->OMSetDepthStencilState(m_pDepthStenS, 0x01);
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
	m_context->OMSetBlendState(m_pBlendState, BlendFactor, 0xffffff);
	return hr;
}

KDepthStencil::KDepthStencil()
{
}

KDepthStencil::~KDepthStencil()
{
}
