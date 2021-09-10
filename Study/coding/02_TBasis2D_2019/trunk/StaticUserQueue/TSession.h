#pragma once
#include "TServerObj.h"
class TSession : public TServerObj
{
public:
	SOCKET			m_Sock;
	SOCKADDR_IN		m_addr;
public:
	TSession();
	virtual ~TSession();
};
class TBaseUser : public TSession
{
public:
	virtual void  Create() = 0;
	virtual void  Destory() = 0;
	virtual void  SetSocket(SOCKET sock,SOCKADDR_IN  client) = 0;
public:
	TBaseUser() {}
	virtual ~TBaseUser() {}
};



