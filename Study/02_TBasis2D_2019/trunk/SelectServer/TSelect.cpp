#include "TSelect.h"
#include "TLock.h"
volatile LONG   gBytesRead=0;
volatile LONG 	gBytesSent = 0;
volatile LONG 	gStartTime = 0;
volatile LONG 	gBytesReadLast = 0;
volatile LONG 	gBytesSentLast = 0;
volatile LONG 	gStartTimeLast = 0;
volatile LONG 	gCurrentConnections = 0;

bool	TSelect::InitSelectServer()
{
	//if (TNetwork::Init() == true)
	//{
	//	SOCKADDR_IN server;
	//	ZeroMemory(&server, sizeof(SOCKADDR_IN));
	//	server.sin_family = AF_INET;
	//	server.sin_port = htons(10000);
	//	server.sin_addr.s_addr = htonl(INADDR_ANY);
	//	int iRet = ::bind(m_Sock, (SOCKADDR*)&server, sizeof(server));
	//	if (iRet == SOCKET_ERROR)  return false;
	//	iRet = ::listen(m_Sock, SOMAXCONN);
	//	if (iRet == SOCKET_ERROR)  return false;
	//	u_long on = TRUE;
	//	iRet = ioctlsocket(m_Sock, FIONBIO, &on);
	//	if (iRet == SOCKET_ERROR)  return false;
	//}

	//gStartTime = gStartTimeLast = lastprint = GetTickCount();

	return true;
};
void TSelect::PrintStatistics()
{
	system("cls");
	ULONG       bps, tick, elapsed;
	tick = GetTickCount();
	elapsed = (tick - gStartTime) / 1000;
	if (elapsed == 0)		return;
	printf("\n");
	bps = gBytesSent / elapsed;
	printf("Average BPS sent: %lu [%lu]\n", bps, gBytesSent);
	bps = gBytesRead / elapsed;
	printf("Average BPS read: %lu [%lu]\n", bps, gBytesRead);

	elapsed = (tick - gStartTimeLast) / 1000;
	if (elapsed == 0)		return;

	bps = gBytesSentLast / elapsed;
	printf("Current BPS sent: %lu\n", bps);
	bps = gBytesReadLast / elapsed;
	printf("Current BPS read: %lu\n", bps);
	printf("Current Connections: %lu\n", gCurrentConnections);
	InterlockedExchange(&gBytesSentLast, 0);
	InterlockedExchange(&gBytesReadLast, 0);
	gStartTimeLast = tick;
}

