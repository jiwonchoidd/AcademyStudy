#pragma once
#include "TServerObj.h"
#include "TObjectPool.h"
#define  g_iRecvBufferSize  4096
#define  g_iMaxDataBufferSize  (g_iRecvBufferSize * 3)
class TNetUser;
class TServer;
struct TPacket
{
	TNetUser*  pUser;
	UPACKET packet;
};
struct OVERLAPPED2 : TObjectPool<OVERLAPPED2>
{
	enum { MODE_RECV=1, MODE_SEND=2, MODE_EXIT};
	OVERLAPPED ov;
	int	 iType; // 0:recv, 1:send
	OVERLAPPED2(int type)
	{
		memset(&ov, 0, sizeof(OVERLAPPED));
		iType = type;
	}
	OVERLAPPED2()
	{
		memset(&ov, 0, sizeof(OVERLAPPED));
		iType = MODE_RECV;
	}
};
class TNetUser
{
public:
	TServer*		m_pServer;
	DWORD			m_dwCurrentServer;
	TPACKET_CHARACTER_INFO m_info;
	bool		m_bZoneSignin;
	bool		m_bExit;
	SOCKET		m_Sock;
	SOCKADDR_IN m_SockAddr;
	char		m_szRecvBuffer[g_iRecvBufferSize];
	char		m_szSendBuffer[g_iRecvBufferSize];
	WSABUF		m_wsaRecvBuffer;
	WSABUF		m_wsaSendBuffer;
	T_STR		m_szName;
	OVERLAPPED2  m_ovRead;
	OVERLAPPED2  m_ovSend;
public:
	char	m_szDataBuffer[g_iMaxDataBufferSize];
	int		m_iPacketPos; // 其哦狼 矫累 林家
	int		m_iWritePos; // 其哦 单捞磐 历厘 林家
	int		m_iReadPos; // 其哦 单捞磐 历厘 林家
public:
	virtual bool DispatchRead(DWORD dwTrans, OVERLAPPED2* ov);
	virtual bool DispatchWrite(DWORD dwTrans, OVERLAPPED2* ov);
	OVERLAPPED2* OverlappedRecv(int type);
	OVERLAPPED2* OverlappedSend(int type, UPACKET& msg);
public:
	virtual bool WaitReceive();
	virtual bool WaitSender(UPACKET& msg);
public:
	TNetUser(TServer* pServer);
	virtual ~TNetUser();
};

