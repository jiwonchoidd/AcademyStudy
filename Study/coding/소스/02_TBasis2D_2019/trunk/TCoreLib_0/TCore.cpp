#include "TCore.h"
#include "TTimer.h"
#include "TInput.h"
#include "TImageMgr.h"

HDC	 TCore::GetOffDC()
{ 
	return m_ObjectBG.m_hDrawDC;
}

bool TCore::TCoreInit()
{
	I_Timer.Init();
	I_Input.Init();
	m_hScreenDC = GetDC( m_hWnd );

	m_ObjectBG.Init();
	m_ObjectBG.CreateOffScreenDC(m_hScreenDC,m_iWidth, m_iHeight );
	m_ObjectBG.Load( _T("../data/aaa.bmp"), NULL);
	Init();
	return true;
}
bool TCore::TCoreRender()
{	
	PreRender();
		Render();
	PostRender();
	return true;
}
bool TCore::PreRender()
{	
	I_Timer.Render();
	I_Input.Render();
	RECT rt;
	SetRect( &rt, 0, 0, m_iWidth, m_iHeight );
	FillRect(m_ObjectBG.m_hDrawDC, &rt, 
		(HBRUSH)GetStockObject(WHITE_BRUSH));
	m_ObjectBG.Draw(m_ObjectBG.m_hDrawDC, 0, 0, 800, 600,
		m_ObjectBG.m_pImage->m_hMemDC, 0, 0,SRCCOPY );
	return true;
}
bool TCore::PostRender()
{	
	BitBlt( m_hScreenDC, 0, 0, m_iWidth, 
			m_iHeight,
		    m_ObjectBG.m_hDrawDC, 0,0, SRCCOPY );

	HDC hClientDC = GetDC(m_hWnd);
	SetBkMode(hClientDC, TRANSPARENT );
	SetTextColor( hClientDC, RGB(255, 100,100 ));
	TextOut(hClientDC, 0, 0, I_Timer.m_strBuffer,
		_tcslen(I_Timer.m_strBuffer));	   
	ReleaseDC(m_hWnd, hClientDC );
	return true;
}
bool TCore::TCoreFrame()
{
	I_Timer.Frame();
	I_Input.Frame();
	Frame();
	return true;
}
bool TCore::TCoreRelease()
{
	I_Timer.Release();
	I_Input.Release();
	Release();
	I_ImageMgr.Release();
	return true;
}
bool TCore::Run()
{
	TCoreInit();

	MSG msg; // Quit = 0
	ZeroMemory(&msg, sizeof(msg ));
	while( msg.message != WM_QUIT )
	{
		if( PeekMessage( &msg, NULL, 0,0, PM_REMOVE ))
		{
			TranslateMessage(&msg); // Å° ÇØ¼®
			DispatchMessage(&msg);
		}
		else
		{
			//Sleep(1);
			TCoreFrame();
			TCoreRender();
		}
	}

	TCoreRelease();
	UnregisterClass(L"TCoreLib", m_hInstance);
	return true;
}
TCore::TCore(void)
{
}


TCore::~TCore(void)
{
}
