#pragma once
#include "TStreamPacket.h"
#include "TNetBase.h"

class TUser : public TNetBase
{
public:
	DWORD				m_dwSessionID;
	BOOL				m_isConnect;
	SOCKET				m_Socket;
	SOCKADDR_IN			m_UserAddress;
	T_SERVER::C_STR		m_szIP;
	DWORD				m_dwPort;
	T_SERVER::T_STR		m_szName;
	TStreamPacket		m_RecvStreamPacket;
	std::list<UPACKET>	m_RecvPacketList;
	std::list<UPACKET>	m_SendPacketList;
public:
	TUser();
	virtual ~TUser();
};

