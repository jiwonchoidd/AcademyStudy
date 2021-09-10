#pragma once
#include "TNetwork.h"
class TAsyncSelect : public TNetwork
{
public:
	char			m_szData[2048];// = { 0, };
	TStreamPacket	m_StreamPacket;
public:
	bool		Connect(char* ip, int iPort);
	void		MsgEvent(MSG msg);
	int			RecvMsg();
	int			SendMsg();
	int			SendMsg(char* msg, WORD type);
	int			SendMsg(SOCKET sock, char* msg, WORD type);
public:
	TAsyncSelect();
	virtual ~TAsyncSelect();
};

