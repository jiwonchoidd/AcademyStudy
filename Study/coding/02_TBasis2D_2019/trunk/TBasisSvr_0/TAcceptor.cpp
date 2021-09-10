#include "TAcceptor.h"
#include "TDebugString.h"
#include "TServer.h"
#include "TSynchronize.h"

bool TAcceptor::Set( int iPort, const char* strAddress )
{
	int     iRet;
	m_ListenSock = socket( AF_INET, SOCK_STREAM, 0 );
	if( m_ListenSock == INVALID_SOCKET )
	{
		return -1;
	}

	int optval = 1;
	setsockopt( m_ListenSock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));

	SOCKADDR_IN serveraddr;
	ZeroMemory( &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family	= AF_INET;
	if( strAddress == NULL)
	{
		serveraddr.sin_addr.s_addr= htonl(INADDR_ANY);
	}
	else
	{
		serveraddr.sin_addr.s_addr= inet_addr(strAddress);
	}
	serveraddr.sin_port		= htons(iPort);
	iRet = bind(m_ListenSock, (SOCKADDR*)&serveraddr, sizeof(serveraddr) );
	if( iRet == SOCKET_ERROR )
	{
		return -1;
	}

	iRet = listen( m_ListenSock, SOMAXCONN );
	if( iRet == SOCKET_ERROR )
	{
		return -1;
	}

	// 추가
	WSAEVENT hEvent = WSACreateEvent();
	I_Server.m_EventArray[I_Server.m_iEventCount++] = hEvent;	
	

	CreateThread();
	return true;
}
bool TAcceptor::Run()
{
	int iRet;
	SOCKADDR_IN clientaddr;
	int			addrlen;
	SOCKET		client_sock;
	while(m_bStarted)
	{
		addrlen = sizeof( clientaddr );
		client_sock = accept( m_ListenSock, (SOCKADDR*)&clientaddr, &addrlen );
		if( client_sock == INVALID_SOCKET )
		{
			continue;
		}

		I_DebugStr.DisplayText("[IP:%s,PORT:%d]\r\n",	inet_ntoa(clientaddr.sin_addr), 
											ntohs(clientaddr.sin_port) );
		//WaitForSingleObject( pServer->m_Mutex, INFINITE );
		{
			TSynchronize  sync(this);
			TUser* pUser = NULL;
			SAFE_NEW( pUser, TUser );
			pUser->m_Socket = client_sock;
			WSAEVENT hEvent = WSACreateEvent();
			ZeroMemory(&(pUser->m_ov), sizeof(pUser->m_ov));
			pUser->m_ov.m_iFlags	= OVERLAPPED2::MODE_SEND;
			pUser->m_ov.hEvent = hEvent;
			pUser->m_iEvent = I_Server.m_iEventCount;
			I_Server.m_EventArray[I_Server.m_iEventCount++] = hEvent;
			I_Server.m_iClientNumber++;
			I_Server.m_UserList.push_back(pUser);
			//ReleaseMutex(  pServer->m_Mutex );
		
			// 환영메세지
		
			UPACKET sendmsg;
			ZeroMemory( &sendmsg, sizeof(sendmsg) );
			sprintf( sendmsg.msg, "[서버]환영합니다. 대화명을 입력해 주세여");
			sendmsg.ph.type = PACKET_CHAT_NAME_REQ;
			sendmsg.ph.len	= sizeof(PACKET_HEADER) + strlen(sendmsg.msg);
			ZeroMemory( &pUser->m_strBuffer, sizeof(char)*2048 );
			pUser->m_wsaBuffer.buf = (char*)&sendmsg;
			pUser->m_wsaBuffer.len = sendmsg.ph.len;

			DWORD dwSendByte = sendmsg.ph.len;
			iRet = WSASend(	client_sock,
							&(pUser->m_wsaBuffer),
							1,
							&dwSendByte, 
							0, 
							(LPOVERLAPPED)&pUser->m_ov, 
							NULL );	
		
			if( iRet == SOCKET_ERROR )
			{
				I_DebugStr.T_ERROR();
			}

			if( WSASetEvent(I_Server.m_EventArray[0])== FALSE )
			{
				break;
			}
		}
		Sleep(1);
	}
	// 스레드 시작 함수가 리턴되면 스레드는 소멸됨.
	return 0;
}
TAcceptor::TAcceptor(void)
{
}

TAcceptor::~TAcceptor(void)
{
}
