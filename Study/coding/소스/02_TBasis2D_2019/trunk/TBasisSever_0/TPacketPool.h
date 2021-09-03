#pragma once
#include "TThread.h"
#include "TStreamPacket.h"
class TPacketPool : public TServerObj
{
public:	
	TStreamPacket		m_StreamPacket;
	virtual bool		AddPacket(T_PACKET addPacket);
	virtual bool		ProcessPacket();
	virtual void		ProcessWork(T_PACKET* pUserData);
public:
	TPacketPool(void);
	~TPacketPool(void);
};

