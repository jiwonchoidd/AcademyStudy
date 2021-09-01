#include "TDevice.h"
#include "TSys.h"
#include "TDxState.h"
//http://seanmiddleditch.com/direct3d-11-debug-api-tricks/
HRESULT		TDevice::CreateDevice()
{
	HRESULT hr = S_OK;	
	UINT   createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_UNKNOWN, D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,	D3D_DRIVER_TYPE_REFERENCE,
	};	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);

	D3D_FEATURE_LEVEL pFeatureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,	D3D_FEATURE_LEVEL_10_0,
	};	UINT numFeatureTypes = sizeof(pFeatureLevels) / sizeof(pFeatureLevels[0]);
	
	IDXGIAdapter* pAdapter;
	m_pGIFactory->EnumAdapters(0, &pAdapter);

	D3D_FEATURE_LEVEL retFeatureLevel;

	for (int i = 0; i < numDriverTypes; i++)
	{
		m_DriverType = driverTypes[i];
		if (SUCCEEDED(hr = D3D11CreateDevice(pAdapter, m_DriverType, NULL, createDeviceFlags, pFeatureLevels, numFeatureTypes, D3D11_SDK_VERSION, &m_pd3dDevice, &retFeatureLevel, &m_pImmediateContext)))
		{
			if (retFeatureLevel < D3D_FEATURE_LEVEL_11_0)
			{
				if (m_pImmediateContext) m_pImmediateContext->Release();
				if (m_pd3dDevice) m_pd3dDevice->Release();
				continue;
			}
			break;
		}
	}


	hr = m_pd3dDevice->QueryInterface(IID_PPV_ARGS(&m_pDebug));
	return hr;
};
HRESULT     TDevice::CreateGIFactory()
{
	HRESULT hr;
	hr = CreateDXGIFactory( __uuidof(IDXGIFactory),
		(void**)(&m_pGIFactory));
	
	return hr;
}
HRESULT		TDevice::CreateSwapChain() 
{
	HRESULT hr = S_OK;
	//if (m_pd3dDevice == NULL) E_FAIL;
	//if (m_pGIFactory == NULL) E_FAIL;	
	_ASSERT(m_pd3dDevice != NULL);
	_ASSERT(m_pGIFactory != NULL);
	
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	ZeroMemory(&SwapChainDesc, sizeof(SwapChainDesc));
	SwapChainDesc.BufferCount = 1;
	SwapChainDesc.OutputWindow = m_hWnd;
	SwapChainDesc.Windowed = TRUE;
	SwapChainDesc.BufferDesc.Width = m_rtClient.right;
	SwapChainDesc.BufferDesc.Height = m_rtClient.bottom;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// 오프스크린 버퍼의 사용 용도
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hr = m_pGIFactory->CreateSwapChain(m_pd3dDevice,
			&SwapChainDesc, &m_pSwapChain);
	
	//hr = m_pSwapChain->SetFullscreenState(FALSE, NULL);
	return hr;
};
HRESULT		TDevice::SetRenderTargetView() 
{
	HRESULT hr = S_OK;
	// 생성된 스왑체인의 정보를 조회
	hr = m_pSwapChain->GetDesc(&m_SwapChainDesc);

	// 랜더타켓 생성한다.
	ID3D11Texture2D *pResource;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pResource);
	hr = m_pd3dDevice->CreateRenderTargetView(pResource, NULL, &m_pRTV);
	pResource->Release();
	if (SUCCEEDED(hr))
	{
		m_pImmediateContext->OMSetRenderTargets(1, &m_pRTV,
			m_pDSV);
	}	
	return hr;
};
HRESULT		TDevice::SetViewPort() 
{
	HRESULT hr = S_OK;	
	// 뷰포트 설정
	m_ViewPort.TopLeftX = 0; // 가로 시작 지점
	m_ViewPort.TopLeftY = 0; // 세로 시작 지점
	m_ViewPort.Width = m_SwapChainDesc.BufferDesc.Width;
	m_ViewPort.Height = m_SwapChainDesc.BufferDesc.Height;
	m_ViewPort.MinDepth = 0;
	m_ViewPort.MaxDepth = 1;

	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
	return hr;
};
bool     TDevice::Reset(UINT iWidth, UINT iHeight)
{
	if (m_pd3dDevice == NULL) return true;
	HRESULT hr = S_OK;	
	m_pImmediateContext->OMSetRenderTargets(0, NULL, NULL);
	if (m_pDSV != NULL) m_pDSV->Release();
	if (m_pRTV != NULL) m_pRTV->Release();

	if (FAILED(hr = m_pSwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount,
				iWidth, iHeight, m_SwapChainDesc.BufferDesc.Format,
				m_SwapChainDesc.Flags)))
	{
		return false;
	}
	CreateDepthStencilState();
	if (FAILED(hr=SetRenderTargetView())) return false;
	if (FAILED(hr=SetViewPort())) return false;

	TWindow::ResizeClient(iWidth, iHeight);
	return true;
}
HRESULT		TDevice::CreateDepthStencilState()
{
	HRESULT hr=S_OK;
	// 생성된 스왑체인의 정보를 조회
	hr = m_pSwapChain->GetDesc(&m_SwapChainDesc);
	
	ID3D11Texture2D* pTex;
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Width = m_SwapChainDesc.BufferDesc.Width;
	texDesc.Height = m_SwapChainDesc.BufferDesc.Height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;	
	if (FAILED(hr = GetDevice()->CreateTexture2D(&texDesc, NULL, &pTex)))
	{
		return hr;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvd.Format = texDesc.Format;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	if (FAILED(hr = GetDevice()->CreateDepthStencilView(pTex,
		&dsvd,
		&m_pDSV)))
	{
		return hr;
	}
	SAFE_RELEASE(pTex);	
	return hr;
}
//IDXGIAdapter  그래픽 카드 장치를 의미한다.
bool	TDevice::Init()
{
	HRESULT hr;
	if (FAILED(CreateGIFactory())){return false;}
	if (FAILED(CreateDevice())) { return false; }
	if (FAILED(CreateSwapChain())) { return false; }
	if (FAILED(CreateDepthStencilState())) { return false; }
	if (FAILED(SetRenderTargetView())) { return false; }
	if (FAILED(SetViewPort())) { return false; }

	// 반드시 디바이스 생성시 아답터 명시해야함.
	if (FAILED(hr = m_pGIFactory->MakeWindowAssociation(m_hWnd,
			DXGI_MWA_NO_WINDOW_CHANGES |
			DXGI_MWA_NO_ALT_ENTER)))
		{
			return hr;
		}

	return true;
}
bool	TDevice::PreRender()
{
	float fRed = 0.0f;
	fRed = (cosf(g_fAccumulation) * 0.5f + 0.5f);
	float fGreen = 0.0f;
	fGreen = (sinf(g_fAccumulation) * 0.5f + 0.5f);

	float clearColor[4] = { 0.4f,0.3f,0.4f,1 };
	m_pImmediateContext->ClearRenderTargetView(m_pRTV, clearColor);
	m_pImmediateContext->ClearDepthStencilView(m_pDSV,
		D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0.0f);
	return true;
}
bool	TDevice::PostRender()
{
	m_pSwapChain->Present(0, 0);
	return true;
}
bool	TDevice::Release()
{
	if (m_pDSV != NULL) m_pDSV->Release();
	if (m_pRTV != NULL)	m_pRTV->Release();
	if (m_pSwapChain != NULL)	m_pSwapChain->Release();
	if (m_pImmediateContext != NULL)	m_pImmediateContext->Release();
	if (m_pd3dDevice != NULL)	m_pd3dDevice->Release();
	//m_pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	if (m_pDebug != NULL) m_pDebug->Release();
	return true;
}

TDevice::TDevice()
{
	m_pRTV = NULL;
	m_pSwapChain = NULL;
	m_pImmediateContext = NULL;
	m_pd3dDevice = NULL;
}


TDevice::~TDevice()
{

}
