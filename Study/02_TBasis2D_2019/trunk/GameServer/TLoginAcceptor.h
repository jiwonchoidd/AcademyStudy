#pragma once
#include "TAcceptor.h"
#include "TChatUser.h"

class TLoginAcceptor :	public TAcceptor
{	
public:
	virtual bool AddSession(SOCKET sock, SOCKADDR_IN addr) override;
	TLoginAcceptor(TServer*);
	virtual ~TLoginAcceptor();
};