bool TSelect::RemoveSession(TUser* pUser)
{
	closesocket(pUser->m_Socket);
	pUser->m_isConnect = FALSE;
	pUser->m_dwSessionID = 0;
	pUser->m_Socket = INVALID_SOCKET;
	printf("\n ip=%s, port=%d 나감!",
		inet_ntoa(pUser->m_UserAddress.sin_addr),
		ntohs(pUser->m_UserAddress.sin_port));
	return true;
}
TUser* TSelect::GetSession()
{
	for (int icnt = 0; icnt < 64; icnt++)
	{
		if (!m_SessionList[icnt].m_isConnect)
		{
			m_SessionList[icnt].m_dwSessionID = icnt;
			return &m_SessionList[icnt];
		}
	}
	return NULL;
}
bool TSelect::SendPacketList(TUser* pSendUser, TUser* pRecvUser)
{
	{
		TLock sync(this);
		TUser* pUser = pRecvUser;
		if (pUser == NULL)
		{
			pUser = pSendUser;
		}
		for (std::list<UPACKET>::iterator itor = pSendUser->m_SendPacketList.begin();
			itor != pSendUser->m_SendPacketList.end(); itor++)
		{
			UPACKET packet = (UPACKET)*itor;
			int iRet = send(pUser->m_Socket, (char*)&packet, packet.ph.len, 0);
			if (iRet == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					return false;
				}
				break;
			}
			else
			{
				gBytesSent += iRet;
				gBytesSentLast += iRet;
				pUser->m_SendPacketList.erase(itor);
				if (pUser->m_SendPacketList.size() <= 0) break;
			}			
		}
	}
	return true;
}
bool TSelect::Frame()
{
	FD_SET     rSet;
	FD_SET     wSet;
	SOCKADDR_IN	clientaddr;
	SOCKET		client_sock;
	int			addrlen;

	// (1)초기화
	FD_ZERO(&rSet);
	FD_ZERO(&wSet);
	// (2)세팅
	FD_SET( m_Sock, &rSet);
	for (std::list<TUser*>::iterator itor = m_pUserList.begin();
		itor != m_pUserList.end();
			itor++)
	{
		TUser* pUser = (TUser*)*itor;		
		FD_SET(pUser->m_Socket, &rSet);
		if (pUser->m_SendPacketList.size() > 0)
		{
			FD_SET(pUser->m_Socket, &wSet);
		}
	}
	// (3) Select함수 호출
	timeval time;
	time.tv_sec = 0;
	time.tv_usec = 1;

	int iRet = select(0, &rSet, &wSet, NULL, &time);
	if (iRet == SOCKET_ERROR)
	{		
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}		
	}
	else if (iRet == 0) // timeout
	{
		//PrintStatistics();
	}
	else
	{
		// 정상적인 처리
		// (4) 네트워크 이벤트가 누구냐?
		if (FD_ISSET(m_Sock, &rSet))
		{			
			addrlen = sizeof(clientaddr);
			client_sock = accept(m_Sock,(SOCKADDR*)&clientaddr, &addrlen);
			if (client_sock == INVALID_SOCKET)
			{
				if ( WSAGetLastError() != WSAEWOULDBLOCK)
				{
					return false;
				}				
			}
			else
			{
				{
					TLock sync(this);
					TUser* pUser = GetSession();
					pUser->m_dwPort = ntohs(clientaddr.sin_port);
					pUser->m_Socket = client_sock;
					pUser->m_isConnect = TRUE;
					pUser->m_UserAddress = clientaddr;
					printf("\n ip=%s, port=%d",
						inet_ntoa(clientaddr.sin_addr),
						ntohs(clientaddr.sin_port));
					m_pUserList.push_back(pUser);
				}				
			}
			InterlockedIncrement(&gCurrentConnections);
		}
		
		{
			TLock sync(this);
			for (std::list<TUser*>::iterator itor = m_pUserList.begin();
				itor != m_pUserList.end(); 
				itor++)
			{
				TUser* pUser = (TUser*)*itor;
				if (pUser->m_isConnect == FALSE) continue;
				if (FD_ISSET(pUser->m_Socket, &rSet))
				{
					// recv
					char buf[1024] = { 0, };
					iRet = recv(pUser->m_Socket, buf, 1024, 0);// > 0)
					// 에러 또는 클라이언트 종료
					if (iRet == SOCKET_ERROR)
					{
						if (WSAGetLastError() != WSAEWOULDBLOCK)
						{
							RemoveSession(pUser);
						}
					}
					else if (iRet == 0)
					{
						RemoveSession(pUser);
					}
					else
					{
						pUser->m_RecvStreamPacket.Put(buf, iRet, pUser);
						Broadcastting(pUser);
					}
				}				

				if (FD_ISSET(pUser->m_Socket, &wSet))
				{
					if (SendPacketList(pUser) == false)
					{
						RemoveSession(pUser);
					}
				}
			}
		}
		{
			TLock sync(this);
			for (std::list<TUser*>::iterator itor = m_pUserList.begin(); itor != m_pUserList.end(); )
			{
				TUser* pUser = (TUser*)*itor;
				if (pUser->m_isConnect == FALSE)
				{
					itor = m_pUserList.erase(itor);
				}
				else
				{
					itor++;
				}
			}

		}
	}	
	// See if we should print statistics
	if ((GetTickCount() - lastprint) > 1000)
	{
		PrintStatistics();
		lastprint = GetTickCount();
	}
	return true;
}
bool TSelect::Broadcastting(TUser* pSendUser)
{
	if (pSendUser && pSendUser->m_isConnect == FALSE)
	{
		return true;
	}
	{
		TLock sync(this);
		std::list<TUser*>::iterator	iter;
		std::list<TUser*>::iterator	delUser;

		int iClientUser = m_pUserList.size();
		for (iter = m_pUserList.begin();
			iter != m_pUserList.end();
			iter++)
		{
			TUser* pUser = (TUser*)*iter;
			if (pUser->m_isConnect == FALSE) continue;
			if (pUser != pSendUser)
			{
				for (std::list<UPACKET>::iterator itor = pSendUser->m_SendPacketList.begin();
					itor != pSendUser->m_SendPacketList.end(); itor++)
				{
					UPACKET packet = (UPACKET)*itor;
					pUser->m_SendPacketList.push_back(packet);
				}

				/*if (SendPacketList(pUser) == false)
				{
					return false;
				}*/
			}
		}
		/*if (SendPacketList(pSendUser) == false)
		{
			return false;
		}*/
	}
	return true;
}
TSelect::TSelect(SOCKET sock) : TStdSelect(sock)
{
}


TSelect::~TSelect()
{
}
