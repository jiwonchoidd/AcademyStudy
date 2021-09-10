#include <windows.h>
#include "CQuery.h"
#include <string>
using namespace std;

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
HINSTANCE g_hInst;
HWND hWndMain;
LPCTSTR lpszClass=_T("CQuery1");
HWND			m_hList;
//SQLConnect : https://docs.microsoft.com/ko-kr/sql/odbc/reference/develop-app/connecting-with-sqlconnect?view=sql-server-2017
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
	
	RECT rt;
	GetClientRect(hWnd, &rt);
	DWORD style = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL;
	m_hList = CreateWindow(L"listbox", NULL, style,	0, 0, rt.right, rt.bottom, hWnd, (HMENU)300, hInstance, NULL);
	ShowWindow(hWnd,nCmdShow);


	while (GetMessage(&Message,NULL,0,0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}


CQuery qry;
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	TCHAR Name[512];
	int arTab[2]={200,400};
	TCHAR str[255];

	switch(iMessage) 
	{
	case WM_CREATE:
		qry.Connect(1,_T("cigarette.mdb"));
		//qry.Connect(2,_T("FileMdb.dsn"));
		//qry.Connect(3, _T("testodbc"), _T("sa"), _T("kgca!@34"));
		return 0;
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		//TCHAR strCallSP[] = _T("{? = CALL dbo.usp_GetProductCategory(?)}");
		//qry.Exec(_T("{? = CALL dbo.usp_GetProductCategory(?)}"));
		//qry.Exec(_T("select * from dbo.Employees"));
		qry.Exec(_T("select * from dbo.Employees"));

		std::wstring data;
		while (qry.Fetch() != SQL_NO_DATA)
		{
			data.clear();
			for (int iCol = 1; iCol < 15; iCol++)
			{
				qry.GetStr(iCol, Name);
				data += Name;
				data += L",  ";											
			}
			SendMessage(m_hList, LB_ADDSTRING, 0, (LPARAM)data.c_str());
		}

		//qry.Exec(_T("select name,price,korean from tblCigar"));		
		/*while (qry.Fetch()!=SQL_NO_DATA)
		{
			qry.GetStr(_T("name"),Name);

			wsprintf(str, _T("이름:%s\t가격:%d\t국산:%s"),
				Name,qry.GetInt(_T("price")),
				(qry.GetInt(_T("korean"))==1 ? _T("예"):_T("아니오")));

			TabbedTextOut(hdc,10,y*20,str,lstrlen(str),2,arTab,0);
			y++;
		}*/
		qry.Clear();
		EndPaint(hWnd, &ps);
		return 0;
	}break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}
