#include "TServer.h"
#include "TDebugString.h"


// 스레드 시작 함수
DWORD WINAPI AcceptProcess( LPVOID arg );
DWORD WINAPI ProcessClient( LPVOID arg );
unsigned __stdcall BroadcastAllClient( void * arg );
unsigned __stdcall WorkerThread( void * arg );



void static T_ERROR(bool bPrint=true)
{
	if( WSAGetLastError() != WSA_IO_PENDING )
	{
		setlocale(LC_ALL,"KOREAN");
		char* lpMsgBuf;
		FormatMessageA( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER|
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char*)&lpMsgBuf, 0, NULL);
		if(bPrint)
			I_DebugStr.DisplayText("ERROR WSASend:%s\r\n",(char*)lpMsgBuf );				
		else
			OutputDebugStringA((char*)lpMsgBuf) ;
		LocalFree(lpMsgBuf);		
	}
}
DWORD WINAPI AcceptProcess( LPVOID arg )
{
	TServer* pServer = (TServer*)arg;
	if( pServer == NULL ) return 1;

	int iRet;
	SOCKADDR_IN clientaddr;
	int			addrlen;
	SOCKET		client_sock;
	while(1)
	{
		addrlen = sizeof( clientaddr );
		client_sock = accept( pServer->m_ListenSock, (SOCKADDR*)&clientaddr, &addrlen );
		if( client_sock == INVALID_SOCKET )
		{
			continue;
		}

		I_DebugStr.DisplayText("[IP:%s,PORT:%d]\r\n",	inet_ntoa(clientaddr.sin_addr), 
											ntohs(clientaddr.sin_port) );
		WaitForSingleObject( pServer->m_Mutex, INFINITE );
		
		TUser* pUser = NULL;
		SAFE_NEW( pUser, TUser );
		pUser->m_Socket = client_sock;
		WSAEVENT hEvent = WSACreateEvent();
		ZeroMemory(&(pUser->m_ov), sizeof(pUser->m_ov));
		pUser->m_ov.m_iFlags	= OVERLAPPED2::MODE_SEND;
		pUser->m_ov.hEvent = hEvent;
		pUser->m_iEvent = pServer->m_iEventCount;
		pServer->m_EventArray[pServer->m_iEventCount++] = hEvent;
		pServer->m_iClientNumber++;

		pServer->m_UserList.push_back(pUser);
		ReleaseMutex(  pServer->m_Mutex );
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
			T_ERROR();
		}

		if( WSASetEvent(pServer->m_EventArray[0])== FALSE )
		{
			break;
		}
		Sleep(1);
	}
	// 스레드 시작 함수가 리턴되면 스레드는 소멸됨.
	return 0;
}
unsigned __stdcall WorkerThread( LPVOID arg )
{
	TServer* pServer = (TServer*)arg;
	char	buf[2048]= {0,};
	int     iRet;

	while(1)
	{
		// 이벤트 객체 관찰
		DWORD index = WSAWaitForMultipleEvents(
												pServer->m_iEventCount, 
												pServer->m_EventArray,
												FALSE, 
												WSA_INFINITE, 
												FALSE);
		if(index == WSA_WAIT_FAILED)
		{
			I_DebugStr.DisplayText("%s\r\n", "오류 : WSAWaitForMultipleEvents.");				
			continue;
		}
		//index -= WSA_WAIT_EVENT_0;
		WSAResetEvent(pServer->m_EventArray[index]);
		if( index == 0 )
		{
			continue;
		}

		TUser* pUser = pServer->GetUser(index);
		if( pUser == NULL ) break;

		DWORD cbTransferred, flags;
		iRet = WSAGetOverlappedResult(
					pUser->m_Socket,
					(LPOVERLAPPED)&pUser->m_ov, //WSAOVERLAPPED
					&cbTransferred, 
					FALSE, 
					&flags );

		if( iRet == FALSE || cbTransferred == 0)
		{
			if( iRet == FALSE ) 
				I_DebugStr.DisplayText("%s%s\r\n", pUser->m_Name.c_str(),"비동기 소켓 오류.");				
			// 클라이언트 접속 종료
			pServer->DelUser(index);
			continue;			
		}
		// 패킷처리
		if(pUser->m_ov.m_iFlags == OVERLAPPED2::MODE_RECV)
		{
			pUser->m_StreamPacket.Put(	pUser->m_wsaBuffer.buf,cbTransferred, pUser);					
			// wsasend
		}else //SEND
		{
			//......
		}		

		pUser->m_ov.m_iFlags = OVERLAPPED2::MODE_RECV;
		pUser->m_wsaBuffer.buf = pUser->m_strBuffer;
		pUser->m_wsaBuffer.len	= 2048;
		iRet = WSARecv( pUser->m_Socket,
						&(pUser->m_wsaBuffer), 1, 
						&cbTransferred, &flags, 
						(LPOVERLAPPED)&pUser->m_ov, NULL );
		if( iRet == SOCKET_ERROR )
		{
			if( WSAGetLastError() != WSA_IO_PENDING )
			{
				I_DebugStr.DisplayText("%s%s\r\n", pUser->m_Name.c_str(),"WSARecv.");							
			}
		}
		I_Server.ProcessPacket();
		Sleep(10);
	}
	return 0;
}


