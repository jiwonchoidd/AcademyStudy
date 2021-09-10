// SamleSync_0.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "SamleSync_0.h"
#include <string>
#define MAX_LOADSTRING 100
using namespace std;
#define STR(a) i##a
// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


HANDLE hThread0, hThread1, hThread2;
int   g_iXPos;
DWORD  id0, id1, id2;
HWND g_hWnd;
int iCnt0 = 0;
int iCnt1 = 0;
int iCnt2 = 0;
DWORD WINAPI Func0(LPVOID pData)
{	
	HDC hdc = GetDC(g_hWnd);
	//for (int i = 0; i < 100; i++ )
	while(iCnt0 < 100)
	{
		g_iXPos = 100;
		Sleep(100); // 스레드 스위칭
		// 현재 스레드 작업을 스위칭하기 전에
		// 반드시 현재 진행된 정보를 저장해야 한다.
		// 저장되는 구조를 "스레드 컨텍스트"라고 한다.
		// 그리고 다시 가동될 때 저장된 정보를 사용해서 
		// 이후부터 작업한다.
		TCHAR buffer[256] = { 0, };
		_stprintf_s(buffer, _T("%s_%d"), _T("KGCA"), iCnt0);
		TextOut(hdc, g_iXPos, 100, buffer, _tcslen(buffer));
		iCnt0++;
	}
	ReleaseDC(g_hWnd, hdc);
	return 0;
}
DWORD WINAPI Func1(LPVOID pData)
{
	HDC hdc = GetDC(g_hWnd);
	//for (int i = 0; i < 100; i++)
	while (iCnt1 < 100)
	{
		g_iXPos = 200;
		Sleep(100);
		TCHAR buffer[256] = { 0, };
		_stprintf_s(buffer, _T("%s_%d"), _T("GAME"), iCnt1);
		TextOut(hdc, g_iXPos, 200, buffer, _tcslen(buffer));
		iCnt1++;
	}
	ReleaseDC(g_hWnd, hdc);
	return 0;
}
DWORD WINAPI Func2(LPVOID pData)
{
	HDC hdc = GetDC(g_hWnd);
	//for (int i = 0; i < 100; i++)
	while (iCnt2 < 100)
	{
		g_iXPos = 300;
		Sleep(100);
		TCHAR buffer[256] = { 0, };
		_stprintf_s(buffer, _T("%s_%d"), _T("AAAA"), iCnt2);
		TextOut(hdc, g_iXPos, 300, buffer, _tcslen(buffer));
		iCnt2++;
	}
	ReleaseDC(g_hWnd, hdc);
	return 0;
}
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SAMLESYNC_0, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SAMLESYNC_0));

    MSG msg;

    // 기본 메시지 루프입니다.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SAMLESYNC_0));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SAMLESYNC_0);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }
   g_hWnd = hWnd;
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_LBUTTONDOWN:
	{
		iCnt0 = 0;
		iCnt1 = 0;
		iCnt2 = 0;
		hThread0 = CreateThread(0, 0, Func0, &hWnd,
			0, &id0);
			       //CREATE_SUSPENDED, &id0);		
		if (FALSE==SetThreadPriority(hThread0, THREAD_PRIORITY_TIME_CRITICAL))
		{
			int kkk = 0;
		}
		hThread1 = CreateThread(0, 0, Func1, &hWnd,
			0, &id1);
		if (FALSE == SetThreadPriority(hThread1, THREAD_PRIORITY_NORMAL))
		{
			int kkk = 0;
		}
		hThread2 = CreateThread(0, 0, Func2, &hWnd,
			0, &id2);
		if (FALSE == SetThreadPriority(hThread2, THREAD_PRIORITY_IDLE))
		{
			int kkk = 0;
		}
		int dw0 = GetThreadPriority(hThread0);
		int dw1 = GetThreadPriority(hThread1);
		int dw2 = GetThreadPriority(hThread2);
		int k = 0;
	}break;
	case WM_MBUTTONDOWN:
	{
		ResumeThread(hThread0);
		ResumeThread(hThread1);
		ResumeThread(hThread2);
	}break;
	case WM_RBUTTONDOWN:
	{
		SuspendThread(hThread0);
		SuspendThread(hThread1);
		SuspendThread(hThread2);
		//ResumeThread(hThread0);
		//ResumeThread(hThread1);
		//ResumeThread(hThread2);
		/*DWORD  dwExitCode;
		BOOL result = GetExitCodeThread(hThread0, &dwExitCode);
		if (result != 0)
		{
			if (dwExitCode == STILL_ACTIVE)
			{
				TerminateThread(hThread0, 0);
				CloseHandle(hThread0);
			}
		}*/
		//CloseHandle(hThread1);
		//CloseHandle(hThread0);
		//CloseHandle(hThread2);
	}break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
