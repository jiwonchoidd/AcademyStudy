#include "TSessionMgr.h"
#include "TServer.h"
void TSessionMgr::Init(TServer* pServer)
{
	m_pServer = pServer;
	//CreateThread();
}
bool TSessionMgr::Run()
{
	while (m_bStarted)
	{
		if (WaitForSingleObject(TServer::g_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
		Sleep(0);
	}
	Sleep(1);
	return true;
}
void TSessionMgr::AddUser(TNetUser * pUser)
{
	{
		TLock lock(this);
		m_ZoneUserList[pUser->m_Sock] = pUser;
		m_pServer->SetBind(pUser->m_Sock, (ULONG_PTR)pUser);
	}
}
bool TSessionMgr::DelUser(TNetUser* user)
{
	{
		TLock lock(this);
		printf("\nÇØÁ¦->%s:%d",
			inet_ntoa(user->m_SockAddr.sin_addr),
			ntohs(user->m_SockAddr.sin_port));

		std::map<SOCKET, TNetUser*>::iterator iter = m_ZoneUserList.find(user->m_Sock);
		if (iter != m_ZoneUserList.end())
		{
			CloseUser(user);
			m_ZoneUserList.erase(iter);
		}	
	}
	return true;
}
bool TSessionMgr::CloseUser(TNetUser* user)
{
	if (user == nullptr) return true;
	shutdown(user->m_Sock, SD_SEND);
	closesocket(user->m_Sock);
	delete user;
	user = nullptr;
	return true;
}
TSessionMgr::TSessionMgr()
{
	
}
TSessionMgr::~TSessionMgr()
{
}
