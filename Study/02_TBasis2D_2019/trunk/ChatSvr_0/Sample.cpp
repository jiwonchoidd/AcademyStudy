#include "Sample.h"
#include "TChatSvr.h"
#include "TDebugString.h"
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
			int iWidth = this->m_rtWindow.right - m_rtWindow.left;
			int iHeight = this->m_rtWindow.bottom - m_rtWindow.top;
			I_Debug.m_hList = CreateWindow( L"listbox", NULL, 
						  WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
						  0, 0, iWidth -100, iHeight,
						  hWnd, (HMENU)0, m_hInstance, NULL );
		}break;
	}
	return 0;
}
int WINAPI WinMain( HINSTANCE hInstance,
				    HINSTANCE hPrevInstance,
					LPSTR lpszCmdParam,
					int nCmdShow )
{
	Sample server;	
	// 윈도우 생성 담당
	if( !server.SetWindow( hInstance, _T("ChatServer_0"), 800, 600 ) )
	{
		return 0;
	}
	SetCursor(NULL);
	// 윈도우 프로시져 담당
	if( server.Run() == false )
	{
		return 0;
	}
	return 1;
}
Sample::Sample(void)
{
}


Sample::~Sample(void)
{
}
