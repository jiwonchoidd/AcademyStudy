#pragma once
#include "TThread.h"
class TAcceptor : public TThread
{
public:
	SOCKET  m_ListenSock;
public:
	bool	Set(int iPort=10000, const char* address=nullptr );
public:
	virtual bool Run() override;
public:
	TAcceptor();
	virtual ~TAcceptor();
};

