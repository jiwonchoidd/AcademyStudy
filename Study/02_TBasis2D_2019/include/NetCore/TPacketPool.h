#pragma once
#include "TNetUser.h"
class TPacketPool : public TServerObj
{
public:
	HANDLE						m_hMutexPacketPool;
	std::vector<TPacket>	    m_list;
public:
	void	AddPacket(TPacket& packet);
	void	Clear();
	void	Lock();
	void	Unlock();
	bool	MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type);
public:
	TPacketPool();
	virtual ~TPacketPool();
};

