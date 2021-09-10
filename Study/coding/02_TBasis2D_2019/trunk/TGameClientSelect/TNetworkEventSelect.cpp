#include "TNetworkClient.h"

bool  TNetworkClientEventSelect::InitNetwork(std::string ip, int iPort)
{
	if (TNetwork::InitNetwork(ip.c_str(), iPort) == false)
	{
		return false;
	}
	return true;
}
bool TNetworkClientEventSelect::InitSocket(std::string ip, int port)
{
	m_EventArray[m_iArrayCount] = WSACreateEvent();
	WSAEventSelect(m_Sock, m_EventArray[m_iArrayCount],
		FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	m_iArrayCount++;

	unsigned long iMode = 1;
	ioctlsocket(m_Sock, FIONBIO, &iMode);
	SOCKADDR_IN sa;
	// 바이트 정렬 구조 	
	USHORT jValue = 10000;
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr(ip.c_str());
	//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
	sa.sin_port = htons(port);
	int iRet = connect(m_Sock, (SOCKADDR*)&sa, sizeof(sa));
	//iRet = WSAConnect(m_Sock, (SOCKADDR*)&sa, sizeof(sa),
	//					NULL,NULL,NULL,NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	return true;
}

// select
bool	TNetworkClientEventSelect::Frame()
{
	int iIndex = WSAWaitForMultipleEvents(m_iArrayCount, m_EventArray, FALSE,0, FALSE);
	if (iIndex == WSA_WAIT_FAILED) return false;
	if (iIndex == WSA_WAIT_TIMEOUT) return true;
	iIndex -= WSA_WAIT_EVENT_0;

	WSANETWORKEVENTS NetworkEvent;
	for (int iEvent = iIndex; iEvent < m_iArrayCount; iEvent++)
	{
		int iSignal = WSAWaitForMultipleEvents(1,&m_EventArray[iEvent], TRUE, 0, FALSE);
		if (iSignal == WSA_WAIT_FAILED)
		{
			TNetwork::g_bConnect = false;
			break;
		}
		if (iSignal == WSA_WAIT_TIMEOUT)
		{
			continue;
		}

		int iRet = WSAEnumNetworkEvents(m_Sock,
			m_EventArray[iEvent], &NetworkEvent);
		if (iRet == SOCKET_ERROR)
		{
			Check(iRet,__LINE__);
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return false;
			}
			break;
		}
		if (NetworkEvent.lNetworkEvents & FD_CONNECT)
		{
			if (NetworkEvent.iErrorCode[FD_CONNECT_BIT] != 0)
			{
				TNetwork::g_bConnect = false;
				return false;
			}
			TNetwork::g_bConnect = true;
		}
		if (NetworkEvent.lNetworkEvents & FD_READ)
		{
			if (NetworkEvent.iErrorCode[FD_READ_BIT] != 0)
			{
				TNetwork::g_bConnect = false;
				continue;
			}
			if (RecvData(m_User) == false)
			{
				TNetwork::g_bConnect = false;
				continue;
			}
		}

		if (NetworkEvent.lNetworkEvents & FD_WRITE)
		{
			if (NetworkEvent.iErrorCode[FD_WRITE_BIT] != 0)
			{
				TNetwork::g_bConnect = false;
				return false;
			}
		}

		if (NetworkEvent.lNetworkEvents & FD_CLOSE)
		{
			if (NetworkEvent.iErrorCode[FD_CLOSE_BIT] != 0)
			{
				TNetwork::g_bConnect = false;
				return false;
			}
		}
	}
	return true;
}