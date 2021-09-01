#pragma once
#include "TStd.h"
#include <process.h>
#include "TProtocol.h"
#include "TStreamPacket.h"



class TClient 
{
public:
	TStreamPacket		m_StreamPacket;
	int					m_iSocket;
	unsigned long 		m_hSendThread;
	unsigned long 		m_hReceiveThread;
	CRITICAL_SECTION	m_CS;
	HANDLE				m_Mutex;
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
	int			ProcessPacket();
public:
	TClient(void);
	~TClient(void);
};
