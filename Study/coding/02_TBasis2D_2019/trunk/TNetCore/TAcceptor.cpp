#include "TAcceptor.h"
#include "TServer.h"
bool TAcceptor::InitNetwork(std::string ip, int port)
{
	int iRet=0;
	m_Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Sock == INVALID_SOCKET)
	{
		Error("socket");
		return false;
	}
	// 소켓 타입 확인
	int sockType;
	int sockTypeLen = sizeof(int);
	getsockopt(m_Sock, SOL_SOCKET, SO_TYPE,
		(char*)&sockType, &sockTypeLen);
	if (sockType == SOCK_STREAM)
		printf("%s\r\n", "SOCK_STREAM.");
	else
		printf("%s\r\n", "SOCK_DGRAM");

	//송수신 버퍼 크기 확인	
	int iSendBuffer = 100000;
	setsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&iSendBuffer, sockTypeLen);

	getsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&sockType, &sockTypeLen);
	printf("SendBuffer=%d\n", sockType);
	getsockopt(m_Sock, SOL_SOCKET, SO_RCVBUF,
		(char*)&sockType, &sockTypeLen);
	printf("RecvBuffer=%d\n", sockType);


	int optval = 1;
	// 1개의 프로세스에서만 ip&port 재사용 가능
	// bind함수에서 오류를 얻는다.
	Check(
		setsockopt(m_Sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
		(char*)&optval, sizeof(optval)),
		__LINE__
	);
	// 다수 프로세스에서 ip&port 재사용 가능
	/*iRet = setsockopt(m_Sock, SOL_SOCKET, SO_REUSEADDR,
		(char*)&optval, sizeof(optval));
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}*/

	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	optLinger.l_onoff = 1;
	optLinger.l_linger = 1000;
	if (setsockopt(m_Sock, SOL_SOCKET, SO_LINGER,
		(char*)&optLinger, iLingerLen) != 0)
	{
		Error("SO_LINGER");
		return false;
	}

	if (InitSocket(ip, port) == false)
	{
		return false;
	}
	return true;
}
bool TAcceptor::InitSocket(std::string ip, int port)
{
	// 넌블로킹 소켓으로 전환
	unsigned long iMode = 1;
	ioctlsocket(m_Sock, FIONBIO, &iMode);

	SOCKADDR_IN sa;
	// 바이트 정렬 구조 	
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr("192.168.0.151");
	//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
	sa.sin_port = htons(port);

	//SO_REUSEADDR가 설정된 소켓이 있을 경우는 오류가 된다.
	Check(
		bind(m_Sock, (SOCKADDR*)&sa, sizeof(sa)),
		__LINE__
	);
	int iRet = listen(m_Sock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
	{
		Error("listen");
		return false;
	}
	return true;
}
bool TAcceptor::CloseSocket()
{
	closesocket(m_Sock);
	return true;
}
bool TAcceptor::DeleteNetwork()
{
	CloseSocket();	
	return true;
}
bool TAcceptor::Run()
{
	while (m_bStarted)
	{		
		if (WaitForSingleObject(TServer::g_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
		{
			TLock lock(this);
			if (Accept() == false) break;			
		}
	}
	return false;
}
bool TAcceptor::Accept()
{
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);

	SOCKET client = accept(m_Sock, (SOCKADDR*)&clientAddr, &len);
	if (client == INVALID_SOCKET)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	else
	{
		AddSession(client, clientAddr);
	}
	return true;
}
bool TAcceptor::AddSession(SOCKET sock, SOCKADDR_IN SockAddr)
{
	{
		TLock lock(this);
		TNetUser* user = new TNetUser(m_pServer);
		user->m_Sock = sock;
		user->m_SockAddr = SockAddr;
		m_pServer->m_pSessionMgr->AddUser(user);
		user->WaitReceive();
		printf("\n접속->%s:%d",
			inet_ntoa(SockAddr.sin_addr),
			ntohs(SockAddr.sin_port));	}
	return true;
}
TAcceptor::TAcceptor(TServer* pServer) : m_pServer(pServer)
{
	// 2.2 ver
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
}
TAcceptor::~TAcceptor()
{
	DeleteNetwork();
	WSACleanup();
}
