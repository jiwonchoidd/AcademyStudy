#pragma once
#include "TNetwork.h"
class TEventSelect : public TNetwork
{
public:
	// 수동리셋이벤트
	HANDLE       m_EventArray[WSA_MAXIMUM_WAIT_EVENTS];
	TUser		 m_SessionList[WSA_MAXIMUM_WAIT_EVENTS];
	int			 m_iCurrentEvent;
	std::list<TUser*>	 m_pUserList;
public:
	bool	InitSelectServer();
	bool	Run();
	TUser*	GetSession();
	bool	RemoveSession(TUser* pUser, int iEventIndex);
	bool	SendPacketList(TUser* pEventUser);
public:
	TEventSelect();
	virtual ~TEventSelect();
};

