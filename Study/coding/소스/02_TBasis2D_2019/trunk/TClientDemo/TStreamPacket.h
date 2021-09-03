#pragma once
#include "TStd.h"
#include "TProtocol.h"
#define MAX_RECV_SIZE 10000

class TStreamPacket
{
public:
	std::list<UPACKET> m_PacketList;
	typedef std::list<UPACKET>::iterator m_PackItor;
public:
	UPACKET*			m_pPacket;
	char				m_RecvBuffer[MAX_RECV_SIZE];
	int					m_iStartPos;
	int					m_iWritePos;
	int					m_iReadPos;
public:
	void    Put(char* redvBuffer, int iSize);
public:
	TStreamPacket();
	virtual ~TStreamPacket();
};
