#pragma once

#include "TPacketPool.h"
typedef struct _OVERLAPPEDEX : OVERLAPPED
{
	enum { MODE_RECV=0, MODE_SEND=1 };
	int iFlags;
	_OVERLAPPEDEX(){
		iFlags = MODE_RECV;
	}
}OVERLAPPEDEX;

class TUser : public TServerObj
{
public:
	TStreamPacket	m_StreamPacket;
	SOCKET			m_Socket;
	C_STR			m_szName;
	SOCKADDR_IN		m_UserAddress;
	WSABUF			m_szRecvBuffer;
	WSABUF			m_szWsaBuf;
	char			m_strBuffer[4096];
	OVERLAPPEDEX	m_ov;
public:
	bool Create();
	virtual void AddPacket(T_PACKET& addPacket );
	virtual void DeleteUser( SOCKET socket );
	virtual void DeleteUser();
	virtual void WaitForPacketReceive();
	virtual void Dispatch( DWORD dwByteSize, LPOVERLAPPED ov);
public:
	TUser(void);
	~TUser(void);
};

