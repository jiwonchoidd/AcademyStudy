#pragma once
#include "TProtocol.h"
class TUser;
#define MAX_RECV_SIZE 10000

class TStreamPacket
{
public:
	UPACKET*			m_pPacket;
	char				m_RecvBuffer[MAX_RECV_SIZE];
	int					m_iStartPos; // 패킷의 시작 위치
	int					m_iWritePos; // 패킷의 복사 위치
	int					m_iReadPos;  // 패킷의 시작 위치로 부터 받은 크기
public:
	void    Put(char* redvBuffer, int iSize, TUser* = NULL);
public:
	TStreamPacket();
	virtual ~TStreamPacket();
};

