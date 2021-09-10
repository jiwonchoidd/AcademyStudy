#include "Sample.h"
#include "TStd.h"
#include "TDebugString.h"

LRESULT Sample::MsgProc(HWND hWnd,
					UINT msg,
					WPARAM wParam,
					LPARAM lParam)
{
	switch( msg )
	{	
	case WM_CREATE:		
		m_hEdit = CreateWindow(_T("edit"),
			NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			500, 10, 200, 25, hWnd,
			(HMENU)202, g_hInstance, NULL );
		CreateWindow(_T("button"), _T("Send"),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON
			, 700, 10, 50, 25, hWnd,
			(HMENU)302, g_hInstance, NULL );
		//I_DebugStr.Init();
		return 0;
	case WM_COMMAND:
		switch( LOWORD(wParam))
		{
			case 302:
			{				
				GetWindowText( m_hEdit, m_strBuffer, 128 );
				I_DebugStr.ConvertWideStringToAnsiCch(
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
	case WM_LBUTTONDOWN:
		{
			//HANDLE hFile = CreateFileA("a.txt", GENERIC_READ | GENERIC_WRITE, 
			//	FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0 );
			HANDLE hRead, hWrite;
			CreatePipe( &hRead, &hWrite, 0, 1024 );
			//SetHandleInformation( hFile, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT );
			SetHandleInformation( hWrite, HANDLE_FLAG_INHERIT, HANDLE_FLAG_INHERIT );
			PROCESS_INFORMATION pi= {0,};
			STARTUPINFO si={0,};
			si.cb = sizeof(si);
			si.hStdOutput= hWrite;//hFile;
			si.dwFlags = STARTF_USESTDHANDLES;
			BOOL b = CreateProcess(0, L"ping.exe www.kgcaschool.com", 0, 0, 
				TRUE, CREATE_NO_WINDOW, 0,0, &si, &pi );
			if( b )
			{
				CloseHandle(hWrite);

				DWORD len;
				while(1)
				{
					char s[4096] = {0, };
					BOOL bRet = ReadFile( hRead, s, 4096, &len, 0);
					if( bRet == FALSE ) break;
					I_DebugStr.DisplayText("%s\r\n",s);

				}
				/*WaitForSingleObject( pi.hProcess, INFINITE );
				char s[4096]= {0,};
				SetFilePointer( hFile, 0, 0, FILE_BEGIN);

				DWORD len;
				ReadFile( hFile, s, 4096, &len, 0);
				SetWindowTextA( m_hEdit, s );*/


				CloseHandle( hRead );
				CloseHandle( pi.hProcess);
				CloseHandle( pi.hThread );
			}
		}return 0;
	case WM_CHAR:			
		break;
	}
	return -1;
}

bool Sample::Init()
{	
	I_DebugStr.Init();
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
bool Sample::GameRun()
{
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
	Sample sWin;
	sWin.SetWindow(hInst, L"SampleClient", 800, 600);
	sWin.Run();
	return 0;
}