unsigned __stdcall BroadcastAllClient( LPVOID arg )
{
	//TServer* pServer = (TServer*)arg;
	SOCKET	client_sock  = (SOCKET)arg;
	char	buf[2048]= {0,};
	int     iRet;

	while(1)
	{
		iRet = recv(client_sock, buf, 2048, 0 );
		if(iRet == SOCKET_ERROR)
		{		
			break;
		}
		if( iRet == 0 ) 
		{
			break;
		}
		buf[iRet] = 0;	

		WaitForSingleObject( I_Server.m_Mutex, INFINITE );
			std::list<TUser*>::iterator	iter;
			int iClientUser = I_Server.m_UserList.size();
			for( iter =  I_Server.m_UserList.begin();
				iter != I_Server.m_UserList.end();
				iter++ )
			{
				TUser* pUser = (TUser*)*iter;					
				if( pUser->m_Socket == client_sock )
				{
					pUser->m_StreamPacket.Put( buf, iRet, pUser);
				}
			}
		ReleaseMutex(I_Server.m_Mutex);

		I_Server.ProcessPacket();
		
		Sleep(10);
	}
	

	// 방에서 나감
	WaitForSingleObject( I_Server.m_Mutex, INFINITE );	
	std::list<TUser*>::iterator	iter;
	std::list<TUser*>::iterator	delUser;

		for( iter =  I_Server.m_UserList.begin();
			iter != I_Server.m_UserList.end();
			iter++ )
		{			
			TUser* pUser = (TUser*)*iter;
			if( pUser->m_Socket == client_sock )
			{
				I_DebugStr.DisplayText("%s%s\r\n", pUser->m_Name.c_str(),"님이 나가셨습니다.");
				delUser = iter;
				break;
			}
		}
		I_Server.m_UserList.erase(delUser);
	I_Server.m_iClientNumber--;
	ReleaseMutex(I_Server.m_Mutex);
	closesocket( client_sock );
	return 0;
}


