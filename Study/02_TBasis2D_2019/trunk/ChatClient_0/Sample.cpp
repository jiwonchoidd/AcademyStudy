#include "Sample.h"
#include "TDebugString.h"
#include "TStd.h"
LRESULT Sample::MsgProc(HWND hWnd,
					UINT msg,
					WPARAM wParam,
					LPARAM lParam)
{
	switch( msg )
	{	
	case WM_CREATE:	
		{
		m_hEdit = CreateWindow(_T("edit"),
			NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			500, 10, 200, 25, hWnd,
			(HMENU)202, g_hInstance, NULL );
		CreateWindow(_T("button"), _T("Send"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 700, 10, 50, 25, hWnd,
			(HMENU)302, g_hInstance, NULL );
		int iWidth = this->m_rtWindow.right - m_rtWindow.left;
		int iHeight = this->m_rtWindow.bottom - m_rtWindow.top;
		I_Debug.m_hList = CreateWindow( L"listbox", NULL, 
						  WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
						  0, 0, iWidth -300, iHeight,
						  hWnd, (HMENU)0, m_hInstance, NULL );
		}return 0;
	case WM_COMMAND:
		switch( LOWORD(wParam))
		{
			case 302:
			{				
				GetWindowText( m_hEdit, m_strBuffer, 128 );
				I_Debug.ConvertWideStringToAnsiCch(
							m_Client.m_strBuffer, 
							m_strBuffer,
							128 );
				InvalidateRect( hWnd, NULL, NULL );
				if( m_Client.m_bLoginOK==false && 
					m_Client.SendMsg( m_Client.m_strBuffer,PACKET_CHAT_NAME_ACK) )
				{
					m_Client.m_bLoginOK = true;
				}
				else
				{
					m_Client.m_bSend = true;	
				}
			}			
		}
		break;
	case WM_CHAR:			
		break;
	}
	return -1;
}

bool Sample::Init()
{	
	m_Client.Init();	
	return true;
}
bool Sample::Frame()
{
	m_Client.ProcessPacket();
	m_Client.Frame();	
	return true;
}
bool Sample::Release()
{
	m_Client.Release();
	return true;
}
Sample::Sample(void)
{
}

Sample::~Sample(void)
{
}
INT WINAPI wWinMain(   HINSTANCE hInst, 
								HINSTANCE hPreInst, 
								LPWSTR strCmdLine, 
								INT nShow )
{
	Sample server;
	// 윈도우 생성 담당
	if( !server.SetWindow(hInst,_T("ChatClient_0"),  800, 600 ) )
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