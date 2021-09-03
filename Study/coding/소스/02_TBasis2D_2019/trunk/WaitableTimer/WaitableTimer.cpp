#define _WIN32_WINDOWS 0x401
#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("WaitableTimer");

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance
	  ,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst=hInstance;

	WndClass.cbClsExtra=0;
	WndClass.cbWndExtra=0;
	WndClass.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	WndClass.hInstance=hInstance;
	WndClass.lpfnWndProc=WndProc;
	WndClass.lpszClassName=lpszClass;
	WndClass.lpszMenuName=NULL;
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd=CreateWindow(lpszClass,lpszClass,WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
		NULL,(HMENU)NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);

	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

HANDLE hTimer;
DWORD WINAPI ThreadFunc(LPVOID temp)
{
	for (;;) {
		WaitForSingleObject(hTimer,INFINITE);
		MessageBeep(0);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR *Mes="왼쪽 버튼 : 타이머 동작, 오른쪽 버튼 : 타이머 중지";
	DWORD ThreadID;
	LARGE_INTEGER Due;

	switch (iMessage) {
	case WM_CREATE:
		hTimer=CreateWaitableTimer(NULL,FALSE,NULL);
		hWndMain=hWnd;
		CloseHandle(CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID));
		return 0;
	case WM_LBUTTONDOWN:
		Due.QuadPart=-30000000;
		SetWaitableTimer(hTimer,&Due,1000,NULL,NULL,FALSE);
		return 0;
	case WM_RBUTTONDOWN:
		CancelWaitableTimer(hTimer);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,Mes,lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		CloseHandle(hTimer);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

/* 타이머로 초당 한번씩 작업 실행하기
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch (iMessage) {
	case WM_CREATE:
		SetTimer(hWnd,1,1000,NULL);
	case WM_TIMER:
		MessageBeep(0);
		return 0;
	case WM_DESTROY:
		KillTimer(hWnd,1);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
//*/
