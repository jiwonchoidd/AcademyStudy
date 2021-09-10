#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TStd.h"
#include "TProtocol.h"
class TNetwork
{
public:
	SOCKET		m_Sock;
	bool		m_bConnect;
	char		m_szRecvBuffer[2048];// = { 0, };
	int			m_iRecvByte;// = 0;
	int			m_iStartByte;// = 0;
	
public:
	std::list<UPACKET>  m_RecvList;
public:
	bool		Init();
	bool		Release();
	virtual int			RecvMsg();
	virtual int			SendMsg(char* msg, WORD type);
	virtual int			SendMsg(SOCKET sock, char* msg, WORD type);
	virtual void		MsgEvent(MSG msg) {};
	virtual bool		Connect(char* ip, int iPort);
public:
	TNetwork();
	virtual ~TNetwork();
};

