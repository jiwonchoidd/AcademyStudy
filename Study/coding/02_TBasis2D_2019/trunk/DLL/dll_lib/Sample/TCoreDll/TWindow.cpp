#include "stdafx.h"
#include "TWindow.h"
TWindow* g_pWindow = 0;

LRESULT CALLBACK WndProc(	HWND hWnd,
							UINT msg,
							WPARAM wParam,
							LPARAM lParam)
{
	return g_pWindow->MsgProc( hWnd, msg, wParam, lParam );
}
LRESULT TWindow::MsgProc(	HWND hWnd,
							UINT msg,
							WPARAM wParam,
							LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:// 닫기
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	return 0;
}
bool    TWindow::SetRegister()
{
	// 윈도우 클래스 생성 구조체	
	WNDCLASSEX  wc;
	wc.cbSize		= sizeof( WNDCLASSEX );
    wc.style		= CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc	= WndProc;
    wc.cbClsExtra	= 0;
    wc.cbWndExtra	= 0;
    wc.hInstance	= m_hInstance;
    wc.hIcon		= LoadIcon( NULL,IDI_APPLICATION);
    wc.hCursor		= LoadCursor( NULL, IDC_ARROW );
	// m_iBackgroundColor가 변수(상수를 사용해야 한다.)이면 
	// 화면 갱신시 문제가 된다. 
    //wc.hbrBackground=(HBRUSH)GetStockObject(m_iBackgroundColor);
	wc.hbrBackground=(HBRUSH)GetStockObject(m_iBackgroundColor);//WHITE_BRUSH
    wc.lpszMenuName = NULL;
    wc.lpszClassName= L"KGCA";
    wc.hIconSm		= NULL;
	// 윈도우 클래스 등록
	if( RegisterClassEx(&wc) == false )
	{
		// 오류로 윈도우 종료
		return false;
	}
	return true;
}
bool	TWindow::CreateWin(WCHAR* strTitleName)
{
	// 클라이언트 영역의 크기( 800 * 600 )
	RECT rt = {0, 0, m_iWindowWidth, m_iWindowHeight};
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE );
	// 윈도우 생성
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,//_In_ DWORD dwExStyle,
		L"KGCA",//_In_opt_ LPCWSTR lpClassName,
		strTitleName,//_In_opt_ LPCWSTR lpWindowName,
		WS_OVERLAPPEDWINDOW,//_In_ DWORD dwStyle,
		0,//_In_ int X,
		0,//_In_ int Y,
		rt.right-rt.left,//_In_ int nWidth,
		rt.bottom-rt.top,//_In_ int nHeight,
		NULL,//_In_opt_ HWND hWndParent,
		NULL,// _In_opt_ HMENU hMenu,
		m_hInstance,//_In_opt_ HINSTANCE hInstance,
		NULL//_In_opt_ LPVOID lpParam);
		);
	if( m_hWnd == NULL )
	{
		return false;
	}
	GetClientRect(m_hWnd, &m_rtClient );
	GetWindowRect(m_hWnd, &m_rtWindow );

	return true;
}
bool    TWindow::SetWindow(HINSTANCE hInstance,
				   			int nCmdShow,
							WCHAR* strTitleName)
{
	m_hInstance = hInstance;
	if( !SetRegister())
	{
		return false;
	}
	if( !CreateWin(strTitleName))
	{
		return false;
	}
	ShowWindow(m_hWnd,nCmdShow);
	UpdateWindow( m_hWnd );
	return true;
}
void TWindow::SetPositionSize( int iX, 
		int iY, int iWidth, int iHeight)
{
	m_iWindowX		= iX ;
	m_iWindowY		= iY;
	m_iWindowWidth	= iWidth;
	m_iWindowHeight	= iHeight;	
};
TWindow::TWindow(void) : m_iBackgroundColor(GRAY_BRUSH)
{
	g_pWindow		= this;
	m_iWindowX		= 0 ;
	m_iWindowY		= 0;
	m_iWindowWidth	= 800;
	m_iWindowHeight	= 600;	
}


TWindow::~TWindow(void)
{
}
