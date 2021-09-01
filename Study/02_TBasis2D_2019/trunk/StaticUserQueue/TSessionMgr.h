#pragma once
#include "TSession.h"
#include "TQueue.h"
#define MAX_SESSION_COUNT 100
class TSessionMgr : public TSingleton<TSessionMgr>
{
private:
	friend class TSingleton<TSessionMgr>;
	TQueue m_QueueSession;
	std::list<TServerObj*>  m_SessionList;
public:
	void Create(CRuntimeClass* pRTCMyClass = NULL);
	TBaseUser*  CreateNewSession(SOCKET sock, SOCKADDR_IN addr);
	void RemoveSession(TBaseUser* pSession);
private:
	TSessionMgr();
public:
	virtual ~TSessionMgr();
};
#define ISessionMgr  TSessionMgr::GetInstance()

