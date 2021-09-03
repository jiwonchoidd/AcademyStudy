#include "TNpcMgr.h"
#include "TSessionMgr.h"
#include "TServer.h"
void TNpcMgr::Init(TServer* pServer)
{
	m_pServer = pServer;
	//CreateThread();

	m_BulletList.reserve(10);
	std::generate_n(std::back_inserter(m_BulletList), 10, [] { return new TBullet(); });
}
bool TNpcMgr::Run()
{
	while (m_bStarted)
	{
		if (WaitForSingleObject(TServer::g_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
		Sleep(0);
	}
	return true;
}
void TNpcMgr::AddNew(TNpc * npc)
{
	{
		TLock lock(this);
		m_List[npc->m_iIndex] = npc;
		//m_List.insert(make_pair(m_KeyGen(), npc));
	}
}
void TNpcMgr::AddDel(int id)
{
	{
		TLock lock(this);
		m_DelList.push_back(id);
	}
}
std::vector<int> TNpcMgr::copy()
{
	{
		TLock lock(this);
		return m_DelList;
	}
}
bool TNpcMgr::Del(TNpc* npc)
{
	{
		TLock lock(this);
		std::map<int, TNpc*>::iterator iter = m_List.find(npc->m_iIndex);
		if (iter != m_List.end())
		{
			Close(npc);
			m_List.erase(iter);
		}
	}
	return true;
}
bool TNpcMgr::Close(TNpc* npc)
{
	if (npc == nullptr) return true;
	delete npc;
	npc = nullptr;
	return true;
}
TNpcMgr::TNpcMgr() : m_KeyGen(0)
{

}
TNpcMgr::~TNpcMgr()
{
	TObjectPool<TNpc>::AllFree();
}
