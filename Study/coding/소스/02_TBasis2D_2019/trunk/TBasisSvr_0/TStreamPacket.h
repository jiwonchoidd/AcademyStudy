#pragma once
#include "TBasisStd.h"
#include <process.h>
#include "TProtocol.h"

struct TUser;

typedef struct {
	UPACKET  packet;
	TUser*	 pUser;
}T_PACKET, *T_UPACKET;

class TStreamPacket
{
public:
	std::list<T_PACKET>						m_PacketList;
	typedef std::list<T_PACKET>::iterator	m_PackItor;
public:
	
public:
	char			m_strBuffer[4096];// Recv버퍼+ 1개의 패킷 크기
	int				m_iWritePos;
	P_UPACKET		m_pPacketData;
	int				m_iRecvPos;

	UPACKET			m_PacketData;
	int				m_iDataLength;
	PACKET_HEADER	m_Header;
	void			Put(char* strBuffer, int iRet, TUser* pUser );
public:
	TStreamPacket(void);
	~TStreamPacket(void);
};
