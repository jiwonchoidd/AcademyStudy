#pragma once
#include "TPacketPool.h"
#include "TProtocolChat.h"

class TChatPacketPool : public TPacketPool
{
public:
	void		ProcessWork(T_PACKET* pUserData);
public:
	TChatPacketPool(void);
	~TChatPacketPool(void);
};

