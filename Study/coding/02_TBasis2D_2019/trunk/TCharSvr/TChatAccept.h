#pragma once
#include "TAcceptor.h"
class TChatAccept : public TAcceptor
{
public:
	void AddUser(SOCKET socket, SOCKADDR_IN address );
public:
	TChatAccept(void);
	~TChatAccept(void);
};

