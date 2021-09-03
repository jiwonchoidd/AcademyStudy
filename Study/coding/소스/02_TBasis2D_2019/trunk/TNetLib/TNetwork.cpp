#include "TNetwork.h"
bool TNetwork::g_bConnect = false;
bool TNetwork::MakePacket(UPACKET& packet,
						 char* msg, int iLen, uint16_t type)
{
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	packet.ph.time = time(NULL);
	memcpy(&packet.msg, msg, iLen);
	return true;
}
int TNetwork::SendMsg(SOCKET sock, char* msg, int iLen, uint16_t type)
{
	UPACKET packet;
	MakePacket(packet,msg, iLen, type);

	int iSendSize = 0;
	int iRet = 0;
	while (iSendSize < packet.ph.len)
	{
		iRet = send(sock, (char*)&packet,
			packet.ph.len - iSendSize, 0);
		if (iRet == 0 || iRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
			return 0;
		}
		iSendSize += iRet;		
	}
	return iSendSize;
}
bool TNetwork::RecvData(TNetUser& user)
{	
	int iLen = recv(user.m_Sock, &user.recvBuf[user.iRecvSize],
		PACKET_HEADER_SIZE - user.iRecvSize, 0);	
	if (iLen == 0 || iLen == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
		return true;
	}
	user.iRecvSize += iLen;
	if (user.iRecvSize == PACKET_HEADER_SIZE)
	{
		UPACKET* packet = (UPACKET*)&user.recvBuf;
		while (user.iRecvSize < packet->ph.len)
		{
			user.iRecvSize += recv(user.m_Sock, 
				&user.recvBuf[user.iRecvSize],
				packet->ph.len - user.iRecvSize, 0);
		}		
		AddPacket(user, packet);
		memset(user.recvBuf, 0, sizeof(char) * 10000);
		user.iRecvSize = 0;
	}	
	return true;
}

