#pragma once
#include "TStreamPacket.h"
#include "TPacketPool.h"

class TUser
{
public:
	SOCKET		m_Socket;
	SOCKADDR_IN m_UserAddress;
	C_STR		m_szIP;
	DWORD       m_dwPort;
	T_STR		m_szName;
	DWORD		m_dwRecvThreadID;
	HANDLE		m_hThread;
public:
	TStreamPacket  m_RecvStreamPacket;
public:
	TUser();
	virtual ~TUser();
};

