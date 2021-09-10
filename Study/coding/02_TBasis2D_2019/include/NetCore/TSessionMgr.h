#pragma once
#include "TIocp.h"
#include "TThread.h"
class TServer;
class TSessionMgr : public TThread
{
public:
	std::map<SOCKET, TNetUser*>		m_ZoneUserList;
	TServer*		m_pServer;
public:
	virtual  void Init(TServer* pServer);
	virtual  void AddUser(TNetUser* pUser);
	virtual  bool Run() override;
	virtual  bool DelUser(TNetUser* user);	
	virtual  bool CloseUser(TNetUser* user);
public:
	TSessionMgr();
	virtual ~TSessionMgr();
};

