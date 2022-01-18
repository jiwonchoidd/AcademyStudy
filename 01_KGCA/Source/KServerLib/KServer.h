#pragma once
#include "KNetwork.h"
#include "KServerObj.h"
//��ε�ĳ��Ʈ�� �������� ó���������
class KServer : public KServerObj
{
public:
	std::list<KNetworkUser*> m_UserList;
	KNetwork				 m_Net;
	std::list<KPacket>		 m_lPacketPool;

public:
	int Broadcast(KNetworkUser* user);
	virtual bool AddUser(SOCKET clientSock, SOCKADDR_IN clientAddr);
public:
	virtual	bool		Init(int port);
	virtual	bool		Run();
	virtual	bool		Release();

public:
	KServer();
	virtual ~KServer();
};

