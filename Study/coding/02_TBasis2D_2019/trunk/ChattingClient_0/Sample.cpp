#include "Sample.h"
#include "TDebugString.h"

#define	PORT			10000
#define BUFFERSIZE		100
#define NAMESIZE		30

unsigned __stdcall sendMessage(void* arg);
unsigned __stdcall receiveMessage(void *arg);

unsigned  sendThread,receiveThread;
unsigned  long hSendThread,hReceiveThread;

int mySocket;
char myName[NAMESIZE];
TCHAR				m_SendMsg[1024] = {0,};
bool				m_bBeginChat = false;
bool				g_bEnd = false;

unsigned __stdcall receiveMessage(void *arg)
{
	int mySocket=(int)arg;
	char fromServer[2048]={0,};	   

	int strlen=0;	
	while(!g_bEnd)
	{
		strlen=recv(mySocket,fromServer,sizeof(fromServer),0);				
		if(strlen<=0) 
		{
			break;			
		}
		fromServer[strlen] = 0;
		I_DebugStr.Print("%s", fromServer );		
	}
	I_DebugStr.Print("%s", "서버가 종료되었습니다." );	
	return 0;
}

LRESULT Sample::MsgProc( HWND hWnd,
						UINT msg,
						WPARAM wParam,
						LPARAM lParam )
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch( msg )
	{
	case WM_COMMAND:
		{
			switch( LOWORD( wParam ))
			{
			case 1:
				{			
					memset( m_SendMsg, 0, sizeof(TCHAR)* 1024 );
					SendMessage( I_DebugStr.m_hEdit,WM_GETTEXT, 1024, (LPARAM)m_SendMsg );
					char toServer[2014] = {0,};
					char* strMsg = I_DebugStr.GetWcstoMbs(m_SendMsg);
					sprintf(toServer,"%s 님이 입장하셨습니다.",strMsg);
						
					int iLength = strlen(myName);
					if( iLength > 0 )
					{
						ZeroMemory( toServer, sizeof(toServer));
						sprintf(toServer,"[%s] %s",myName,strMsg);						
					}
					else
					{
						strcpy( myName, strMsg );
					}
					iLength = strlen(toServer);
					toServer[iLength] = 0;
					iLength = send(mySocket,toServer,iLength,0);

					SendMessage( I_DebugStr.m_hEdit,WM_SETTEXT, 0, 0 );
					InvalidateRect( hWnd, NULL, NULL );
					m_bBeginChat = true;
				}break;			
			}
		}
		case WM_PAINT:
		  hdc = BeginPaint(hWnd,&ps);
		  EndPaint(hWnd, &ps);
		  return 0;
		break;
	}
	return 0;
}
bool Sample::Init()
{		
	I_DebugStr.Init();
	I_DebugStr.Print("=============== 채팅==============");		
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2,2),&wsadata);
	
	/*서버에 접속해서 메시지를 주고 받는 클라이언트 소켓생성*/
	mySocket = socket( PF_INET, SOCK_STREAM, 0 );
	/*클라이언트 소켓생성에 실패하면 에러메시지를 출력하고 함수 종료*/
	if ( mySocket == -1 )
	{
		I_DebugStr.Print("클라이언트 소켓을 생성하는데 실패 했습니다.");
		return 0;
	}
	I_DebugStr.Print("클라이언트 소켓을 생성 했습니다.");
	
    /*서버의 주소 정보를 저장할 구조체 serverAddress 선언*/
	sockaddr_in serverAddress;
    memset( &serverAddress, 0,sizeof( serverAddress ) );
	serverAddress.sin_addr.s_addr	= inet_addr("127.0.0.1");
 	serverAddress.sin_family		= AF_INET;	
	serverAddress.sin_port			= htons( PORT );	

	if(connect( mySocket, (sockaddr *)&serverAddress, 	sizeof( serverAddress ) )==-1)
	{
		I_DebugStr.Print("서버와 연결하는데 실패했습니다");
		return 0;
	}

	I_DebugStr.Print("채팅을 시작합니다.");	
	hReceiveThread=_beginthreadex(NULL,0,receiveMessage,(void*)mySocket,0,&receiveThread);
	return true;
}
bool Sample::PreRender()
{	
	return true;
}
bool Sample::Render()
{
	I_DebugStr.Frame();
	return PostRender();
}
bool Sample::PostRender()
{	
	return true;
}
bool Sample::Release()
{
	if( !g_bEnd )
	{
		closesocket(mySocket);
		g_bEnd = true;
		I_DebugStr.Release();
		CloseHandle((HANDLE)hReceiveThread);
		WaitForSingleObject((HANDLE)hReceiveThread,INFINITE);
		hReceiveThread = 0;
	}
	return true;
}

Sample::Sample(void)
{
	m_bBeginChat =false;
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
	sWin.SetWindow(hInst, L"Chatting Client",500, 450 );
	sWin.Run();
	return 0;
}