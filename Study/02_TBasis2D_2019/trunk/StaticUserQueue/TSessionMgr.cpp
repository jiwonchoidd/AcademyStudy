#include "TSessionMgr.h"

TBaseUser* g_pSession = 0;
template < class T> class IsObject
{
public:
	const bool operator() (T pObj)
	{
		return (pObj == g_pSession);
	}
};

void TSessionMgr::Create(CRuntimeClass* pRTCMyClass)
{
	for (int iQueue = 0; iQueue < MAX_SESSION_COUNT; iQueue++)
	{
		CObject* pObj = pRTCMyClass->CreateObject();
		m_QueueSession.Push((TServerObj*)pObj);		
	}
}

TBaseUser* TSessionMgr::CreateNewSession(SOCKET sock, SOCKADDR_IN addr)
{
	TBaseUser* newSession = 0;
	{
		newSession = dynamic_cast<TBaseUser*>(m_QueueSession.Pop());
	}
	newSession->SetSocket(sock, addr);
	m_SessionList.push_back(newSession);
	return newSession;
}
void TSessionMgr::RemoveSession(TBaseUser* pSession)
{
	{
		g_pSession = pSession;
		m_SessionList.remove_if(IsObject<TServerObj*>());
		m_QueueSession.Push((TServerObj*)pSession);
	}
}

TSessionMgr::TSessionMgr()
{
}


TSessionMgr::~TSessionMgr()
{
	m_SessionList.clear();
	while (m_QueueSession.m_List.size() > 0)
	{
		TBaseUser* pSession = (TBaseUser*)m_QueueSession.Pop();
		pSession->Destory();
		delete pSession;
	}
}
