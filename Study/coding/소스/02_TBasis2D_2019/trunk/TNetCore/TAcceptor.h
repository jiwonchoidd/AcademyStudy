#pragma once
#include "TSessionMgr.h"
class TServer;
class TAcceptor : public TThread
{
public:
	SOCKET		m_Sock;
	TServer*	m_pServer;
public:
	virtual bool Run() override;
	virtual bool Accept();
	virtual bool AddSession(SOCKET sock, SOCKADDR_IN m_SockAddr);

	virtual bool InitNetwork(std::string ip, int iPort);
	virtual bool DeleteNetwork();

	virtual bool InitSocket(std::string ip, int iPort);	
	virtual bool CloseSocket();
public:
	TAcceptor(TServer*);
	virtual ~TAcceptor();
};

