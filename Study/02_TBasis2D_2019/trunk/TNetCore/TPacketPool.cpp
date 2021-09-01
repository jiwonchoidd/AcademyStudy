#include "TPacketPool.h"
void	TPacketPool::Lock()
{
	WaitForSingleObject(m_hMutexPacketPool, INFINITE);
}
void	TPacketPool::Unlock()
{
	ReleaseMutex(m_hMutexPacketPool);
}
bool TPacketPool::MakePacket(UPACKET& packet,
	char* msg, int iLen, uint16_t type)
{
	packet.ph.iotype = 0;
	packet.ph.len = PACKET_HEADER_SIZE + iLen;
	packet.ph.type = type;
	packet.ph.time = time(NULL);
	memcpy(&packet.msg, msg, iLen);
	return true;
}
void	TPacketPool::AddPacket(TPacket& tPacket)
{
	Lock();
		m_list.push_back(tPacket);
		//if (tPacket.packet.ph.type == PACKET_USER_POSITION)
		//{
		//	TEnemyPos* pos = (TEnemyPos*)tPacket.packet.msg;
		//	std::cout << pos->p[2] << ":";
		//}
	Unlock();
}
void	TPacketPool::Clear()
{
	Lock();
		m_list.clear();
	Unlock();
}
TPacketPool::TPacketPool()
{
	m_hMutexPacketPool = CreateMutex(NULL, FALSE, NULL);
}

TPacketPool::~TPacketPool()
{
	CloseHandle(m_hMutexPacketPool);
}