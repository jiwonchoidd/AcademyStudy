#include "KDevice.h"
ID3D11Device* g_pd3dDevice=nullptr;		// ?????̽? ??ü
bool	KDevice::SetDevice()
{
	HRESULT hr;
	if (FAILED(CreateGIFactory()))
	{
		return false;
	}
	if (FAILED(CreateDevice()))
	{
		return false;
	}
	if (FAILED(CreateSwapChain(g_hWnd,
		g_rtClient.right,
		g_rtClient.bottom)))
	{
		return false;
	}
	if (FAILED(SetRenderTargetView()))
	{
		return false;
	}
	if (FAILED(SetDepthStencilView()))
	{
		return false;
	}
	if (FAILED(SetViewPort()))
	{
		return false;
	}
	return true;
}
HRESULT KDevice::SetRenderTargetView()
{
	HRESULT hr = S_OK;
	ID3D11Texture2D* pBackBuffer;
	if (FAILED(hr = m_pSwapChain->GetBuffer(
		0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer), hr))
	{
		return hr;
	}
	m_DefaultRT.SetRenderTargetView(pBackBuffer);
	return hr;
}
HRESULT KDevice::SetDepthStencilView()
{
	HRESULT hr = S_OK;
	DXGI_SWAP_CHAIN_DESC Desc;
	m_pSwapChain->GetDesc(&Desc);
	hr = m_DefaultDS.CreateDepthStencilView(Desc.BufferDesc.Width,
		Desc.BufferDesc.Height);
	return hr;
}

HRESULT KDevice::CreateDevice()
{
	HRESULT hr = S_OK;
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(driverTypes[0]);
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
	};
	UINT numFeatureLevels = sizeof(featureLevels) / sizeof(featureLevels[0]);

	m_DriverType = driverTypes[0];
	hr = D3D11CreateDevice(NULL, m_DriverType, NULL, createDeviceFlags,
		featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &m_pd3dDevice,
		&m_FeatureLevel, &m_pImmediateContext);
	g_pd3dDevice = m_pd3dDevice;
	return hr;
}

HRESULT KDevice::CreateGIFactory()
{
	HRESULT hr = S_OK;
	hr = CreateDXGIFactory(__uuidof(IDXGIFactory),
		(void**)(&m_pGIFactory));
	return hr;
}

HRESULT KDevice::CreateSwapChain(HWND hWnd, UINT iWidth, UINT iHeight)
{
	HRESULT hr = S_OK;
	if (m_pGIFactory == NULL) return S_FALSE;
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = iWidth;
	sd.BufferDesc.Height = iHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;
	//sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	hr = m_pGIFactory->CreateSwapChain(
		m_pd3dDevice, &sd, &m_pSwapChain);
	return hr;
}

HRESULT KDevice::SetViewPort()
{
	HRESULT hr = S_OK;
	DXGI_SWAP_CHAIN_DESC Desc;
	m_pSwapChain->GetDesc(&Desc);
	// Setup the viewport    
	m_ViewPort.Width = Desc.BufferDesc.Width;
	m_ViewPort.Height = Desc.BufferDesc.Height;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);

	return hr;
}

bool KDevice::CleanupDevice()
{
	m_DefaultDS.Release();
	m_DefaultRT.Release();

	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pGIFactory) m_pGIFactory->Release();
	m_pd3dDevice = NULL;
	m_pSwapChain = NULL;
	m_pImmediateContext = NULL;
	m_pGIFactory = NULL;
	return true;
}
