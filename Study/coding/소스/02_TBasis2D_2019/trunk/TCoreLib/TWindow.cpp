#include "TWindow.h"
#include <windowsx.h>
#include "TInput.h"
TWindow* g_Window = NULL;

RECT			g_rtClient;
HWND			g_hWnd;
HINSTANCE		g_hInstance;
DWORD			g_dwFixedFrame;
DWORD			g_dwStepTick;

// 운영체제로 부터 메세지를 받는 함수
// 메세지는 메세지 큐에 저장되고 
// 필요시 해당 윈도가 큐에서 꺼내온다.
LRESULT CALLBACK WndProc(HWND hWnd,
				UINT msg, 
				WPARAM wParam, 
				LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT crt;
	HBRUSH hBrush, OldBrush;
	COLORREF background;

	if(g_Window->MsgProc(hWnd, msg, 
		wParam, lParam)	 > 0)
	{
		return 0;
	}

	switch (msg)
	{		
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
		{
			g_Window->m_isActive = false;
			background = RGB(255, 0, 0);
		}
		else
		{
			g_Window->m_isActive = true;
			background = RGB(0, 0, 255);
		}
		SetWindowLong(hWnd, 0, (LONG)background);
		InvalidateRect(hWnd, NULL, FALSE);
		return 0;
	}break;
	
	case WM_SIZE:
	{
		//if( wParam == SIZE_MAXIMIZED )
		if (wParam != SIZE_MINIMIZED && g_Window != NULL)
		{
			UINT width = LOWORD(lParam); // Client 변경 영역임.
			UINT height = HIWORD(lParam);// Client 변경 영역임.
			g_Window->ResizeClient(width, height);
		}
	}break;
		// 윈도우를 다시 그려야 할때 발생
		case WM_PAINT://BeginPaint
		{
			GetClientRect(hWnd, &crt);
			hdc = BeginPaint(hWnd, &ps);
			background = GetWindowLong(hWnd, 0);
			hBrush = CreateSolidBrush(background);
			OldBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Rectangle(hdc, 0, 0, crt.right, crt.bottom);
			DeleteObject(SelectObject(hdc, OldBrush));
			EndPaint(hWnd, &ps);
			return 0;
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
LRESULT TWindow::MsgProc(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam)
{
	return 0;
}
void TWindow::CenterWindow()
{
	int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	int x = (iScreenWidth-(m_rtWindow.right-m_rtWindow.left)) / 2;
	int y = (iScreenHeight - (m_rtWindow.bottom - m_rtWindow.top)) / 2;
	RECT rt = { 0,0,m_rtClient.right, m_rtClient.bottom };
	AdjustWindowRectEx(&rt, GetWindowStyle(m_hWnd), GetMenu(m_hWnd) != NULL, GetWindowExStyle(m_hWnd));
	//AdjustWindowRect(&rt, GetWindowStyle(m_hWnd), FALSE);
	MoveWindow(m_hWnd, x, y, rt.right-rt.left,rt.bottom-rt.top, true);

	GetWindowRect(m_hWnd, &m_rtWindow);
	GetClientRect(m_hWnd, &m_rtClient);
	g_rtClient = m_rtClient;
}
bool	TWindow::ResizeClient(UINT iWidth, UINT iHeight)
{
	GetWindowRect(m_hWnd, &m_rtWindow);	
	GetClientRect(m_hWnd, &m_rtClient);
	g_rtClient = m_rtClient;
	return true;
}
// char 멀티바이트 => CHAR
//      유티코드   => WCHAR
bool	TWindow::SetWindow(HINSTANCE hInstance,
	const  TCHAR* pTitleName,
	int iWidth, int iHeight,
	DWORD dwStyle )
{	
	m_dwWindowStyle = dwStyle;
	m_hInstance = hInstance;
	g_hInstance = m_hInstance;
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
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"KGCA",
	wc.hIconSm = LoadIcon(NULL, IDI_QUESTION);
	if (RegisterClassEx(&wc) == false)
	{
		return false;
	}
	//2, 등록된 클래스 객체를 사용하여 윈도우 생성	
	RECT rt = { 0,0,iWidth, iHeight };
	AdjustWindowRect(&rt, m_dwWindowStyle, FALSE);
	m_rtClient = rt;
#ifdef _DEBUG
	m_hWnd = CreateWindowEx(
		WS_EX_APPWINDOW,//DWORD dwExStyle,
		L"KGCA",// LPCWSTR lpClassName,
		pTitleName,//LPCWSTR lpWindowName,
		m_dwWindowStyle,		
		rt.left, rt.top,rt.right-rt.left,rt.bottom - rt.top,
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
	GetWindowRect(m_hWnd, &m_rtWindow);
	GetClientRect(m_hWnd, &m_rtClient);
	g_rtClient = m_rtClient;
	CenterWindow();

	ShowWindow(m_hWnd, SW_SHOW );
	return true;
};
bool	TWindow::Run()
{
	GameInit();
	m_dwLastTick = timeGetTime();

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	// 메세지 큐에서 메세지 1개를 가져온다.
	while( I_Input.KeyCheck(VK_ESCAPE) != KEY_UP )
	{
		
		m_dwDeltaTick = (timeGetTime() - m_dwLastTick);
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
			if (m_dwDeltaTick >= m_dwStepTick)// && m_isActive == TRUE)
			{
				m_dwLastTick = timeGetTime();
				GameRun();				
			}		
		}		
#ifdef _DEBUG
		if (m_isActive == FALSE)
		{
			DWORD time = m_dwDeltaTick / 1000;
			DWORD Hour = (time / 60 / 60) % 60;
			DWORD Minute = (time / 60) % 60;
			DWORD Second = time % 60;
			T_STR strBuffer = L"DebugTime[" + std::to_wstring(m_dwDeltaTick) + L"] : ";
			strBuffer += std::to_wstring(Hour) + L":";
			strBuffer += std::to_wstring(Minute) + L":";
			strBuffer += std::to_wstring(Second) + L"\n";
			OutputDebugString(strBuffer.c_str());
		}
#endif
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
	m_dwDebugStartTick = 0;
	m_dwDebugTick = 0;
	m_dwDeltaTick = 0;
	m_dwLastTick = 0;
	g_dwFixedFrame = m_dwFixedFrame = 50;
	g_dwStepTick = m_dwStepTick = 1000/ m_dwFixedFrame;	
}


TWindow::~TWindow()
{
}
