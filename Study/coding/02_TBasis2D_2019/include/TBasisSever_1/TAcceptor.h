#pragma once
#include "TThread.h"
#include "TPacket.h"
class TAcceptor : public TThread
{
public:
	SOCKET		m_ListenSock;
public:
	bool  Run();
	bool  Release();
	bool  Set( int iPort, const char* strAddress = 0);
	virtual void	AddUser(SOCKET socket, SOCKADDR_IN address );
public:
	TAcceptor(void);
	virtual ~TAcceptor(void);
};

