#pragma once
#include "TServerStd.h"
#include "TProtocol.h"

class TUser;
typedef struct{
	UPACKET packet;
	TUser*  pUser;
}T_PACKET,*P_TPACKET;

class TStreamPacket
{
public:
	std::list<T_PACKET> m_PacketList;
	typedef std::list<T_PACKET>::iterator m_PackItor;
	char    m_strBuffer[4096];
	int	    m_iWritePos;
	int		m_iRecvPos;
	P_UPACKET		m_pPacketData;
	UPACKET			m_PacketData;
	PACKET_HEADER	m_Header;
public:
	void Put(	char* redvBuffer, 
				int iSize, 
				TUser* pUser = NULL );
	//virtual void AddPacket(T_PACKET addPacket );
public:
	TStreamPacket(void);
	~TStreamPacket(void);
};

