#pragma once
#include "TProtocol.h"
class TUser;
#define MAX_RECV_SIZE 10000

class TStreamPacket
{
public:
	UPACKET*			m_pPacket;
	char				m_RecvBuffer[MAX_RECV_SIZE];
	int					m_iStartPos; // ��Ŷ�� ���� ��ġ
	int					m_iWritePos; // ��Ŷ�� ���� ��ġ
	int					m_iReadPos;  // ��Ŷ�� ���� ��ġ�� ���� ���� ũ��
public:
	void    Put(char* redvBuffer, int iSize, TUser* = NULL);
public:
	TStreamPacket();
	virtual ~TStreamPacket();
};

