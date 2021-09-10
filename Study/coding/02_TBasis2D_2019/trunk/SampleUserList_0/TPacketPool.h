#pragma once
#include "TServerStd.h"
#include "TProtocol.h"
class TPacketPool : public TSingleton<TPacketPool>
{
private:
	friend class TSingleton<TPacketPool>;
public:
	std::list<UPACKET>	m_RecvPacketList;
	bool				AddPacket(UPACKET addPacket);
	bool				ProcessPacket();
	bool				ProcessWork(UPACKET addPacket);
public:
	TPacketPool();
	virtual ~TPacketPool();
};
#define I_Pool TPacketPool::GetInstance()
