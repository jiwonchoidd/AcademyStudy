#pragma once
#include "TUser.h"
class TChatUser : public TUser
{
public:
	void AddPacket(T_PACKET addPacket );
	void DeleteUser( SOCKET socket );
public:
	TChatUser(void);
	~TChatUser(void);
};

