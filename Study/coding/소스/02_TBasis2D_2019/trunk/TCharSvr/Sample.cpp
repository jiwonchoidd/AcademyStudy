#include "Sample.h"
#include "TChatSvr.h"
#include "TChatDebugString.h"
bool Sample::Init()
{
	I_Server.Init();
	return true;
}
bool Sample::Release()
{
	I_Server.Release();
	return true;
}
LRESULT Sample::MsgProc(HWND hWnd, 
						 UINT message, 
						 WPARAM wParam, 
						 LPARAM lParam)
{
	switch( message )
	{
	case WM_CREATE:
		{
			INT iWidth = this->m_rtWindow.right - m_rtWindow.left;
			I_Debug.m_hList = CreateWindow( L"listbox", NULL, 
						  WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
						  0, 0, iWidth -100, iWidth,
						  hWnd, (HMENU)0, this->m_hInstance, NULL );
		}return 0;
	}
	return -1;
}
int WINAPI WinMain( HINSTANCE hInstance,
				    HINSTANCE hPrevInstance,
					LPSTR lpszCmdParam,
					int nCmdShow )
{
	Sample server;
	SetCursor(NULL);
	// 윈도우 생성 담당
	if( !server.SetWindow(hInstance,_T("ChatSvr_0"),800, 600) )
	{
		return 0;
	}	
	server.Run();
	return 1;
}
Sample::Sample(void)
{
}


Sample::~Sample(void)
{
}