void TServer::ProcessPacket()
{
	int retval;
	std::list<T_PACKET>::iterator	iter;
	for( iter =  m_StreamPacket.m_PacketList.begin();
		 iter != m_StreamPacket.m_PacketList.end();
		 iter++ )
	{		
		UPACKET PacketMsg;
		ZeroMemory( &PacketMsg, sizeof(PacketMsg) );

		T_PACKET* pSendUser = &(*iter);
		UPACKET* pPacket = &(pSendUser->packet);

		switch( pPacket->ph.type )
		{
			case PACKET_CHAT_NAME_ACK:		
			{					
				PacketMsg.ph.len = pPacket->ph.len;
				PacketMsg.ph.type = PACKET_CHAT_MSG;

				pPacket->msg[pPacket->ph.len-4] = 0;
				C_STR name = pPacket->msg;
				memcpy(&PacketMsg.msg, &pPacket->msg, pPacket->ph.len-4);
				
				char buffer[64] = "님이 입장하였습니다.";
				strcpy( &PacketMsg.msg[pPacket->ph.len-4], buffer );
				PacketMsg.ph.len += strlen(buffer);

				I_DebugStr.DisplayText("Message:%s\r\n", PacketMsg.msg );

				WaitForSingleObject( I_Server.m_Mutex, INFINITE );
					std::list<TUser*>::iterator	iter;
					int iClientUser = I_Server.m_UserList.size();
					for( iter =  I_Server.m_UserList.begin();
						iter != I_Server.m_UserList.end();
						iter++ )
					{
						TUser* pUser = (TUser*)*iter;	
					
						if( pUser->m_Socket != pSendUser->pUser->m_Socket )
						{
							retval = send(pUser->m_Socket, (char*)&PacketMsg, PacketMsg.ph.len, 0 );
							if(retval == SOCKET_ERROR)
							{			
								break;
							}
						}
						else
						{
							pUser->m_Name = name;
						}
					}	
				ReleaseMutex(I_Server.m_Mutex);
			}break;
			case PACKET_CHAT_MSG:	
			{
				I_DebugStr.DisplayText("Message:%s\r\n", pPacket->msg );
				WaitForSingleObject( I_Server.m_Mutex, INFINITE );
					std::list<TUser*>::iterator	iter;
					int iClientUser = I_Server.m_UserList.size();
					for( iter =  I_Server.m_UserList.begin();
						iter != I_Server.m_UserList.end();
						iter++ )
					{
						TUser* pUser = (TUser*)*iter;						
						if( pUser->m_Socket != pSendUser->pUser->m_Socket )
						{
							retval = send(pUser->m_Socket, (char*)pPacket, pPacket->ph.len, 0 );
							if(retval == SOCKET_ERROR)
							{			
								break;
							}
						}						
					}	
				ReleaseMutex(I_Server.m_Mutex);
			}break;
		}
	}
	m_StreamPacket.m_PacketList.clear();
	return;	
}
int TServer::CreateListenSocket( int iPort)
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
	serveraddr.sin_addr.s_addr= htonl(INADDR_ANY);
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
	m_EventArray[m_iEventCount++] = hEvent;
	return 0;
}
bool TServer::Init()
{	
	int iRet;
	WSADATA wsa;
	if( WSAStartup( MAKEWORD(2,2), &wsa ) != 0 )
	{
		return -1;
	}

	CreateListenSocket(9000);

	m_Mutex = CreateMutex( NULL, FALSE, _T("EditMutex"));
	
	DWORD dwThreadID;
	HANDLE hThread;
	hThread = CreateThread( 
					NULL, 0, 
					AcceptProcess, // 쓰레드 시작 함수 주소를 지정
					this, // 쓰레드 시작함수의 인자값(주소지정)
					0, // 쓰레드 시작를 지정, (임시 멈춤지정) 
					&dwThreadID );	

	// 유저 당 1개 스레드 붙임.
	unsigned int dwThread;
	int hThreadID = 
	 _beginthreadex(NULL, 0, WorkerThread,
					(LPVOID)this, 0, &dwThread );				
	
	return true;
}
bool TServer::Frame()
{
	return true;
}
bool TServer::Render()
{
	return true;
}
bool TServer::Release()
{	
	WSACleanup();	
	std::list<TUser*>::iterator	iter;
	int iClientUser = m_UserList.size();
	for( iter =  m_UserList.begin();
		iter != m_UserList.end();
		iter++ )
	{
		TUser* pUser = (TUser*)*iter;						
		SAFE_DEL( pUser );					
	}		
	m_UserList.clear();
	CloseHandle( m_Mutex );
	m_Mutex = 0;
	return true;
}


TUser* TServer::GetUser( int iIndex )
{
	WaitForSingleObject( m_Mutex, INFINITE );
	std::list<TUser*>::iterator	iter;
	int iClientUser = m_UserList.size();
	for( iter =  m_UserList.begin();
		iter != m_UserList.end();
		iter++ )
	{		
		TUser* pUser = (TUser*)*iter;					
		if( pUser->m_iEvent == iIndex )
		{
			ReleaseMutex(m_Mutex);
			return pUser;			
		}
	}
	ReleaseMutex(m_Mutex);
	return NULL;
}
bool TServer::DelUser( int iIndex )
{
	// 방에서 나감
	WaitForSingleObject( I_Server.m_Mutex, INFINITE );	
	std::list<TUser*>::iterator	iter;
	std::list<TUser*>::iterator	delUser;

	for( iter =  I_Server.m_UserList.begin();
		iter != I_Server.m_UserList.end();
		iter++ )
	{			
		TUser* pUser = (TUser*)*iter;
		if( pUser->m_iEvent == iIndex )
		{
			I_DebugStr.DisplayText("%s%s\r\n", pUser->m_Name.c_str(),"님이 나가셨습니다.");
			closesocket( pUser->m_Socket );
			SAFE_DEL( pUser );
			delUser = iter;
			break;
		}
	}		
	m_UserList.erase(delUser);
	m_iClientNumber--;
	ReleaseMutex( m_Mutex);	
	return true;
}
TServer::TServer(void)
{
	m_iClientNumber = 0;
}

TServer::~TServer(void)
{
}
