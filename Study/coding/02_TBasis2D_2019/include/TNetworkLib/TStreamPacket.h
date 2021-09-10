#pragma once
#include "TServerStd.h"
#include "Protocol.h"
class TUser;
#define MAX_RECV_SIZE 1000

enum MSG_TYPE
{
	SEND_MSG_DAMAGE=0,
	SEND_SEND_ATTACK,
};

class TStreamPacket
{
public:	
	UPACKET*			m_pPacket;
	char				m_RecvBuffer[MAX_RECV_SIZE];
	int					m_iStartPos;
	int					m_iWritePos;
	int					m_iReadPos;
public:
	void    Put(char* redvBuffer, int iSize, TUser* = NULL);
public:
	TStreamPacket();
	virtual ~TStreamPacket();
};

