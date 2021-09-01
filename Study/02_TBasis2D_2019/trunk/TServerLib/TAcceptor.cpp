#include "TAcceptor.h"
#include "TUserManager.h"
#include "TIocp.h"
bool	TAcceptor::Set(	int iPort,
						const char* address	)
{	
	m_ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(iPort);
	if(address ==nullptr)
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
	else
		sa.sin_addr.s_addr = inet_addr(address);

	int ret = bind(m_ListenSock, (SOCKADDR*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR)
	{
		E_MSG("Server::bind");
		return false;
	}
	ret = listen(m_ListenSock, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		E_MSG("Server::listen");
		return false;
	}
	std::cout << "서버 시작!!!" << std::endl;
	
	int socketType;
	int typeLen = sizeof(socketType);
	if (getsockopt(m_ListenSock, SOL_SOCKET, SO_SNDBUF, (char*)&socketType,
		&typeLen) == SOCKET_ERROR)
	{
		E_MSG("Server::listen");
	}
	if (getsockopt(m_ListenSock, SOL_SOCKET, SO_RCVBUF, (char*)&socketType,
		&typeLen) == SOCKET_ERROR)
	{
		E_MSG("Server::listen");
	}

	CreateThread();
	return true;
}
bool TAcceptor::Run()
{
	int addlen = sizeof(SOCKADDR);
	while (1)
	{
		TUser* user = new TUser;
		user->sock = accept(m_ListenSock, (SOCKADDR*)&user->clientaddr,	&addlen);
		if (user->sock == INVALID_SOCKET)
		{
			E_MSG("Server::accept");
		}
		I_IOCP.SetSocketBind(user->sock,(ULONG_PTR)user);
		I_UserMgr.AddUser(user);
	}
	closesocket(m_ListenSock);
	return true;
}

TAcceptor::TAcceptor()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
}


TAcceptor::~TAcceptor()
{	
	WSACleanup();
}
