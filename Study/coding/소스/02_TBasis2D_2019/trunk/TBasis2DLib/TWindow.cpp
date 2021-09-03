#include "TWindow.h"
#include "TInput.h"
//rmdir /s/q "..\\..\\include"
//rmdir /s/q "..\\..\\lib"
//md "..\\..\\include"
//md "..\\..\\lib"

namespace TBASIS_CORE_LIB
{
	HINSTANCE		g_hInstance;
	HWND			g_hWnd = 0;
	bool			g_bActive = false;
	RECT			g_rtClient;
	TWindow*		g_pWindow;	
}

static bool		m_bDrag =false;
static POINT	m_ptClick;

LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{	
	int iRet = g_pWindow->MsgProc(hWnd, message, wParam, lParam);
	if (iRet > -1)
	{
		return iRet;
	}
	switch (message)
	{
	//case WM_NCCREATE:
	//{
	//	return TRUE;
	//}break;
	case WM_NCLBUTTONDOWN:
	case  WM_NCMOUSEMOVE:
	{		
		//return TRUE;
	}break;
	case WM_MBUTTONDOWN:
	{
		return DefWindowProc(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
		m_bDrag = true;
		m_ptClick.x = LOWORD(lParam);
		m_ptClick.y = HIWORD(lParam);
	}return 0;
	case WM_MOUSEMOVE:
	{
		if (m_bDrag)
		{
			RECT rtWindow;
			GetWindowRect(hWnd, &rtWindow);
			rtWindow.left -= m_ptClick.x - LOWORD(lParam);
			rtWindow.top -= m_ptClick.y - HIWORD(lParam);
#if NDEBUG
			SetWindowPos(hWnd, HWND_TOPMOST, rtWindow.left, rtWindow.top, 0, 0, SWP_NOSIZE);//HWND_TOP
#else
			SetWindowPos(hWnd, HWND_NOTOPMOST, rtWindow.left, rtWindow.top, 0, 0, SWP_NOSIZE);
#endif
		}
	}return 0;
	case WM_MBUTTONUP:
	{
		m_bDrag = false;
	}return 0;
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			g_bActive = false;
			m_bDrag = false;
			SetWindowText(g_hWnd, L"INACTIVE");
		}
		else
		{
			::SetFocus(g_hWnd);
			g_bActive = true;
			SetWindowText(g_hWnd, L"ACTIVE");
		}
		
	}break;
	case WM_DESTROY:
		PostQuitMessage(0);// WM_QUIT
		break;	
	}	
	return DefWindowProc(hWnd, message, wParam, lParam);
}
LRESULT	 TWindow::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return -1;
}
bool TWindow::MsgProcess()
{
	while (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&m_msg);
		DispatchMessage(&m_msg);
		if (m_msg.message == WM_QUIT)
		{
			return false;
		}
		MsgEvent(m_msg);
	}
	
	return true;
}
void TWindow::MsgEvent(MSG msg)
{
}
bool	TWindow::SetWindow(HINSTANCE hInstance,const  TCHAR* pTitleName,int iWidth, int iHeight)
{
	g_hInstance = hInstance;
	m_hInstance = hInstance;
	//1, 윈도우 클래스 객체 등록
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpfnWndProc = WndProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = L"KGCAWINDOW";
	if (RegisterClassEx(&wc) == 0)
	{
		return false;
	}
	//2, 윈도우 생성
	RECT rt = { 0,0, 800, 600 };
	AdjustWindowRectEx(&rt, m_dwStyle, FALSE, m_dwExStyle);
	m_hWnd = CreateWindowEx(
		m_dwExStyle,
		m_szClassName.c_str(), m_szWindowName.c_str(),
		m_dwStyle,
		0, 0, 
		rt.right-rt.left, 
		rt.bottom - rt.top,
		nullptr, nullptr,
		m_hInstance, nullptr);

	if (m_hWnd == NULL)
	{
		return false;
	}
	g_hWnd = m_hWnd;
	GetClientRect(m_hWnd, &m_rtClient);
	g_rtClient = m_rtClient;
	GetWindowRect(m_hWnd, &m_rtWindow);
	ShowWindow(m_hWnd, SW_SHOW);
	return true;
}

TWindow::TWindow()
{
	g_pWindow = this;
	ZeroMemory(&m_msg, sizeof(MSG));
	m_bDrag = false;
	m_szClassName = L"KGCAWINDOW";
#if NDEBUG
	m_dwExStyle		= WS_EX_TOPMOST;
	m_szWindowName	= L"Sample Project(Release)";
	m_dwStyle		= WS_POPUPWINDOW;// WS_OVERLAPPEDWINDOW,,;
#else
	m_dwExStyle		= WS_EX_APPWINDOW;
	m_szWindowName	= L"Sample Project(Debug)";
	m_dwStyle		= WS_OVERLAPPEDWINDOW;
#endif
}
TWindow::~TWindow()
{

}