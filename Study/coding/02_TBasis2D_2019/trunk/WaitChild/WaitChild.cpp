#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("WaitChild");

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

LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	STARTUPINFO si = {sizeof(STARTUPINFO),};
	PROCESS_INFORMATION pi;
	TCHAR *Mes="마우스 왼쪽 버튼을 누르면 카드놀이가 실행됩니다.";
	MSG Message;

	switch (iMessage) {
	case WM_LBUTTONDOWN:
		if (CreateProcess(NULL,"sol.exe",NULL,NULL,FALSE,0,NULL,NULL,&si,&pi)) {
			// 차일드 종료시까지 대기하기
			//WaitForSingleObject(pi.hProcess,INFINITE);
//차일드 대기 중에 메시지 펌핑하기
			while (WaitForSingleObject(pi.hProcess,10) != WAIT_OBJECT_0) {
				if (PeekMessage(&Message,NULL,0,0,PM_REMOVE)) {
					if ((Message.message==WM_PAINT) || (Message.message==WM_NCPAINT) || (Message.message==WM_ERASEBKGND)) {
						TranslateMessage(&Message);
						DispatchMessage(&Message);
					}
				}
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc, 100, 100, Mes, lstrlen(Mes));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