bool TNetwork::SendData(TNetUser& user, UPACKET& msg)
{
	while (user.iSendSize < msg.ph.len)
	{
		user.iSendSize += send(user.m_Sock,
			(char*)&msg,
			msg.ph.len - user.iSendSize, 0);
		if (user.iSendSize == 0)
		{
			return false;
		}
		if (user.iSendSize == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			user.iSendSize = 0;
			break;
		}		
	}
	if(user.iSendSize == msg.ph.len)
	{
		user.iSendSize = 0;
	}
	return true;
}
bool TNetwork::RecvUserList()
{
	EnterCriticalSection(&m_cs);
		std::list<TNetUser>::iterator iter;
		for (iter = m_UserList.begin();
			iter != m_UserList.end();
			)
		{					
			if (RecvData(*iter)== false)
			{
				DelUser(*iter);
				iter = m_UserList.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	LeaveCriticalSection(&m_cs);
	return true;
}
void TNetwork::PacketProcess()
{
	WaitForSingleObject(m_hMutex, INFINITE);
		std::vector<TPacket>::iterator senditer;
		for (senditer = m_recvPacket.begin();
			senditer != m_recvPacket.end();
			senditer++)
		{
			UPACKET* packet = (UPACKET*)&senditer->packet;
			if (packet->ph.type == PACKET_CHAT_MSG)
			{
				TChatMsg* pMsg = (TChatMsg*)&packet->msg;
				printf("\n[%s]%s:%d", pMsg->szName,
					pMsg->buffer, pMsg->iCnt);
			}
			if (packet->ph.type == PACKET_LOGIN_REQ)
			{
				UPACKET sendPacket;
				T_STR szID = L"kgca";
				T_STR szPS = L"game";
				TLogin* login = (TLogin*)packet->msg;
				T_STR szIDUser = to_mw(login->szID);
				T_STR szPSUser = to_mw(login->szPS);
				TNetResult ret;
				if (szID == szIDUser && szPS == szPSUser)
				{
					ret.iRet = 1;
				}
				else
				{
					ret.iRet = 0;
				}
				MakePacket(sendPacket, (char*)&ret, sizeof(TNetResult),
					PACKET_LOGIN_ACK);
				if (senditer->pUser!=nullptr)
				{
					senditer->pUser->m_SendPacket.push_back(sendPacket);
				}
			}
		}
	ReleaseMutex(m_hMutex);
}
bool TNetwork::Process()
{
	PacketProcess();

	EnterCriticalSection(&m_cs);		
		std::list<TNetUser>::iterator iter;
		for (iter = m_UserList.begin();
			iter != m_UserList.end();
			)
		{
			bool bDelete = false;
			std::vector<UPACKET>::iterator senditer;
			for (senditer = iter->m_SendPacket.begin();
				senditer != iter->m_SendPacket.end();
				senditer++)
			{
				if (SendData(*iter, *senditer) == false)
				{
					bDelete = true;
					break;
				}
			}
			iter->m_SendPacket.clear();

			if (bDelete == true)
			{
				DelUser(*iter);
				iter = m_UserList.erase(iter);
			}
			else
			{
				iter++;
			}
		
		}			
	LeaveCriticalSection(&m_cs);

	EnterCriticalSection(&m_cs);
		Broadcastting();
	LeaveCriticalSection(&m_cs);
	return true;
}
bool TNetwork::Broadcastting()
{
	std::list<TNetUser>::iterator iter;
	for (iter = m_UserList.begin();
		iter != m_UserList.end();
		)
	{
		bool bDelete = false;
		std::vector<TPacket>::iterator senditer;

		WaitForSingleObject(m_hMutex, INFINITE);
			for (senditer = m_recvPacket.begin();
				senditer != m_recvPacket.end();
				senditer++)
			{
				if (SendData(*iter, senditer->packet)==false)
				{
					bDelete = true;
					break;
				}
			}
			if (bDelete == true)
			{
				DelUser(*iter);
				iter = m_UserList.erase(iter);
			}
			else
			{
				iter++;
			}
		ReleaseMutex(m_hMutex);
	}
	WaitForSingleObject(m_hMutex, INFINITE);
		m_recvPacket.clear();
	ReleaseMutex(m_hMutex);
	return true;
}
bool TNetwork::InitNetwork(std::string ip, int port)
{
	InitializeCriticalSection(&m_cs);
	// 2.2 ver
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return false;
	}
	int iRet;
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

	if (InitSocket(ip, port)==false)
	{
		return false;
	}
	return true;
}
bool TNetwork::InitSocket(std::string ip, int port)
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
bool TNetwork::DeleteNetwork()
{
	m_recvPacket.clear();
	
	closesocket(m_Sock);		
	CloseHandle(m_hMutex);
	DeleteCriticalSection(&m_cs);
	WSACleanup();
	return true;
}
bool TNetwork::AddUser(SOCKET sock, SOCKADDR_IN addr)
{
	TNetUser user;
	user.m_Sock = sock;
	user.addr = addr;
	
	EnterCriticalSection(&m_cs);
		m_UserList.push_back(user);
	LeaveCriticalSection(&m_cs);

	printf("\n접속->%s:%d",
		inet_ntoa(addr.sin_addr),
		ntohs(addr.sin_port));
	return true;
}
bool TNetwork::DelUser(TNetUser& user)
{
	shutdown(user.m_Sock, SD_SEND);
	closesocket(user.m_Sock);
	printf("\n해제->%s:%d",
		inet_ntoa(user.addr.sin_addr),
		ntohs(user.addr.sin_port));
	return true;
}
void TNetwork::AddPacket(TNetUser & user, UPACKET* packet)
{
	TPacket tPacket;
	tPacket.packet = *packet;
	tPacket.pUser = &user;

	WaitForSingleObject(m_hMutex, INFINITE);
		m_recvPacket.push_back(tPacket);
	ReleaseMutex(m_hMutex);

	user.iRecvSize = 0;
}
bool TNetwork::Accept()
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
		AddUser(client, clientAddr);
	}
	return true;
}
bool TNetwork::Run()
{
	while (1)
	{
		if (Accept() == false)
		{
			break;
		}
		if (RecvUserList() == false)
		{
			break;
		}
		if (Process() == false)
		{
			break;
		}
	}
	return true;
}
TNetwork::TNetwork() {}
TNetwork::~TNetwork() {}