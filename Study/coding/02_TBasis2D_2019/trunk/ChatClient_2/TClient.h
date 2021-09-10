#pragma once
#include "TServerStd.h"
#include <process.h>
#include "TProtocolChat.h"
#include "TUser.h"

class TClient : public TUser
{
public:
	int					m_iSocket;
	unsigned long 		m_hSendThread;
	unsigned long 		m_hReceiveThread;
	bool				m_bExit;
public:
	char		m_strBuffer[128];
	bool		m_bSend;
	bool		m_bLoginOK;
	bool		Init();
	bool		Frame();
	bool		Release();
	int			CreateConnectSocket( int iPort);
	int			SendMsg( char* pMsg, WORD code );
	int			SendMsg( char* pMsg, int iSize, WORD code );
	int			ProcessPacket();
public:
	TClient(void);
	~TClient(void);
};
