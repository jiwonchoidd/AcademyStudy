#include "TClient.h"
#include "TDebugString.h"
#include "TGameUser.h"

unsigned __stdcall sendMessage( void * arg )
{
	I_DebugStr.DisplayText("%s\r\n", "send 스레드 시작" );

	TClient* pClient = (TClient*)arg;

	while(!pClient->m_bExit)
	{		
		if( pClient->m_bLoginOK && pClient->m_bSend )
		{
			if( pClient->SendMsg(pClient->m_strBuffer, sizeof(pClient->m_strBuffer), PACKET_CHAT_MSG ) <= 0 )
			{
				break;
			}			
		}	
		Sleep(10);
	}
	return 0;
}
unsigned __stdcall receiveMessage( void * arg )
{
	I_DebugStr.DisplayText("%s\r\n", "receive 스레드 시작" );
	TClient* pClient = (TClient*)arg;
	int iSocket = pClient->m_iSocket;
	char  strBuffer[2048]= {0,};

	while(!pClient->m_bExit)
	{		
		int iRet = recv( iSocket, strBuffer, sizeof(strBuffer), 0);
		if( iRet <= 0 ) 
		{
			//비동기 소켓
			if( WSAGetLastError() == WSAEWOULDBLOCK )
			{
				continue;
			}

			pClient->m_bExit = true;
			closesocket(pClient->m_iSocket);
			break;
		}
		strBuffer[iRet] = 0;	
		EnterCriticalSection(&pClient->m_CS);
			pClient->m_StreamPacket.Put( strBuffer, iRet );
		LeaveCriticalSection(&pClient->m_CS);
		
		Sleep(10);
	}
	return 0;
}
// 완성된 패킷에 대한 처리
int TClient::ProcessPacket()
{
	EnterCriticalSection(&m_CS);
	std::list<UPACKET>::iterator	iter;
	for( iter =  m_StreamPacket.m_PacketList.begin();
		 iter != m_StreamPacket.m_PacketList.end();
		 iter++ )
	{			
		UPACKET* pPacket = &(*iter);
		switch( pPacket->ph.type )
		{
			case PACKET_CHAT_NAME_REQ:		
			{
				I_DebugStr.DisplayText("%s\r\n", pPacket->msg );
				memset(m_strBuffer, 0, sizeof(char) * 128);
				strcpy( m_strBuffer, "홍길동");
				m_bSend = true;
				while( !m_bExit )
				{								
					if( m_bSend && SendMsg( m_strBuffer,strlen(m_strBuffer),PACKET_CHAT_NAME_ACK) )
					{
						m_bLoginOK = true;
						break;
					}					
				}
			}break;
			case PACKET_CHAT_MSG:	
			{
				I_DebugStr.DisplayText("%s\r\n", pPacket->msg );
			}break;
			case PACKET_USER_POSITION:
			{
				//typedef struct {
				//	int		user_idx;
				//	BYTE	posX;
				//	BYTE	posY;
				//	BYTE	direction; //0 ~7 8방향
				//}TPACKET_USER_POSITION;
				TPACKET_USER_POSITION user;
				memcpy( &user, pPacket->msg, sizeof(char)* pPacket->ph.len - PACKET_HEADER_SIZE);
				if( user.direction ==  VK_LEFT )
				{
					I_GameUser.m_Direction = VK_LEFT;
					/*I_GameUser.m_fPosX = user.posX;
					I_GameUser.m_fPosY = user.posY;
					I_GameUser.MoveLeft(0);*/
				}
				if( user.direction ==  VK_RIGHT )
				{
					I_GameUser.m_Direction = VK_RIGHT;
					/*I_GameUser.m_fPosX = user.posX;
					I_GameUser.m_fPosY = user.posY;
					I_GameUser.MoveRight(0);*/
				}
				if( user.direction ==   VK_UP  )
				{
					I_GameUser.m_Direction = VK_UP;
					/*I_GameUser.m_fPosX = user.posX;
					I_GameUser.m_fPosY = user.posY;
					I_GameUser.MoveUp(0);*/
				}
				if( user.direction ==  VK_DOWN )
				{
					I_GameUser.m_Direction = VK_DOWN;
					/*I_GameUser.m_fPosX = user.posX;
					I_GameUser.m_fPosY = user.posY;
					I_GameUser.MoveDown(0);*/
				}

			}break;
		}
	}
	m_StreamPacket.m_PacketList.clear();
	LeaveCriticalSection(&m_CS);
	return 1;
}
int TClient::SendMsg( char* pMsg, int iSize, WORD code )
{
	UPACKET sendmsg;
	ZeroMemory( &sendmsg, sizeof(sendmsg) );
	//sprintf(sendmsg.msg, pMsg);
	memcpy( sendmsg.msg, pMsg, iSize );
	sendmsg.ph.type = code;
	sendmsg.ph.len	= PACKET_HEADER_SIZE + iSize;
	int iSend = 0;
	int sendbytes = 0;
	if (sendmsg.ph.len == 0)
	{
		int kkk = 0;
	}
	do {
		iSend = send(m_iSocket, (char*)&sendmsg, sendmsg.ph.len, 0);
		if (iSend <= 0)
		{
			return 0;
		}
		sendbytes += iSend;
	} while (sendbytes < sendmsg.ph.len);
	m_bSend = false;		
	return 1;
}
int TClient::CreateConnectSocket( int iPort)
{
	int     rv;
	m_iSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if( m_iSocket == INVALID_SOCKET )
	{
		return -1;
	}

	

	SOCKADDR_IN serveraddr;
	ZeroMemory( &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family	= AF_INET;
	serveraddr.sin_addr.s_addr= inet_addr("127.0.0.1");
	serveraddr.sin_port		= htons(iPort);
	rv = connect(m_iSocket,(sockaddr*)&serveraddr, sizeof(serveraddr));
	if( rv == SOCKET_ERROR )
	{
		return -1;
	}	

	//struct linger
	//{
	//	int l_onoff;   //linger 옵션 사용여부
	//	int l_linger;  //linger time
	//}
	linger optval;
	int iLength = sizeof(optval);	
	getsockopt(m_iSocket,	SOL_SOCKET,	SO_LINGER,(char*)&optval, &iLength );
	optval.l_onoff  = 1;
	optval.l_linger = 1000;
	setsockopt(m_iSocket,	SOL_SOCKET,SO_LINGER,(char*)&optval, iLength );


	int socketType1;
	int typeLen1=sizeof(socketType1);
	getsockopt(m_iSocket,	SOL_SOCKET,
							SO_TYPE,
							(char*)&socketType1,&typeLen1);

	if(socketType1==SOCK_STREAM)
		I_DebugStr.DisplayText("%s\r\n", "SOCK_STREAM." );		
	else
		I_DebugStr.DisplayText("%s\r\n", "SOCK_DGRAM" );		

	getsockopt(m_iSocket,	SOL_SOCKET,
							SO_SNDBUF,
							(char*)&socketType1,&typeLen1);
	I_DebugStr.DisplayText("%s=%d\r\n","SO_SNDBUF",socketType1 );

	getsockopt(m_iSocket,	SOL_SOCKET,
							SO_RCVBUF,
							(char*)&socketType1,&typeLen1);
	I_DebugStr.DisplayText("%s=%d\r\n", "SO_RCVBUF",socketType1 );
	
	
	// If iMode = 0, blocking is enabled; 
	// If iMode != 0, non-blocking mode is enabled.
	int iMode = 238;
	ioctlsocket(m_iSocket, FIONBIO, (u_long FAR*) &iMode);

	
	return 0;
}
bool TClient::Init()
{
	InitializeCriticalSection(&m_CS);
	int rv;
	WSADATA wsa;
	if( WSAStartup( MAKEWORD(2,2), &wsa ) != 0 )
	{
		return -1;
	}
	
	if( CreateConnectSocket( 10000 ) == 0 )
	{		
		// send thread		
		unsigned  sendThread, receiveThread;
		m_hSendThread = _beginthreadex( NULL, 0, sendMessage,
			(void*)this, 0, &sendThread);
		// recv thread
		m_hReceiveThread = _beginthreadex( NULL, 0, receiveMessage,
			(void*)this, 0, &receiveThread);		
		
	}	
	return true;
}
bool TClient::Frame()
{
	ProcessPacket();
	if( m_bExit )
	{
		I_DebugStr.DisplayText("%s\r\n", "서버가 종료되었습니다." );		
		m_bExit = false;
	}
	return true;
}
bool TClient::Release()
{		
	m_bExit = true;
	closesocket( m_iSocket );
	WaitForSingleObject((HANDLE)m_hSendThread, INFINITE );	
	WaitForSingleObject((HANDLE)m_hReceiveThread, INFINITE );	

	CloseHandle( (HANDLE)m_hReceiveThread );
	CloseHandle( (HANDLE)m_hSendThread );

	m_hReceiveThread	= 0;
	m_hSendThread		= 0;
	DeleteCriticalSection(&m_CS);
	
	WSACleanup();
	return true;
}
TClient::TClient(void)
{
	m_bSend		= false;
	m_bLoginOK	=  false;
	m_bExit		= false;
	m_hReceiveThread = 0;
	m_hSendThread = 0;
}

TClient::~TClient(void)
{
}
