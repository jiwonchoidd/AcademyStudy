#include <windows.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=TEXT("CQuey2");

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

#include "CQuery.h"
CQuery qry;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	int nExmp=6;
	static TCHAR szResult[255];
	TCHAR szTmp[255];
	int iResult;

	switch (iMessage) {
	case WM_CREATE:
		qry.Connect(2,_T("../../data/db/cigarette.mdb.dsn"));	
		//qry.Connect(1, _T("cigarette.accdb"));
		//qry.Connect(1, _T("cigarette.mdb"));
		return 0;
	case WM_LBUTTONDOWN:
		if (nExmp == 1) {
			qry.Exec(_T("select price from tblCigar where name='this'"));
			qry.Fetch();
			//iResult=qry.GetInt("price");
			iResult=qry.GetInt(1);
			qry.Clear();
			wsprintf(szResult, _T("This의 가격=%d"),iResult);
		}

		if (nExmp == 2) {
			qry.Exec(_T("select name from tblCigar where price=1200"));
			qry.Fetch();
			//qry.GetStr("name",szTmp);
			qry.GetStr(1,szTmp);
			qry.Clear();
			wsprintf(szResult, _T("가격이 1200원인 담배=%s"),szTmp);
		}


		if (nExmp == 3) {
			iResult=qry.ExecGetInt(_T("select price from tblCigar where name='this'"));
			wsprintf(szResult, _T("This의 가격=%d"),iResult);
		}
  
		if (nExmp == 4) {
			qry.ExecGetStr(_T("select name from tblCigar where price=1200"),szTmp);
			wsprintf(szResult, _T("가격이 1200원인 담배=%s"),szTmp);
		}

		if (nExmp == 5) {
			wsprintf(szResult, _T("가격이 1300원인 담배 개수는 %d개이다."),
				qry.ExecGetInt(_T("select count(*) from tblCigar where price=1300")));
		}

		if (nExmp == 6) {
			wsprintf(szResult, _T("가격이 1300원인 담배 개수는 %d개이다."),
				qry.ExecGetInt(_T("select count(*) from tblCigar where price=1300")));
		}

		InvalidateRect(hWnd,NULL,TRUE);
		return 0;
	case WM_PAINT:
		hdc=BeginPaint(hWnd, &ps);
		TextOut(hdc,10,10,szResult,lstrlen(szResult));
		EndPaint(hWnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
