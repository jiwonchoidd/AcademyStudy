#pragma once

#include "TPacketPool.h"
class TUser : public TThread
{
public:
	TStreamPacket m_StreamPacket;
	SOCKET		  m_Socket;
	C_STR  m_szName;
	SOCKADDR_IN   m_UserAddress;
public:
	bool Create();
	bool Run();
	virtual void AddPacket(T_PACKET& addPacket );
	virtual void DeleteUser( SOCKET socket );
public:
	TUser(void);
	~TUser(void);
};

