#include "TWindow.h"
#include <assert.h>
TWindow* g_pWindow = nullptr;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND hwnd, UINT uint, WPARAM wp, LPARAM lp)
{
	assert(g_pWindow);
	return g_pWindow->MsgProc(hwnd, uint, wp, lp);
}

LRESULT TWindow::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM IParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, IParam);
	}
	return 0;
}

bool TWindow::InitWindows(HINSTANCE hInstance, int nCmdShow, const WCHAR* strWindowTitle)
{
	m_hInstance = hInstance;

	WNDCLASSEXW wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEXW));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wcex.lpszClassName = L"Jiwon";
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);

	if (!RegisterClassExW(&wcex))
	{
		return false;
	}
	RECT rc = { 0,0,400,300 };
	//클라이언트 사이즈
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindowExW(
		0,
		L"Jiwon",
		strWindowTitle,
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		rc.right - rc.left,
		rc.bottom-rc.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (m_hWnd == NULL)
	{
		return false;
	}
	GetWindowRect(m_hWnd, &m_rtWindow);
	GetClientRect(m_hWnd, &m_rtClient);

	ShowWindow(m_hWnd, nCmdShow);
	return true;
}
bool TWindow::Run()
{
	GameInit();
	MSG msg;
	while (m_bGameRun)
	{
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_bGameRun = GameRun();
		}
	}
	GameRelease();
	return true;
}
bool TWindow::GameRun()
{
	return true;
}
bool	TWindow::GameInit()
{
	return true;
}
bool	TWindow::GameRelease()
{
	return true;
}
TWindow::TWindow() : m_bGameRun(true)
{
	g_pWindow = this;
}