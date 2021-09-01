#include "TPacketPool.h"
#include "TUserManager.h"

std::list<T_PACKET>& TPacketPool::Get()
{
	return  m_PacketPool;
}
void  TPacketPool::Lock()
{
	WaitForSingleObject(m_hMutex, INFINITE);
}
void  TPacketPool::UnLock()
{
	ReleaseMutex(m_hMutex);
}

void TPacketPool::Push(T_PACKET& pack)
{
	Lock();
		m_PacketPool.push_back(pack);
	UnLock();
}


TPacketPool::TPacketPool()
{
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
}


TPacketPool::~TPacketPool()
{
	CloseHandle(m_hMutex);
}
