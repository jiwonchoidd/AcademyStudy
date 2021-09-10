#pragma once
#include "TUser.h"
class TChatUser : public TUser
{
public:
	int			m_iIndex;
	int			m_PositionX;
	int			m_PositionY;
	int			m_iDirection;
public:
	void Movement(UPACKET* pPacket);
	void AddPacket(T_PACKET& addPacket );
	void DeleteUser( SOCKET socket );
	void DeleteUser();
	void Dispatch( DWORD dwByteSize, LPOVERLAPPED ov);
public:
	TChatUser(void);
	~TChatUser(void);
};

