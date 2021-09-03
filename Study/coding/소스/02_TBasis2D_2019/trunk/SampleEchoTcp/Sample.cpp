#include "Sample.h"
#include "TBasisSys.h"
// ������ ���� �Լ�
DWORD WINAPI ServerMain( LPVOID arg );
DWORD WINAPI ProcessClient( LPVOID arg );

HWND			 g_hEdit;
CRITICAL_SECTION g_CS;

HRESULT ConvertAnsiStringToWideCch( WCHAR* wstrDestination, 
									const CHAR* strSource, 
									int cchDestChar )
{
	if( wstrDestination==NULL || strSource==NULL || cchDestChar < 1 )
		return E_INVALIDARG;

	int nResult = MultiByteToWideChar( CP_ACP, 0, strSource, -1, 
										wstrDestination, cchDestChar );
	wstrDestination[cchDestChar-1] = 0;

	if( nResult == 0 )
		return E_FAIL;
	return S_OK;
}

TCHAR* GetMbtoWcs( const char* srcmsg )
{
	// ��Ƽ����Ʈ => �����ڵ� ��ȯ.
	TCHAR msg[1024] = {0, };
	ConvertAnsiStringToWideCch(msg, srcmsg, 1024);
	return msg;
}
void DisplayText( char* fmt, ...)
{
	EnterCriticalSection(&g_CS);
		va_list arg;
		va_start(arg, fmt );	
			char buf[1024+256] = {0,};
			vsprintf( buf, fmt, arg );	
			SendMessage( g_hEdit,EM_REPLACESEL,FALSE,(LPARAM)GetMbtoWcs(buf) );
		va_end(arg );
	LeaveCriticalSection(&g_CS);
}

DWORD WINAPI ServerMain( LPVOID arg )
{
	int rv;
	WSADATA wsa;
	if( WSAStartup( MAKEWORD(2,2), &wsa ) != 0 )
	{
		return -1;
	}

	SOCKET listen_sock = socket( AF_INET, SOCK_STREAM, 0 );
	if( listen_sock == INVALID_SOCKET )
	{
		return -1;
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory( &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family	= AF_INET;
	serveraddr.sin_addr.s_addr= htonl(INADDR_ANY);
	serveraddr.sin_port		= htons(9000);
	rv = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr) );
	if( rv == SOCKET_ERROR )
	{
		return -1;
	}

	rv = listen( listen_sock, SOMAXCONN );
	if( rv == SOCKET_ERROR )
	{
		return -1;
	}

	SOCKADDR_IN clientaddr;
	int			addrlen;
	SOCKET		client_sock;

	while(1)
	{
		addrlen = sizeof( clientaddr );
		client_sock = accept( listen_sock, (SOCKADDR*)&clientaddr, &addrlen );
		if( client_sock == INVALID_SOCKET )
		{
			continue;
		}

		DisplayText("[IP:%s,PORT:%d]\r\n",	inet_ntoa(clientaddr.sin_addr), 
											ntohs(clientaddr.sin_port) );

		// ���� �� 1�� ������ ����.
		DWORD dwThreadID;
		HANDLE hThread = CreateThread( NULL, 0, 
										ProcessClient, // ������ ���� �Լ� �ּҸ� ����
										(LPVOID)client_sock, // ������ �����Լ��� ���ڰ�(�ּ�����)
										0, // ������ ���۸� ����, (�ӽ� ��������) 
										&dwThreadID );
		if( hThread != NULL )
		{
			CloseHandle( hThread );// �����Լ��� ���ϵǾ߸� ��.
		}	
		Sleep(1);
	}
	// ������ ���� �Լ��� ���ϵǸ� ������� �Ҹ��.
	return 0;
}
DWORD WINAPI ProcessClient( LPVOID arg )
{
	SOCKET	client_sock  = (SOCKET)arg;
	char	buf[1024]= {0,};
	int     retval;

	while(1)
	{
		retval = recv(client_sock, buf, 1024, 0 );
		if(retval == SOCKET_ERROR)
		{		
			break;
		}
		if( retval == 0 ) 
		{
			break;
		}
		buf[retval] = 0;
		DisplayText("Message:%s\r\n", buf );

		retval = send(client_sock, buf, retval, 0 );	
		if(retval == SOCKET_ERROR)
		{			
			break;
		}	
		Sleep(1);
	}
	closesocket( client_sock );
	return 0;
}
bool Sample::Init()
{
	InitializeCriticalSection(&g_CS);
	// ������ ��Ʈ�� ������ ����
	g_hEdit = CreateWindow(	 L"edit", NULL,
							WS_CHILD | WS_VISIBLE |WS_HSCROLL|WS_VSCROLL 
							| ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY
							| ES_AUTOHSCROLL,
							0,0,
							this->m_iWidth,
							this->m_iHeight,
							g_hWnd,NULL, m_wc.hInstance, NULL );

		
	DWORD dwThreadID;
	HANDLE hThread;
	hThread = CreateThread( 
					NULL, 0, 
					ServerMain, // ������ ���� �Լ� �ּҸ� ����
					NULL, // ������ �����Լ��� ���ڰ�(�ּ�����)
					0, // ������ ���۸� ����, (�ӽ� ��������) 
					&dwThreadID );	

	if( hThread != NULL )
	{
		CloseHandle( hThread );		
	}
	return true;
}
bool Sample::Release()
{
	DeleteCriticalSection(&g_CS);
	WSACleanup();
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
	sWin.SetWindow(hInst, 800, 600, L"SampleEchoTcp");
	return 0;
}