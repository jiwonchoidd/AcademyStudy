#pragma once
#include "TThread.h"
#include <ws2tcpip.h>

class TUdpSocket : public TThread
{
public:
	SOCKET		m_Socket;
	SOCKET		m_RecvSocket;
	SOCKADDR_IN	m_RecvAddr;
	SOCKADDR_IN	m_SendAddr;
	vector<string> m_ipList;
public:
	bool		Init();
	bool		Run();
	bool		Release();
	int			GetIPList();
public:
	TUdpSocket(void);
	~TUdpSocket(void);
};

