#include "TCoreDx.h"

bool TCoreDx::GameInit()
{
	HRESULT hr;
	TDevice::Init();	
	TDxState::SetState(GetDevice());
	m_pMainCamera = &m_DebugCamera;

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

	m_LineAxis.Create(GetDevice(),
			GetContext(),
			L"../../../data/lobby.bmp",
			L"../../../data/shader/Shape.vsh",
			L"../../../data/shader/Shape.psh");
		D3DXMatrixIdentity(&m_matWorldLine);

	// 뷰 행렬
	m_pMainCamera->Init();
	m_pMainCamera->SetViewMatrix(D3DXVECTOR3(0, 0, -50.0f),
		D3DXVECTOR3(0, 0, 1.0f));
	m_pMainCamera->SetProjMatrix((float)D3DX_PI * 0.25f,
		m_rtClient.right / (FLOAT)m_rtClient.bottom, 0.1f, 1000.0f);
	Init();
	return true;
}
bool TCoreDx::GameRelease()
{
	Release();
	m_LineAxis.Release();
	m_Write.Release();
	m_Timer.Release();
	m_Input.Release();
	TDxState::Release();
	TDevice::Release();	
	return true;
}
bool TCoreDx::GameFrame()
{
	m_Timer.Frame();
	m_Input.Frame();
	PreCoreFrame();
		Frame();
	PostCoreFrame();
	return true;
}
bool TCoreDx::PreRender()
{
	return TDevice::PreRender();
}
bool TCoreDx::PostRender()
{
	return TDevice::PostRender();
}
bool TCoreDx::PreCoreFrame()
{
	// 월드 행렬 업데이트
	float fTime = m_Timer.m_fAccumulation;
	PreFrame();
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
	
	// 카메라 행렬 업데이트
	if (m_Input.KeyCheck('W') == KEY_HOLD)
	{
		m_pMainCamera->Front();
	}
	if (m_Input.KeyCheck('S') == KEY_HOLD)
	{
		m_pMainCamera->Back();
	}
	if (m_Input.KeyCheck('A') == KEY_HOLD)
	{
		m_pMainCamera->Left();
	}
	if (m_Input.KeyCheck('D') == KEY_HOLD)
	{
		m_pMainCamera->Right();
	}
	if (m_Input.KeyCheck('Q') == KEY_HOLD)
	{
		m_pMainCamera->Up();
	}
	if (m_Input.KeyCheck('E') == KEY_HOLD)
	{
		m_pMainCamera->Down();
	}
	if (m_Input.KeyCheck(VK_SPACE) == KEY_HOLD)
	{
		m_pMainCamera->m_fSpeed += m_Timer.m_fSecondPerFrame * 50.0f;
	}
	else
	{
		if (m_pMainCamera->m_fSpeed > 5.0f)
			m_pMainCamera->m_fSpeed -= m_Timer.m_fSecondPerFrame * 100.0f;
	}
	float fDir = (m_Input.m_sMouseWheelDelta / 120.0f);
	D3DXVECTOR4 vDir(m_Input.m_MovePt.x,m_Input.m_MovePt.y,	0, fDir);

	m_pMainCamera->Update(vDir);
	m_pMainCamera->Frame();
	return true;
}
bool TCoreDx::PreCoreRender()
{
	PreRender();
	return true;
}
bool TCoreDx::PostCoreRender()
{
	DrawDebug();
	PostRender();
	return true;
}
bool TCoreDx::DrawDebug(TCHAR* pString, int iX, int iY)
{
	return true;
}
bool TCoreDx::DrawDebug()
{
	GetContext()->OMSetBlendState(TDxState::g_pAlphaBlend, 0, -1);
	m_LineAxis.SetMatrix(m_matWorldLine,m_pMainCamera->m_matView,m_pMainCamera->m_matProj);
	m_LineAxis.Render();

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
		if (false == m_Write.Set(m_hWnd, m_SwapChainDesc.BufferDesc.Width,m_SwapChainDesc.BufferDesc.Height,pResource))
		{
			return false;
		}		
	}
	if (pResource != NULL) pResource->Release();
	
	TWindow::ResizeClient(iWidth, iHeight);
	return true;
}
bool TCoreDx::GameRender()
{
	PreCoreRender();
		Render();
		m_Timer.Render();
		m_Input.Render();		
		PostCoreRender();
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
