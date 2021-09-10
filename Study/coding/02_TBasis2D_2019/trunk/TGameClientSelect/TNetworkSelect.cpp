#include "TNetworkClient.h"
bool  TNetworkClientSelect::InitNetwork(std::string ip, int iPort)
{
	if (TNetwork::InitNetwork(ip.c_str(), iPort) == false)
	{
		return false;
	}
	TNetwork::g_bConnect = true;
	return true;
}
bool TNetworkClientSelect::InitSocket(std::string ip, int port)
{
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
bool	TNetworkClientSelect::Frame()
{
	if (!TNetwork::g_bConnect) return true;
	FD_ZERO(&m_rSet);
	FD_ZERO(&m_wSet);
	FD_ZERO(&m_eSet);
	FD_SET(m_Sock, &m_rSet);
	FD_SET(m_Sock, &m_wSet);
	FD_SET(m_Sock, &m_eSet);

	timeval time;
	time.tv_sec = 5;// 초
	time.tv_usec = 0; // 마이크로초 1/1000000초
	int iRet = select(0, &m_rSet, &m_wSet, &m_eSet, &time);
	if (iRet == SOCKET_ERROR) return false;
	if (iRet == 0)
	{
		return true; // timeout
	}
	if (FD_ISSET(m_Sock, &m_eSet))
	{
		TNetwork::g_bConnect = false;
		return true;
	}
	if (FD_ISSET(m_Sock, &m_rSet))
	{
		if (RecvData(m_User) == false)
		{
			TNetwork::g_bConnect = false;
			return true;
		}
	}
	if (FD_ISSET(m_Sock, &m_wSet))
	{
		return true;
	}
	return true;
}