#pragma once
#include "TUser.h"
#include "TSynchronize.h"
class TNetwork : public TServerObj
{
public:
	SOCKET		m_Sock;
	bool		m_bConnect;
	bool		m_bSendEnable;
	char		m_szRecvBuffer[2048];// = { 0, };
	DWORD		m_dwRecvByte;// = 0;
	int			m_iStartByte;// = 0;	
public:
	std::list<UPACKET>  m_RecvList;
	std::list<UPACKET>  m_SendList;
public:
	bool		Init();	
	bool		Release();
	bool		NetworkProcess();
	bool		NetworkFrame();
	virtual bool		Run();
	virtual bool		Push(UPACKET& packet);
	virtual bool		PushMsg(char* msg, WORD type);
	virtual int			RecvMsg();
	virtual int			SendMsg();
	virtual int			SendMsg(char* msg, WORD type);
	virtual int			SendMsg(SOCKET sock, char* msg, WORD type);
	virtual void		MsgEvent(MSG msg) {};
	virtual bool		Connect(char* ip, int iPort);
	virtual bool		Broadcastting(UPACKET* pPacket);
public:
	TNetwork();
	virtual ~TNetwork();
};

