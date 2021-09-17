#include "GWindow.h"
#include <assert.h>
GWindow* g_pWindow = nullptr;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND hwnd, UINT uint, WPARAM wp, LPARAM lp)
{
	assert(g_pWindow);
	return g_pWindow->MsgProc(hwnd, uint, wp, lp);
}

LRESULT GWindow::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM IParam)
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

bool GWindow::InitWindows(HINSTANCE hInstance, int nCmdShow, 
	const WCHAR* strWindowTitle)
{
	m_hInstance = hInstance;

	WNDCLASSEXW wcex;
	ZeroMemory(&wcex, sizeof(WNDCLASSEXW));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hbrBackground = CreateSolidBrush(RGB(133, 143, 108));
	wcex.lpszClassName = L"KGCA";
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);

	if (!RegisterClassExW(&wcex))
	{
		return false;
	}

	m_hWnd = CreateWindowExW(
		0,
		L"KGCA",
		strWindowTitle,
		WS_OVERLAPPEDWINDOW,
		-100,
		0,
		1024,
		768,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (m_hWnd == NULL)
	{
		return false;
	}
	//wm_show
	ShowWindow(m_hWnd, nCmdShow);
	return true;
}

bool GWindow::Run()
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
bool GWindow::GameRun()
{
	return true;
}
bool	GWindow::GameInit()
{
	return true;
}
bool	GWindow::GameRelease()
{
	return true;
}
GWindow::GWindow() : m_bGameRun(true)
{
	g_pWindow = this;
}