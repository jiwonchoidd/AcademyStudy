#include "TServer.h"

#include <windows.h>
#include <stdio.h>

#define IDC_MAIN_EDIT 101

HWND hBackWnd;

void DrawInWindow(HWND hWndToPaint) {
	HDC hdc = GetDC(hWndToPaint);
	if (!hdc)printf("Invalid handle\n");
	HPEN hPen = CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
	SelectObject(hdc, hPen);
	static float x = 620, y = 1, tg = 0.5, ctg = 2;
	static int Xone = 1, Yone = 1;//depending on later recalculation this may become negative
	MoveToEx(hdc, (int)x, (int)y, NULL);
	if (tg < 1) {
		y += tg;
		x += Xone;
	}
	else {
		y += Yone;
		x += ctg;
	}
	if (!LineTo(hdc, (int)x, (int)y))printf("There are paint problem\n");
	ReleaseDC(hWndToPaint, hdc);
	//Now recalculate direction
	RECT WndRect;
	GetClientRect(hWndToPaint, &WndRect);
	if (x >= WndRect.right) {
		if (ctg > 0)ctg *= -1;//make negative
		Xone = -1;
	}
	if (x <= WndRect.left) {
		if (ctg < 0)ctg *= -1;//make positive
		Xone = 1;
	}
	if (y >= WndRect.bottom) {
		if (tg > 0)tg *= -1;//make negative
		Yone = -1;
	}
	if (y <= WndRect.top) {
		if (tg < 0)tg *= -1;//make positive
		Yone = 1;
	}
}

LRESULT __stdcall MyMainCallBckProcedure(HWND window, unsigned msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	case WM_KEYDOWN:
		if (wp == VK_ESCAPE)PostQuitMessage(0);
		break;
	case WM_DESTROY:
		printf("\ndestroying window\n");
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		HWND hEdit;
		RECT rcClient;

		GetClientRect(window, &rcClient);
		hEdit = GetDlgItem(window, IDC_MAIN_EDIT);
		SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
		break;
	case WM_WINDOWPOSCHANGED: {//LPARAM is a ptr to WINDOWPOS
		RECT BckWndRect;
		if (!GetWindowRect(hBackWnd, &BckWndRect))printf("ERR: getting backwnd rectangle\n");
		bool IsRepaint;
		WINDOWPOS* pNewPos = (WINDOWPOS*)lp;
		if (BckWndRect.left + BckWndRect.right != pNewPos->cx
			|| BckWndRect.top + BckWndRect.bottom != pNewPos->cy)IsRepaint = true;
		else IsRepaint = false;
		MoveWindow(hBackWnd, pNewPos->x, pNewPos->y, pNewPos->cx, pNewPos->cy, IsRepaint);
		break;
	}
	default:
		return DefWindowProc(window, msg, wp, lp);
	}
}

WNDPROC lpEditWndProc;

LRESULT CALLBACK MyEditCallBckProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if ((uMsg == WM_CHAR) && (wParam == VK_ESCAPE))
	{
		PostQuitMessage(0);
		return 0;
	}
	DrawInWindow(hBackWnd);
	lpEditWndProc(hWnd, uMsg, wParam, lParam);
}

bool CreateWindows() 
{
	//creating back window
	const TCHAR* backwnd = L"backwnd";
	WNDCLASSEX backwndclass = { sizeof(WNDCLASSEX), CS_DBLCLKS, MyMainCallBckProcedure,
							0, 0, GetModuleHandle(0), LoadIcon(0,IDI_APPLICATION),
							LoadCursor(0,IDC_ARROW), HBRUSH(COLOR_WINDOW + 1),
							0, backwnd, LoadIcon(0,IDI_APPLICATION) };
	if (RegisterClassEx(&backwndclass) < 0) {
		printf("ERR: in registering second window class\n");
		return false;
	}
	hBackWnd = CreateWindowEx(0, backwnd, L"title", WS_EX_TOOLWINDOW |
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		640, 480, 0, 0, GetModuleHandle(0), 0);
	if (!hBackWnd) {
		printf("ERR: in creating background window\n");
		return false;
	}
	ShowWindow(hBackWnd, SW_SHOWDEFAULT);
	//Creating front window
	const TCHAR* const frontwnd = L"frontwnd";
	WNDCLASSEX wndclass = { sizeof(WNDCLASSEX), CS_DBLCLKS, MyMainCallBckProcedure,
							0, 0, GetModuleHandle(0), LoadIcon(0,IDI_APPLICATION),
							LoadCursor(0,IDC_ARROW), HBRUSH(COLOR_WINDOW + 1),
							0, frontwnd, LoadIcon(0,IDI_APPLICATION) };
	if (RegisterClassEx(&wndclass) < 0) {
		printf("ERR: in registering foreground window class\n");
		return false;
	}
	HWND window = CreateWindowEx(0, frontwnd, L"title",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		640, 480, 0, 0, GetModuleHandle(0), 0);
	if (!window) {
		printf("ERR: in creating foreground window\n");
		return false;
	}
	ShowWindow(window, SW_SHOWDEFAULT);
	//creating textbox
	HWND hEdit = CreateWindowEx(0, L"edit", L"", WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL
		| ES_MULTILINE | ES_AUTOVSCROLL, CW_USEDEFAULT, CW_USEDEFAULT, 640,
		480, window, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(0), 0);
	HFONT hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
	//Let's try to catch some messages in TextBox...
	lpEditWndProc = (WNDPROC)SetWindowLongPtr(hEdit, GWLP_WNDPROC, (LONG_PTR)&MyEditCallBckProcedure);
	//Making foreground window transparent
	SetWindowLongPtr(window, GWL_EXSTYLE, WS_EX_LAYERED | GetWindowLongPtr(window, GWL_EXSTYLE));
	SetLayeredWindowAttributes(window, 0, (255 * 50) / 100, LWA_ALPHA);
	return true;
	//###
}

void main()
{
	// cout 쓰레드 사용시 안전하게 처리된다.
	cout.sync_with_stdio(true);
	I_Server.Init();
	I_Server.Start();

/*	if (!CreateWindows()) { printf("Something gone wrong\n"); return; }
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}*/	
}