#include "TWindow.h"
TWindow* g_Window = NULL;

RECT			g_rtClient;
HWND			g_hWnd;
// 운영체제로 부터 메세지를 받는 함수
// 메세지는 메세지 큐에 저장되고 
// 필요시 해당 윈도가 큐에서 꺼내온다.
LRESULT CALLBACK WndProc(HWND hWnd,
				UINT msg, 
				WPARAM wParam, 
				LPARAM lParam)
{
	switch (msg)
	{		
	case WM_SIZE:
	{
		//if( wParam == SIZE_MAXIMIZED )
		if (wParam != SIZE_MINIMIZED && g_Window != NULL)
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			GetWindowRect(hWnd, &g_Window->m_rtWindow);
			GetClientRect(hWnd, &g_Window->m_rtClient);
			g_rtClient = g_Window->m_rtClient;
		}
	}break;
		// 윈도우를 다시 그려야 할때 발생
		case WM_PAINT://BeginPaint
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
		}break;
		case WM_DESTROY:
		{
			// 닫기 메세지를 윈도우 
			// 프로시져에 등록한다.
			PostQuitMessage(0);
		}break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
void TWindow::CenterWindow()
{
	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
	int x = (iScreenWidth-(m_rtWindow.right-m_rtWindow.left)) / 2;
	int y = (iScreenHeight - (m_rtWindow.bottom - m_rtWindow.top)) / 2;
	MoveWindow(m_hWnd, x, y, m_rtWindow.right, m_rtWindow.bottom, true);
}
// char 멀티바이트 => CHAR
//      유티코드   => WCHAR
bool	TWindow::SetWindow(HINSTANCE hInstance,
	TCHAR* pTitleName,
	int iWidth, int iHeight )
{
	m_hInstance = hInstance;

	//1, 윈도우 클래스 객체 등록(운영체제)
	WNDCLASSEX wc;
	wc.cbSize	= sizeof(WNDCLASSEX);
	wc.style	= CS_HREDRAW | CS_VREDRAW;
	// 윈도우 프로시져
	wc.lpfnWndProc = WndProc,//WNDPROC     lpfnWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_QUESTION);
	wc.hCursor = LoadCursor(NULL, IDC_IBEAM);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"KGCA",
	wc.hIconSm = LoadIcon(NULL, IDI_QUESTION);
	ATOM ret = RegisterClassEx(&wc);
	if (ret == false)
	{
		return false;
	}
	//2, 등록된 클래스 객체를 사용하여 윈도우 생성	
#ifdef _DEBUG
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,//DWORD dwExStyle,
		L"KGCA",// LPCWSTR lpClassName,
		pTitleName,//LPCWSTR lpWindowName,
		WS_POPUPWINDOW,//DWORD dwStyle,
		0,		0, iWidth, iHeight,
		NULL,		NULL,		m_hInstance,		NULL);
	if (m_hWnd == NULL) return false;
	g_hWnd = m_hWnd;
#else
	m_hWnd = CreateWindowEx(
		WS_EX_TOPMOST,//DWORD dwExStyle,
		L"KGCA",// LPCWSTR lpClassName,
		pTitleName,//LPCWSTR lpWindowName,
		WS_POPUPWINDOW,//DWORD dwStyle,
		0, 0, iWidth, iHeight,
		NULL, NULL, m_hInstance, NULL);
	if (m_hWnd == NULL) return false;
#endif
	GetClientRect(m_hWnd, &m_rtWindow);

	CenterWindow();

	ShowWindow(m_hWnd, SW_SHOW );
	return true;
};
bool	TWindow::Run()
{
	GameInit();
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	// 메세지 큐에서 메세지 1개를 가져온다.
	while( true )
	{
		// PM_NOREMOVE, PM_REMOVE
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// 키 해석해라.
			TranslateMessage(&msg);
			// 메세지 프로시져에 전달해라.
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
			MsgEvent(msg);
		}
		else
		{
			GameRun();
		}
	}
	GameRelease();
	return true;
};
bool	TWindow::Release()
{
	return true;
};

TWindow::TWindow()
{
	g_Window = this;
}


TWindow::~TWindow()
{
}
