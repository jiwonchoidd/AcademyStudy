#pragma once
#include "TNetwork.h"
class TEventSelect : public TNetwork
{
public:
	// 수동리셋이벤트
	HANDLE       m_EventArray[WSA_MAXIMUM_WAIT_EVENTS];
	TUser		 m_UserList[WSA_MAXIMUM_WAIT_EVENTS];
	int			 m_iCurrentEvent;
public:
	bool	InitSelectServer();
	bool	Run();
	void	RemoveSession(int iIndex);
	void	SendProc(int iIndex);
public:
	TEventSelect();
	virtual ~TEventSelect();
};

