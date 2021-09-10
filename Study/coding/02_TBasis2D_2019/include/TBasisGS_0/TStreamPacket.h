#pragma once
#include "TLock.h"
#include "TProtocol.h"
#define MAX_RECV_STREAM_SIZE 2048
class TUser;

// 어떤 메세지인지 구분해야 한다. ( 필요성 )
typedef struct _OVERLAPPED2 : OVERLAPPED
{
	enum { MODE_RECV = 0, MODE_SEND = 1, MODE_ZERO_SEND, MODE_ZERO_RECV, };
	int		m_iFlags;
	_OVERLAPPED2()
	{
		m_iFlags = MODE_ZERO_RECV;
	}

}OVERLAPPED2;

typedef struct {
	UPACKET			packet;
	WSABUF			buffer;
	OVERLAPPED2		ov;
	SOCKET          sock;
	TUser*			pUser;
}T_PACKET, *P_TPACKET;

typedef std::list<T_PACKET>::iterator TPacketIterator;

class TStreamPacket : public TServerObj
{
public:
	UPACKET*			m_pPacket;
	char				m_RecvBuffer[MAX_RECV_STREAM_SIZE];
	DWORD				m_iStartPos;
	DWORD				m_iWritePos;
	DWORD				m_iReadPos;
public:
	bool    Put(char* redvBuffer, int iSize, TUser* = NULL);
	void    Reset();
public:
	TStreamPacket();
	virtual ~TStreamPacket();
};
