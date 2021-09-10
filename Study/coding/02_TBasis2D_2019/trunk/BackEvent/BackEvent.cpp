#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("BackEvent");

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

HANDLE hEvent;
int buf[100];
DWORD WINAPI ThreadFunc(LPVOID temp)
{
	int i;
	for (i=0;i<100;i++) {
		Sleep(30);
		buf[i]=i;
		if (i == 30) 
			SetEvent(hEvent);
	}
	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	int i;
	TCHAR str[128];
	HDC hdc;
	PAINTSTRUCT ps;
	DWORD ThreadID;
	HANDLE hThread;
	TCHAR *Mes="마우스 왼쪽 버튼을 눌러 주십시오";

	switch (iMessage) {
	case WM_CREATE:
		hWndMain=hWnd;
		// 이벤트속성, 수동리셋이벤트,  초기상태, 이벤트이름
		hEvent=CreateEvent(NULL,FALSE,FALSE, NULL );
		return 0;
	case WM_LBUTTONDOWN:
		InvalidateRect(hWnd,NULL,TRUE);
		UpdateWindow(hWnd);
		hThread = CreateThread(NULL, 0, ThreadFunc, NULL, 0, &ThreadID);
		CloseHandle(hThread);
		WaitForSingleObject(hEvent, INFINITE);
		hdc=GetDC(hWnd);
		for (i=0;i<=30;i++) {
			wsprintf(str, "%d Line is %d",i,buf[i]);
			TextOut(hdc, 10, i*20, str, lstrlen(str));
		}
		ReleaseDC(hWnd, hdc);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		TextOut(hdc,100,100,Mes,lstrlen(Mes));
		EndPaint(hWnd,&ps);
		return 0;
	case WM_DESTROY:
		CloseHandle(hEvent);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

