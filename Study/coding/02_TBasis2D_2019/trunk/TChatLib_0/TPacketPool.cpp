#include "TPacketPool.h"
bool TPacketPool::AddPacket(T_PACKET& addPacket)
{
	{
		TSynchronize sync(this);
		m_PacketList.push_back(addPacket);
	}
	return true;
}
bool TPacketPool::ProcessPacket()
{
	{
		TSynchronize sync(this);
		std::list<T_PACKET>::iterator iter;
		for( iter= m_PacketList.begin();
			 iter != m_PacketList.end();
			 iter++)
		{
			T_PACKET* pUserData = (T_PACKET*)&(*iter);
			ProcessWork(pUserData);
		}
		m_PacketList.clear();
	}
	return true;
}
void TPacketPool::ProcessWork(T_PACKET* pUserData)
{
}
TPacketPool::TPacketPool(void)
{
}


TPacketPool::~TPacketPool(void)
{
}
