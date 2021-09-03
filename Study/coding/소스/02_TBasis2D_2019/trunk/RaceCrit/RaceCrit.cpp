#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass = TEXT("RaceCrit");

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance
	, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

int X;
CRITICAL_SECTION crit;
DWORD WINAPI ThreadFunc1(LPVOID Param)
{
	HDC hdc;
	hdc = GetDC(hWndMain);
	for (int i = 0; i<100; i++) {
		EnterCriticalSection(&crit);		
			X = 100;
			Sleep(1);
			TextOut(hdc, X, 100, "강아지", 6);
		LeaveCriticalSection(&crit);
	}
	ReleaseDC(hWndMain, hdc);
	return 0;
}

DWORD WINAPI ThreadFunc2(LPVOID Param)
{
	HDC hdc;
	hdc = GetDC(hWndMain);
	for (int i = 0; i<100; i++) {
		EnterCriticalSection(&crit);
			X = 200;
			Sleep(1);
			TextOut(hdc, X, 200, "고양이", 6);
		LeaveCriticalSection(&crit);
	}
	ReleaseDC(hWndMain, hdc);
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	DWORD ThreadID;
	HANDLE hThread;

	switch (iMessage) {
	case WM_CREATE:
		InitializeCriticalSection(&crit);
		hWndMain = hWnd;
		return 0;
	case WM_LBUTTONDOWN:
		hThread = CreateThread(NULL, 0, ThreadFunc1, NULL, 0, &ThreadID);
		CloseHandle(hThread);
		hThread = CreateThread(NULL, 0, ThreadFunc2, NULL, 0, &ThreadID);
		CloseHandle(hThread);
		return 0;
	case WM_DESTROY:
		DeleteCriticalSection(&crit);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

