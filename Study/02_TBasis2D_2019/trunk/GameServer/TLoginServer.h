#pragma once
#define PACKET_TEST
#include "TLoginAcceptor.h"
#include "TServer.h"
#include "TGameSessionMgr.h"
#include "TDemoGameScene.h"

class TLoginServer : public TSingleton<TLoginServer>, public TServer
{
private:
	friend class TSingleton<TLoginServer>;
public:
	TLoginAcceptor*	m_pAcceptor;
	bool			m_bRun;
	TDemoGameScene	m_SceneNpc;

public:
	virtual bool Init() override;
	virtual bool PreRun()override;
	virtual bool Run()override;
	virtual bool PostRun()override;
	virtual bool Release()override;
public:
	virtual void ZoneEntry(TPacket& t) override;	
	virtual void Login(TPacket& t) override;
	virtual void MoveAction(TPacket& t) override;
public:
	virtual void SyncNpc(TPacket& t);
	virtual void HitNpc(TPacket& t);
private:
	TLoginServer();
public:
	virtual ~TLoginServer();
};
#define I_Server TLoginServer::GetInstance()
