#include "TEventSelect.h"
bool TEventSelect::RemoveSession(TUser* pUser, int iEventIndex)
{
	shutdown(pUser->m_Socket, SD_BOTH);
	WSACloseEvent(m_EventArray[pUser->m_dwSessionID]);
	for (int icnt = iEventIndex; icnt < m_iCurrentEvent; icnt++)
	{
		m_EventArray[icnt] = m_EventArray[icnt + 1];
	}

	for (std::list<TUser*>::iterator itor = m_pUserList.begin(); itor != m_pUserList.end(); itor++)
	{
		TUser* pUser = (TUser*)*itor;
		if (pUser->m_dwSessionID == iEventIndex)
		{
			for (std::list<TUser*>::iterator NextUser = ++itor;
			NextUser != m_pUserList.end();	NextUser++)
			{
				TUser* pNextUser = (TUser*)*NextUser;
				if (NextUser != m_pUserList.end())
				{
					pNextUser->m_dwSessionID--;
				}
			}
			closesocket(pUser->m_Socket);
			pUser->m_isConnect = FALSE;
			pUser->m_dwSessionID = 0;
			pUser->m_Socket = INVALID_SOCKET;
			m_pUserList.erase(--itor);
			break;
		}
	}
	m_iCurrentEvent--;
	printf("\n ip=%s, port=%d 나감!",
		inet_ntoa(pUser->m_UserAddress.sin_addr),
		ntohs(pUser->m_UserAddress.sin_port));
	return true;
}
TUser* TEventSelect::GetSession()
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
bool	TEventSelect::InitSelectServer()
{
	if (TNetwork::Init() == true)
	{
		SOCKADDR_IN server;
		ZeroMemory(&server, sizeof(SOCKADDR_IN));
		server.sin_family = AF_INET;
		server.sin_port = htons(10000);
		server.sin_addr.s_addr = htonl(INADDR_ANY);
		int iRet = ::bind(m_Sock, (SOCKADDR*)&server, sizeof(server));
		if (iRet == SOCKET_ERROR)  return false;
		iRet = ::listen(m_Sock, SOMAXCONN);
		if (iRet == SOCKET_ERROR)  return false;
		u_long on = TRUE;
		iRet = ioctlsocket(m_Sock, FIONBIO, &on);
		if (iRet == SOCKET_ERROR)  return false;

		linger optval;
		int iLength = sizeof(optval);
		getsockopt(m_Sock, SOL_SOCKET, SO_LINGER, (char*)&optval, &iLength);
		optval.l_onoff = 1;
		optval.l_linger = 1000;
		setsockopt(m_Sock, SOL_SOCKET, SO_LINGER, (char*)&optval, iLength);

		m_EventArray[m_iCurrentEvent] = WSACreateEvent();
		WSAEventSelect(m_Sock, m_EventArray[m_iCurrentEvent],
			FD_ACCEPT | FD_CLOSE);
		//m_UserList[m_iCurrentEvent].m_Socket = m_Sock;
		m_iCurrentEvent = 1;
	}
	return true;
};
bool TEventSelect::Run()
{
	WSANETWORKEVENTS  NetworkEvent;
	SOCKADDR_IN	clientaddr;
	SOCKET		client_sock;
	int			addrlen;
	
	// 전체 이벤트를 관찰한다.
	int iIndex = WSAWaitForMultipleEvents(m_iCurrentEvent,	m_EventArray, 
		FALSE, // 전체 이벤트를 기달릴 것이냐? , 아니냐?
		1000, FALSE);
		//WSA_INFINITE, 	FALSE );
	if (iIndex == WSA_WAIT_FAILED) return false;
	if (iIndex == WSA_WAIT_TIMEOUT)
	{
		//printf("\n------->wait");		
		WSASetEvent(m_EventArray[1]);
		//WSAResetEvent(m_EventArray[iIndex]);
		return true;
	}
	iIndex -= WSA_WAIT_EVENT_0;
	// 네트워크 이벤트를 확인한다.
	// 중요 :  iIndex(이벤트 배열의 인덱스)를 갖고 해당 소켓을 찾아서
	//         어떤 이벤트인지 확인한다.
	int iRet = 0;
	TUser* pEventUser = NULL;
	if (iIndex > 0)
	{
		for (std::list<TUser*>::iterator itor = m_pUserList.begin();itor != m_pUserList.end();itor++)
		{
			TUser* pUser = (TUser*)*itor;
			if (pUser->m_dwSessionID == iIndex)
			{
				iRet = WSAEnumNetworkEvents(pUser->m_Socket, m_EventArray[iIndex], &NetworkEvent);
				pEventUser = pUser;
				break;
			}
		}
	}
	else
	{
		iRet = WSAEnumNetworkEvents(m_Sock, m_EventArray[iIndex], &NetworkEvent);
	}
	
	if (iRet == SOCKET_ERROR) return false;

	// 어떤 네트워크 이벤트인지 알 수 있다.
	if (NetworkEvent.lNetworkEvents & FD_ACCEPT)
	{
		// 오류 여부를 판단한다. 
		if (NetworkEvent.iErrorCode[ FD_ACCEPT_BIT] != 0)
		{
			return false;
		}
		addrlen = sizeof(clientaddr);
		client_sock = accept(m_Sock, (SOCKADDR*)&clientaddr, &addrlen);

		// 최대
		if (m_iCurrentEvent < WSA_MAXIMUM_WAIT_EVENTS)
		{
			m_EventArray[m_iCurrentEvent] = WSACreateEvent();
			WSAEventSelect(client_sock, m_EventArray[m_iCurrentEvent],	FD_READ | FD_WRITE | FD_CLOSE);
			TUser* pUser = GetSession();
			pUser->m_dwPort = ntohs(clientaddr.sin_port);
			pUser->m_Socket = client_sock;
			pUser->m_isConnect = TRUE;
			pUser->m_UserAddress = clientaddr;
			pUser->m_dwSessionID = m_iCurrentEvent;			
			m_pUserList.push_back(pUser);
			WSASetEvent(m_EventArray[0]);
			m_iCurrentEvent++;			
			printf("\n ip=%s, port=%d", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		}
	}
	// Recv
	if (NetworkEvent.lNetworkEvents & FD_READ  )
	{
		// 오류 여부를 판단한다. 
		if (NetworkEvent.iErrorCode[FD_READ_BIT] != 0)
		{
			return false;
		}		
		// recv
		char buf[1024] = { 0, };
		iRet = recv(pEventUser->m_Socket, buf, 1024, 0);// > 0)
												   // 에러 또는 클라이언트 종료
		if (iRet == SOCKET_ERROR || iRet == 0)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			else
			{
				RemoveSession(pEventUser, iIndex);
			}
		}
		else
		{
			pEventUser->m_RecvStreamPacket.Put(buf, iRet, pEventUser);	
			SendPacketList(pEventUser);
		}
	}
	// Send
	if (NetworkEvent.lNetworkEvents & FD_WRITE)
	{
		// 오류 여부를 판단한다. 			
		if (NetworkEvent.iErrorCode[FD_WRITE_BIT] != 0)
		{
			return false;
		}
		//send
		SendPacketList(pEventUser);
	}
	// close
	if (NetworkEvent.lNetworkEvents & FD_CLOSE)
	{
		// 오류 여부를 판단한다. 			
		if (NetworkEvent.iErrorCode[FD_CLOSE_BIT] != 0)
		{
			if (NetworkEvent.iErrorCode[FD_CLOSE_BIT])
			{
				if (NetworkEvent.iErrorCode[FD_CLOSE_BIT] == WSAENETDOWN)
				{
					printf("\nStopping");
				}
			}			
		}
		//CLOSE
		RemoveSession(pEventUser, iIndex);
	}	
	return true;
}
bool TEventSelect::SendPacketList(TUser* pEventUser)
{
	//send
	for (std::list<UPACKET>::iterator itor = pEventUser->m_SendPacketList.begin();
		itor != pEventUser->m_SendPacketList.end(); )
	{
		UPACKET packet = (UPACKET)*itor;
		int iRet = send(pEventUser->m_Socket, (char*)&packet, packet.ph.len+ PACKET_HEADER_SIZE, 0);
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
			pEventUser->m_SendPacketList.erase(itor++);
		}
	}
	return true;
}
TEventSelect::TEventSelect()
{
	m_iCurrentEvent = 0;
}
TEventSelect::~TEventSelect()
{
}
