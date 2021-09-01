#pragma once
#include "TLock.h"
#include "Protocol.h"

class TUser;
struct T_PACKET
{
	UPACKET packet;
	TUser*  pUser;
};

class TPacketPool
{
private:
	std::list<T_PACKET>  m_PacketPool;
	HANDLE          m_hMutex;
public:
	std::list<T_PACKET>& Get();
	void Push(T_PACKET& pack);
	void  Lock();
	void  UnLock();
public:
	TPacketPool();
	virtual ~TPacketPool();
};

