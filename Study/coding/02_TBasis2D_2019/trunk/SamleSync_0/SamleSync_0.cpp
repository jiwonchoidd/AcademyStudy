// SamleSync_0.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "SamleSync_0.h"
#include <string>
#define MAX_LOADSTRING 100
using namespace std;
#define STR(a) i##a
// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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
		Sleep(100); // ������ ����Ī
		// ���� ������ �۾��� ����Ī�ϱ� ����
		// �ݵ�� ���� ����� ������ �����ؾ� �Ѵ�.
		// ����Ǵ� ������ "������ ���ؽ�Ʈ"��� �Ѵ�.
		// �׸��� �ٽ� ������ �� ����� ������ ����ؼ� 
		// ���ĺ��� �۾��Ѵ�.
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

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SAMLESYNC_0, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SAMLESYNC_0));

    MSG msg;

    // �⺻ �޽��� �����Դϴ�.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
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
            // �޴� ������ ���� �м��մϴ�.
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
            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
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

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
