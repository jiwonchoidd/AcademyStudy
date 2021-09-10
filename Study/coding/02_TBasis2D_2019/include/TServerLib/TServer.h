#pragma once
#include "TPacketPool.h"
#include "TAcceptor.h"

class TServer : public TSingleton<TServer>, public TThread
{
private:
	friend class TSingleton<TServer>;
public:
	TPacketPool  m_RecvPool;
	TPacketPool  m_SendPool;
	TAcceptor	 m_Accepter;
public:
	bool Init();
	bool Start();
	bool Run();
	bool SendPool(TUser* pUser, int type, char* data, int iSize);
	bool SendPool(T_PACKET& packet);
	bool SendPool(TUser* pUser, UPACKET& packet);
public:
	bool SendMsg(TUser* pUser, int type, char* data, int iSize);
	bool SendMsg(TUser* pUser, UPACKET& packet);
	void Broadcast(T_PACKET& tPacket);
public:
	TServer();
	virtual ~TServer();
};
#define I_Server TServer::GetInstance()
