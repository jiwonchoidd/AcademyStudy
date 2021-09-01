#include "TAcceptor.h"
#include "TUser.h"
#include "TServer.h"
#include "stdio.h"
bool TAcceptor::Release()
{
	return true;
}
bool TAcceptor::Run()
{
	SOCKET clientSocket;
	SOCKADDR_IN clientaddr;
	int   addrlen;
	while(m_bStarted)
	{
		addrlen = sizeof( clientaddr );
		clientSocket = accept( m_ListenSock,
			(SOCKADDR*)&clientaddr, &addrlen );
		if( clientSocket == INVALID_SOCKET )
		{
			continue;
		}
		AddUser(clientSocket,clientaddr);
		Sleep(10);
	}
	return true;
}
void TAcceptor::AddUser(SOCKET socket, SOCKADDR_IN address )
{
	
}
bool TAcceptor::Set( int iPort, 
					const char* strAddress)
{
	int iRet;
	WSADATA wd;
	WSAStartup( MAKEWORD(2,2), &wd);
	/////////////////////////////////
	//
	/////////////////////////////////
	m_ListenSock = socket( AF_INET, SOCK_STREAM, 0 );
	if( m_ListenSock == INVALID_SOCKET )
	{
		return false;
	}
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	if( strAddress == NULL )
	{
		serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	}else
	{
		serveraddr.sin_addr.s_addr = 
			inet_addr(strAddress);
	}
	serveraddr.sin_port = htons(iPort);
	iRet = bind( m_ListenSock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if( iRet == SOCKET_ERROR ) return false;

	iRet = listen( m_ListenSock, SOMAXCONN );
	if( iRet == SOCKET_ERROR ) return false;
	
	// 쓰레드 생성 
	CreateThread();
	return true;
}
TAcceptor::TAcceptor(void)
{
}


TAcceptor::~TAcceptor(void)
{
}
