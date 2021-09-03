#include "TServer.h"
#include "TDebugString.h"


// 스레드 시작 함수
DWORD WINAPI AcceptProcess( LPVOID arg );
DWORD WINAPI ProcessClient( LPVOID arg );
unsigned __stdcall BroadcastAllClient( void * arg );


DWORD WINAPI AcceptProcess( LPVOID arg )
{
	TServer* pServer = (TServer*)arg;
	if( pServer == NULL ) return 1;

	int rv;
	SOCKADDR_IN clientaddr;
	int			addrlen;
	SOCKET		client_sock;
	while(1)
	{
		addrlen = sizeof( clientaddr );
		client_sock = accept( pServer->listen_sock, (SOCKADDR*)&clientaddr, &addrlen );
		if( client_sock == INVALID_SOCKET )
		{
			continue;
		}

		I_DebugStr.DisplayText("[IP:%s,PORT:%d]\r\n",	inet_ntoa(clientaddr.sin_addr), 
											ntohs(clientaddr.sin_port) );
		WaitForSingleObject( pServer->m_Mutex, INFINITE );
				TUser	user;
				user.m_Socket = client_sock;
				pServer->m_iClientNumber++;
				pServer->m_UserList.push_back(user);
		ReleaseMutex(  pServer->m_Mutex );
				// 환영메세지
			
		UPACKET sendmsg;
		ZeroMemory( &sendmsg, sizeof(sendmsg) );
		sprintf( sendmsg.msg, "[서버]환영합니다. 대화명을 입력해 주세여");
		sendmsg.ph.type = PACKET_CHAT_NAME_REQ;
		sendmsg.ph.len	= sizeof(PACKET_HEADER) + strlen(sendmsg.msg);
		send( client_sock, (char*)&sendmsg, sendmsg.ph.len, 0);

		// 유저 당 1개 스레드 붙임.
		unsigned int dwThreadID;
		int hThread = 
		_beginthreadex(NULL, 0, BroadcastAllClient,
						(LPVOID)client_sock, 0, &dwThreadID );		
		Sleep(1);
	}
	// 스레드 시작 함수가 리턴되면 스레드는 소멸됨.
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
			std::list<TUser>::iterator	iter;
			int iClientUser = I_Server.m_UserList.size();
			for( iter =  I_Server.m_UserList.begin();
				iter != I_Server.m_UserList.end();
				iter++ )
			{
				TUser* pUser = &(*iter);					
				if( pUser->m_Socket == client_sock )
				{
					pUser->m_StreamPacket.Put( buf, iRet, pUser );
					
				}
			}
		ReleaseMutex(I_Server.m_Mutex);

		I_Server.ProcessPacket();
		
		Sleep(10);
	}
	

	// 방에서 나감
	WaitForSingleObject( I_Server.m_Mutex, INFINITE );	
	std::list<TUser>::iterator	iter;
	std::list<TUser>::iterator	delUser;

		for( iter =  I_Server.m_UserList.begin();
			iter != I_Server.m_UserList.end();
			iter++ )
		{			
			TUser* pUser = &(*iter);
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
					std::list<TUser>::iterator	iter;
					int iClientUser = I_Server.m_UserList.size();
					for( iter =  I_Server.m_UserList.begin();
						iter != I_Server.m_UserList.end();
						iter++ )
					{
						TUser* pUser = &(*iter);	
					
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
					std::list<TUser>::iterator	iter;
					int iClientUser = I_Server.m_UserList.size();
					for( iter =  I_Server.m_UserList.begin();
						iter != I_Server.m_UserList.end();
						iter++ )
					{
						TUser* pUser = &(*iter);						
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
	int     rv;
	listen_sock = socket( AF_INET, SOCK_STREAM, 0 );
	if( listen_sock == INVALID_SOCKET )
	{
		return -1;
	}

	int optval = 1;
	setsockopt( listen_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));

	SOCKADDR_IN serveraddr;
	ZeroMemory( &serveraddr, sizeof(serveraddr));
	serveraddr.sin_family	= AF_INET;
	serveraddr.sin_addr.s_addr= htonl(INADDR_ANY);
	serveraddr.sin_port		= htons(iPort);
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
	return 0;
}
bool TServer::Init()
{	
	int rv;
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

	if( hThread != NULL )
	{
		CloseHandle( hThread );		
	}
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
	CloseHandle( m_Mutex );
	WSACleanup();
	m_UserList.clear();
	return true;
}
TServer::TServer(void)
{
	m_iClientNumber = 0;
}

TServer::~TServer(void)
{
}
