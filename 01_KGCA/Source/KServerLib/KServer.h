#pragma once
#include "KNetwork.h"
class KServer
{
public:
	std::list<KNetworkUser> m_UserList;
	HANDLE					m_hMutex;
	KNetwork				m_Net;
public:
	int Broadcast(KNetworkUser& user);
public:
	bool		Init(int port);
	bool		Run();
	bool		Release();
};

