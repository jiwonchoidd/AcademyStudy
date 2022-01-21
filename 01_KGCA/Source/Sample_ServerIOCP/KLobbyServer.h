#pragma once
#include "KServer.h"
#define MAX_WORKER_THREAD 3

//로비 서버 싱글톤

class KLobbyServer : public KServer
{
public:
	HANDLE			m_hWorkThread[MAX_WORKER_THREAD];
	/*완료포트 관리 객체*/
	HANDLE			m_hIOCP;
	//유저 받아드리는 별도의 스레드


public:
	virtual	bool		Init(int port);
	virtual	bool		Run();
	virtual	bool		Release();
	virtual bool		AddUser(SOCKET clientSock, SOCKADDR_IN clientAddr);
};

