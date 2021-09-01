
/////////////////////////////////////////////////////////////////////
// WinMain.cpp
//
// Windows �� �⺻ �Լ���.............
// 
/////////////////////////////////////////////////////////////////////
#include "CommonInclude.h"
#include <windowsx.h>
#include "DefineEnum.h"
#include "Main.h"
#include "Network.h"

/////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------
// WinMain ��� ���� ����.
/////////////////////////////////////////////////////////////////////
HWND		g_hWnd = NULL;
HINSTANCE	g_hInstance = NULL;
BOOL		g_bActiveApp = TRUE; //�� Ȱ��ȭ ���� Ű �Է� ����WM_ACTIVATE

char		g_szIP[16];


/////////////////////////////////////////////////////////////////////
// ������ �ʱ�ȭ. 
/////////////////////////////////////////////////////////////////////
BOOL InitialWindow(HINSTANCE hInstance, WNDPROC WndProc, char* pAppName)
{
	WNDCLASSEX WndClassEx;
	HWND hWnd;

	WndClassEx.cbSize			= sizeof(WNDCLASSEX);
	WndClassEx.style			= CS_HREDRAW | CS_VREDRAW;
	WndClassEx.lpfnWndProc		= WndProc;
	WndClassEx.cbClsExtra		= 0;
	WndClassEx.cbWndExtra		= 0;
	WndClassEx.hInstance		= hInstance;
	WndClassEx.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	WndClassEx.hCursor			= LoadCursor(NULL, IDC_ARROW);
	WndClassEx.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClassEx.lpszMenuName		= NULL;
	WndClassEx.lpszClassName	= pAppName;
	WndClassEx.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&WndClassEx);


	hWnd = CreateWindowEx(0,
						pAppName,
						pAppName,
						WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
						CW_USEDEFAULT,
						CW_USEDEFAULT,
						640,
						480,
						NULL,
						NULL,
						hInstance,
						NULL);


	if ( hWnd == NULL ) return FALSE;

	g_hWnd = hWnd;
	g_hInstance = hInstance;

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	SetFocus(hWnd);


	RECT WindowRect;


	SetRect(&WindowRect, 0, 0, dfSCREEN_WIDTH, dfSCREEN_HEIGHT); //���� ������ ��Ÿ�Ͽ��� ���� ������ ������

	AdjustWindowRectEx(&WindowRect, 
						GetWindowStyle(g_hWnd), 
						GetMenu(g_hWnd) != NULL,
						GetWindowExStyle(g_hWnd)); //MSDN�� ����.


	int iX = (GetSystemMetrics( SM_CXSCREEN ) - dfSCREEN_WIDTH)		>> 1; //ȭ���� ũ�⸦ �� �� �߾�����
	int iY = (GetSystemMetrics( SM_CYSCREEN ) - dfSCREEN_HEIGHT)	>> 1;

	MoveWindow(g_hWnd, iX, iY,
				WindowRect.right - WindowRect.left,
				WindowRect.bottom - WindowRect.top, TRUE);


	return TRUE;

}




/////////////////////////////////////////////////////////////////////
// ������ ���ν���.
/////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	switch (iMsg)
	{
	case dfNETWORK_UM:
		//---------------------------------------------------------------
		// ��Ʈ��ũ �޽���
		//---------------------------------------------------------------
		if ( !netNetworkProc(wParam, lParam) )
		{
			MessageBox(hWnd, "������ ����Ǿ����ϴ�.", "�������� ����", MB_OK);
			PostQuitMessage(0);
		}
		break;

	case WM_ACTIVATEAPP:
		g_bActiveApp = (BOOL)wParam; //wParam�� TRUE�� ������ Ȱ��ȭ.
		break;
	
	case WM_IME_KEYDOWN:
		//------------------------------------------------------------
		// IME ���� ����.
		//------------------------------------------------------------
		//------------------------------------------------------------
		// �ѱ��Է� â�� ������ ���ؼ�.. ������ �������� �����.
		//------------------------------------------------------------
		HIMC hIMC;
		//------------------------------------------------------------
		// IME �� �ڵ��� ���..
		//------------------------------------------------------------
		hIMC = ImmGetContext(hWnd);
		//------------------------------------------------------------
		// IME_CMODE_ALPHANUMERIC ���� �µ�.
		//------------------------------------------------------------
		ImmSetConversionStatus(hIMC, IME_CMODE_ALPHANUMERIC, IME_CMODE_ALPHANUMERIC);
		ImmReleaseContext(hWnd,hIMC);
		//------------------------------------------------------------
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}





//////////////////////////////////////////////////////////////////
// Dialog ���ν���.
//////////////////////////////////////////////////////////////////
LPARAM CALLBACK DialogProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam) 
{
	static HWND hEditBox;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		memset(g_szIP, 0, 16);

		//---------------------------------------------------------------
		// �⺻ ������ IP �ּ� �Է�  127.0.0.1
		//---------------------------------------------------------------
		hEditBox = GetDlgItem(hWnd, IDC_EDIT);
		SetWindowText(hEditBox, "127.0.0.1");
		return TRUE;


	case WM_COMMAND:

		switch(wParam) 
		{
		case IDOK:
			//---------------------------------------------------------------
			// ���� ��ư ������ ����Ʈ â�� ���� ����.
			//---------------------------------------------------------------
			GetDlgItemText(hWnd, IDC_EDIT, g_szIP, 16);
			EndDialog(hWnd, TRUE);
			return TRUE;
			break;
		}
		break;
	}
	return FALSE;
}






/////////////////////////////////////////////////////////////////////
// ���� 
/////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	MSG				msg;

	//------------------------------------------------------------
	// timeGetTime() �Լ��� ������ üũ�� ���� ����.
	//------------------------------------------------------------
	timeBeginPeriod( 1 );

	//------------------------------------------------------------
	// ��������Ʈ �ε�
	//------------------------------------------------------------
	if ( !GameInit() )
		goto END;

	//------------------------------------------------------------
	// ������ �Է��� ���� ���̾˷α׸� �����Ѵ�.
	//------------------------------------------------------------
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_ADDR), NULL, DialogProc);

	//------------------------------------------------------------
	// �����츦 �����Ѵ�.
	//------------------------------------------------------------
	if ( !InitialWindow(hInstance, WndProc, "Base") )
		goto END;




	//------------------------------------------------------------
	// ���� �ʱ�ȭ.
	//------------------------------------------------------------
	if ( !netStartUp() )
		goto END;

	//------------------------------------------------------------
	// WSAAsyncSelect ���.
	//------------------------------------------------------------
	if ( !netSetAsyncSelect(g_hWnd) )
		goto END;

	//------------------------------------------------------------
	// ������ ���� �õ�.
	//------------------------------------------------------------
	if ( !netConnect(g_szIP) )
		goto END;



	while( 1 )
	{
		if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			if (msg.message == WM_QUIT)
				break;
			
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			//------------------------------------------------------------
			// ���� ó���� �Ǿ�߸� ������ ��������
			//------------------------------------------------------------
			if ( g_netConnect )
				Main();
		}
	}


END:

	//------------------------------------------------------------
	// ���� ���ҽ� ����
	//------------------------------------------------------------
	GameRelease();

	//------------------------------------------------------------
	// ���� ����
	//------------------------------------------------------------
	netCleanUp();


	timeEndPeriod( 1 );
	return (int)msg.wParam;
}



