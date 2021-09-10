#pragma once
#include "TAcceptor.h"
#include "TPacketPool.h"
#include <functional>
#include "TQuery.h"
class TServer : public TThread
{
public:
	enum TServerID
	{
		TSERVER_LOGIN = 0,
		TSERVER_LOBBY,
		TSERVER_ZONE,
	};
public:
	static HANDLE      g_hKillEvent;
	TIocp*		m_pIocp;
	TAcceptor*	m_pAcceptor;
	TSessionMgr* m_pSessionMgr;
	TQuery			m_Query;
public:
	TPacketPool	 m_RecvPacketPool;
	TPacketPool	 m_SendPacketPool;
	TPacketPool	 m_SendBroadcastPacketPool;
//public:
//	typedef	void (TServer::*CallFuction)(TPacket& t);
//	typedef std::map<int, CallFuction>::iterator FunctionIterator;
//	std::map<int, CallFuction>    m_fnExecutePacket;
public:
	typedef std::function<void(TPacket& t)> CallFuction;
	typedef std::map<int, CallFuction>::iterator FunctionIterator;
	std::map<int, CallFuction>    m_fnExecutePacket;	
public:
	virtual  bool MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type);
	virtual  bool SendData(TNetUser& user, UPACKET& msg);
	virtual  bool Broadcastting();
	virtual  bool PreRun();
	virtual  bool Run() override;
	virtual  bool PostRun();
	virtual  bool Init();
	virtual  bool Release();
	virtual  void SendLogoutUser(TNetUser* user);
	virtual  void SetBind(SOCKET sock, ULONG_PTR key);
public:
	virtual void ChatMsg(TPacket& t);
	virtual void MoveAction(TPacket& t);
	virtual void Login(TPacket& t);
	virtual void ZoneEntryReq(TPacket& t);
	virtual void ZoneEntry(TPacket& t);
	virtual void LogoutUser(TPacket& t);
	virtual void AttackAction(TPacket& t);	
	virtual void CreateAccount(TPacket& t);
public:
	TServer();
	virtual ~TServer();
};

