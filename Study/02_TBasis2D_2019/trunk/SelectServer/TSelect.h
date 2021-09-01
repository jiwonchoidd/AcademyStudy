#pragma once
#include "TStdSelect.h"
#include "TUser.h"
class TSelect : public TStdSelect
{
public:
	TUser			     m_SessionList[64];
	std::list<TUser*>	 m_pUserList;
	
	ULONG           lastprint;
public:
	bool	InitSelectServer();
	void	PrintStatistics();
	bool	SendPacketList(TUser* pSendUser, TUser* pRecvUser=NULL);
	bool	Frame();
	TUser*	GetSession();
	bool	RemoveSession(TUser* pUser); 
	bool	Broadcastting(TUser* pSendUser);
public:
	TSelect(SOCKET sock);
	virtual ~TSelect();
};

