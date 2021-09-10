#pragma once
#include "TNetStd.h"
class TNetUser
{
public:
	SOCKET		m_Sock;
	SOCKADDR_IN addr;
	char		recvBuf[10000];
	int			iRecvSize;
	int			iSendSize;
	T_NET::T_STR		szName;
	std::vector<UPACKET>	m_SendPacket;
	TNetUser();
	virtual ~TNetUser();
};

