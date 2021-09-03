#pragma once
#include "TThread.h"

#define BUFFERSIZE 1024
#define NAMESIZE 30

class TAcceptor : public TThread
{
public:
	SOCKET		m_ListenSock;	

public:
	bool		Run();
	bool		Set( int iPort, const char* strAddress = 0 );
	int			GetIPList(vector<string>& ipList);
	bool		Release();
public:
	TAcceptor(void);
	virtual ~TAcceptor(void);
};
