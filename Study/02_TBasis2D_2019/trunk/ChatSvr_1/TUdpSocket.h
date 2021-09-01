#pragma once
#include "TThread.h"
#include <ws2tcpip.h>

class TUdpSocket : public TThread
{
public:
	SOCKET		m_Socket;
	SOCKET		m_SendSocket;
	SOCKADDR_IN	m_RecvAddr;
	SOCKADDR_IN	m_SendAddr;
public:
	bool		Init();
	bool		Run();
	bool		Release();
public:
	TUdpSocket(void);
	~TUdpSocket(void);
};

