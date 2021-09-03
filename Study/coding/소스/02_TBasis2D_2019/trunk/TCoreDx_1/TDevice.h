#pragma once
#include "TWindow.h"

class TDevice : public TWindow
{
public:
	ID3D11Debug*			m_pDebug;
	IDXGIFactory*			m_pGIFactory;
	// 그래픽 카드 장치 인터페이스(리소스 생성)
	ID3D11Device*			m_pd3dDevice;
	// 명령 제어,전달 및 실행 인터페이스
	ID3D11DeviceContext*	m_pImmediateContext;
	// 오프스크린 (더블버퍼링 담당)인터페이스
	IDXGISwapChain*			m_pSwapChain;
	// 오프스크린 대상 버퍼
	ID3D11RenderTargetView*	m_pRTV;
	DXGI_SWAP_CHAIN_DESC	m_SwapChainDesc;
	D3D11_VIEWPORT			m_ViewPort;
	D3D_DRIVER_TYPE			m_DriverType;
	ID3D11Device*			GetDevice() 
	{	return m_pd3dDevice;	};
	ID3D11DeviceContext*	GetContext() 
	{	return m_pImmediateContext;	};
	ID3D11DepthStencilView*   m_pDSV;
public:
	bool		Init();
	bool		PreRender();
	bool		PostRender();
	bool		Release();
public:
	HRESULT     CreateGIFactory();
	HRESULT		CreateDevice();
	HRESULT		CreateSwapChain();
	HRESULT		SetRenderTargetView();
	HRESULT		SetViewPort();
	HRESULT		CreateDepthStencilState();
	bool		Reset(UINT iWidth, UINT iHeight);
public:
	TDevice();
	virtual ~TDevice();
};

