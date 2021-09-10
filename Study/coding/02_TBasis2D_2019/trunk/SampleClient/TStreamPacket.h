#pragma once
#include "TStd.h"
#include <process.h>
#include "TProtocol.h"

class TStreamPacket 
{
public:
	std::list<UPACKET>						m_PacketList;
	typedef std::list<UPACKET>::iterator	m_PackItor;
public:
	char		m_strBuffer[4096];// Recv버퍼+ 1개의 패킷 크기
	int			m_iWritePos;
	P_UPACKET	m_pPacketData;
	int			m_iRecvPos;

	UPACKET		m_PacketData;
	int		m_iDataLength;
	PACKET_HEADER m_Header;
	int	Put(char* strBuffer, int iRet);
public:
	TStreamPacket(void);
	~TStreamPacket(void);
};
