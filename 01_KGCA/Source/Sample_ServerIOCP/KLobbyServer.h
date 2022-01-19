#pragma once
#include "KServer.h"
#include "KAcceptor.h"
#define MAX_WORKER_THREAD 3

//�κ� ���� �̱���

class KLobbyServer : public KServer
{
public:
	HANDLE			m_hWorkThread[MAX_WORKER_THREAD];
	/*�Ϸ���Ʈ ���� ��ü*/
	HANDLE			m_hIOCP;
	//���� �޾Ƶ帮�� ������ ������
	KAcceptor		m_Acceptor;

public:
	virtual	bool		Init(int port);
	virtual	bool		Run();
	virtual	bool		Release();
	virtual bool		AddUser(SOCKET clientSock, SOCKADDR_IN clientAddr);
};
