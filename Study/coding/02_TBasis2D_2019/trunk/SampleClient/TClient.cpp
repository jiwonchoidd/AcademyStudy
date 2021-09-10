#include "TClient.h"
#include "TDebugString.h"


unsigned __stdcall sendMessage( void * arg )
{
	I_DebugStr.DisplayText("%s\r\n", "send 스레드 시작" );

	TClient* pClient = (TClient*)arg;

	
	while(!pClient->m_bExit)
	{				
		if( pClient->m_bLoginOK && pClient->m_bSend )
		{
			if( pClient->SendMsg(pClient->m_strBuffer,PACKET_CHAT_MSG ) <= 0 )
			{
				break;
			}		
			pClient->m_bSend = false;
		}	
		
		if( SwitchToThread() == FALSE )
		{
			Sleep(1);
			// Sleep(0)은 동일한 우선순위 스레드 스위칭 되지만
			// Sleep(1)은 서로 다른 우선순위 스레드에서도 스위칭이 된다.
		}
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
		WaitForSingleObject( pClient->m_Mutex, INFINITE );
		//EnterCriticalSection(&pClient->m_CS);
		int iPacket = pClient->m_StreamPacket.Put( strBuffer, iRet );	
		//LeaveCriticalSection(&pClient->m_CS);
		ReleaseMutex(pClient->m_Mutex);
		// Sleep(1) 또는 SwitchToThread()를 사용하면
		// 우선순위가 낮은 스레드에도 스위칭이 가능하다.
		if( SwitchToThread() == FALSE )
		{
			Sleep(1);
		}
	}
	return 0;
}
// 완성된 패킷에 대한 처리
int TClient::ProcessPacket()
{
	WaitForSingleObject( m_Mutex, 0 );
	//EnterCriticalSection(&m_CS);
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
			
			}break;
			case PACKET_CHAT_MSG:	
			{
				I_DebugStr.DisplayText("%s\r\n", pPacket->msg );
			}break;
		}
	}
	m_StreamPacket.m_PacketList.clear();
	//LeaveCriticalSection(&m_CS);
	ReleaseMutex(m_Mutex);
	return 1;
}
int TClient::SendMsg( char* pMsg, WORD code )
{
	UPACKET sendmsg;
	ZeroMemory( &sendmsg, sizeof(sendmsg) );
	sprintf(sendmsg.msg, pMsg);
	sendmsg.ph.type = code;
	sendmsg.ph.len	= sizeof(PACKET_HEADER) + strlen(sendmsg.msg);
	int iRet = send( m_iSocket, (char*)&sendmsg, sendmsg.ph.len, 0);
	if( iRet <= 0 ) 
	{
		m_bSend = false;
		//비동기 소켓
		if( WSAGetLastError() == WSAEWOULDBLOCK )
		{			
			return 1;
		}		
		return 0;
	}
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
	int iMode = 1;
	ioctlsocket(m_iSocket, FIONBIO, (u_long FAR*) &iMode);	
	return 0;
}
bool TClient::Init()
{
	InitializeCriticalSection(&m_CS);
	m_Mutex = CreateMutex( NULL, FALSE, _T("TChatMutex"));
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
		
		int iLevelSend = GetThreadPriority(HANDLE(m_hSendThread));
		int iLevelRecv = GetThreadPriority(HANDLE(m_hReceiveThread));
		if(SetThreadPriority( HANDLE(m_hReceiveThread), THREAD_PRIORITY_ABOVE_NORMAL == TRUE))
		{
			iLevelRecv = GetThreadPriority(HANDLE(m_hReceiveThread));
			INT KKKK = 0;
		};
	}	
	return true;
}
bool TClient::Frame()
{
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

	// send exit wait
	WaitForSingleObject((HANDLE)m_hSendThread, INFINITE );
	// rect exit wait	
	WaitForSingleObject((HANDLE)m_hReceiveThread, INFINITE );
	//TerminateThread( (HANDLE)m_hReceiveThread, 0);
	WSACleanup();

	CloseHandle( HANDLE(m_hReceiveThread) );
	CloseHandle( HANDLE(m_hSendThread) );
	m_hReceiveThread = NULL;
	m_hSendThread = NULL;
	
	DeleteCriticalSection(&m_CS);
	CloseHandle( m_Mutex );
	m_Mutex = NULL;
	return true;
}
TClient::TClient(void)
{
	m_bSend		= false;
	m_bLoginOK	=  false;
	m_bExit		= false;
}

TClient::~TClient(void)
{
}
