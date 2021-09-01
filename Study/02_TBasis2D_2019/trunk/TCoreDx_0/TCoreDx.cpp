#include "TCoreDx.h"


bool TCoreDx::GameInit()
{
	HRESULT hr;
	TDevice::Init();	
	TDxState::SetState(GetDevice());

	m_Timer.Init();
	m_Input.Init();

	// 랜더타켓 생성한다.
	IDXGISurface1 *pResource = 0;
	if (SUCCEEDED(m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (LPVOID*)&pResource)))
	{
		if (false == m_Write.Set(m_hWnd, m_SwapChainDesc.BufferDesc.Width,
			m_SwapChainDesc.BufferDesc.Height,	pResource))
		{
			return false;
		}
	}
	if (pResource != NULL) pResource->Release();

	Init();
	return true;
}
bool TCoreDx::GameRelease()
{
	Release();	
	m_Timer.Release();
	m_Input.Release();
	TDxState::Release();
	TDevice::Release();	
	m_Write.Release();
	return true;
}
bool TCoreDx::GameFrame()
{
	m_Timer.Frame();
	m_Input.Frame();
	PreFrame();
	Frame();
	PostFrame();
	
	return true;
}
bool TCoreDx::PreFrame()
{
	if (TDxState::g_pRSWireFrame && TDxState::g_pRSSolid)
	{
		if (m_Input.KeyCheck('V') == KEY_HOLD)
			GetContext()->RSSetState(TDxState::g_pRSWireFrame);
		else
			GetContext()->RSSetState(TDxState::g_pRSSolid);

		// 깊이 버퍼 사용해라.
		if (m_Input.KeyCheck('N') == KEY_HOLD)
			GetContext()->OMSetDepthStencilState(TDxState::g_pDisableDSS, 0x00);
		else
			GetContext()->OMSetDepthStencilState(TDxState::g_pEnableDSS, 0x00);

		if (m_Input.KeyCheck('B') == KEY_HOLD)
			GetContext()->OMSetBlendState(TDxState::g_pNoAlphaBlend, 0, -1);
		else
			GetContext()->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);

		if (m_Input.KeyCheck('C') == KEY_HOLD)
			GetContext()->RSSetState(TDxState::g_pRSNoneCullSolid);
	}
	return true;
}
bool TCoreDx::PreRender()
{
	TDevice::PreRender();
	return true;
}
bool TCoreDx::PostRender()
{
	DrawDebug();
	TDevice::PostRender();
	return true;
}
bool TCoreDx::DrawDebug(TCHAR* pString, int iX, int iY)
{
	return true;
}
bool TCoreDx::DrawDebug()
{
	if (m_Write.PreRender())
	{
		D2D_RECT_F rt = { 0.0f, 0.0f, 800.0f, 600.0f };
		m_Write.Draw(rt, m_Timer.m_csBuffer, D2D1::ColorF::Red);
		m_Write.PostRender();
	}
	return true;
}
bool     TCoreDx::ResizeClient(UINT iWidth, UINT iHeight)
{
	if (m_pSwapChain == NULL || m_pd3dDevice == NULL ) return true;
	m_Write.Release();

	TDevice::Reset(iWidth, iHeight);

	IDXGISurface1 *pResource = 0;
	if (SUCCEEDED(m_pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (LPVOID*)&pResource)))
	{
		if (false == m_Write.Set(m_hWnd, m_SwapChainDesc.BufferDesc.Width,
			m_SwapChainDesc.BufferDesc.Height,
			pResource))
		{
			return false;
		}
		//m_Write.Reset(pResource);
	}
	if (pResource != NULL) pResource->Release();
	
	TWindow::ResizeClient(iWidth, iHeight);
	return true;
}
bool TCoreDx::GameRender()
{
	PreRender();
		Render();
		m_Timer.Render();
		m_Input.Render();		
	PostRender();
	return true;
}
bool TCoreDx::GameRun()
{
	GameFrame();
	GameRender();
	return true;
};
void TCoreDx::MsgEvent(MSG msg)
{	
	m_Input.MsgEvent(msg);
};
TCoreDx::TCoreDx()
{
}


TCoreDx::~TCoreDx()
{
}
