#include "TWindow.h"
TWindow*    g_pWindow = NULL;
HWND		g_hWnd = NULL;
HINSTANCE	g_hInstance = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, 
						 UINT message, 
						 WPARAM wParam, 
						 LPARAM lParam)
{
	HRESULT  iReturn= 0;
	if( iReturn = g_pWindow->MsgProc(hWnd,message, wParam,lParam) > 0)
	{
		return iReturn;
	}
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
LRESULT TWindow::MsgProc(HWND hWnd, 
						 UINT message, 
						 WPARAM wParam, 
						 LPARAM lParam)
{	
	return 0;
}
void TWindow::SetWinRect(
					 int ix, int iy,
					 int iWidth, 
					 int iHeight)
{
	m_iStartX = ix;
	m_iStartY = iy;
	m_iWidth  = iWidth;
	m_iHeight = iHeight;
}
void TWindow::SetStyle( int iStyle)
{
	m_iStyle = iStyle;
}
bool TWindow::SetRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);    
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
	wc.hIcon		= LoadIcon(NULL, IDI_APPLICATION );
	wc.hCursor		= LoadCursor(NULL, IDI_APPLICATION );
    wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = L"TCoreLib"; // _T("aaa") tchar.h
    wc. hIconSm  = LoadCursor(NULL, IDI_APPLICATION );

	if( !RegisterClassEx(&wc))
	{
		return false;
	}
	return true;
}
bool TWindow::SetWin(HINSTANCE hInstance,
				     int nCmdShow,
					 TCHAR* szTitle)
{
	m_hInstance = hInstance;
	g_hInstance = hInstance;

	SetRegisterClass(m_hInstance);	
	RECT rc = {0,0,m_iWidth,m_iHeight};
	if( TRUE != AdjustWindowRectEx( &rc, m_iStyle, FALSE, WS_EX_APPWINDOW) )
	{
		return false;
	}
	m_hWnd = CreateWindowEx( 
		 WS_EX_APPWINDOW
		, L"TCoreLib"
		, szTitle
		, m_iStyle
		, m_iStartX
		, m_iStartY
		, rc.right-rc.left//m_iWidth
		, rc.bottom - rc.top//m_iHeight
		, NULL
		, NULL
		, hInstance
		, NULL );
	if( m_hWnd == NULL)
	{
		return false;
	}
	g_hWnd = m_hWnd;

	GetWindowRect( m_hWnd, &m_rcWindowRect);
	GetClientRect( m_hWnd, &m_rcClientRect);

	ShowWindow(m_hWnd,nCmdShow);
	SetCursor(NULL);
	return true;
}


TWindow::TWindow(void)
{
	m_hWnd		= NULL;
	g_pWindow	= this;
	m_iStartX	= 0;
	m_iStartY	= 0;
	m_iWidth	= 800;
	m_iHeight	= 600;
	m_iStyle = WS_OVERLAPPED     | 
                             WS_CAPTION        | 
                             WS_SYSMENU        | 
                             WS_THICKFRAME     | 
                             WS_MINIMIZEBOX    | 
                             WS_MAXIMIZEBOX;
}


TWindow::~TWindow(void)
{
}
