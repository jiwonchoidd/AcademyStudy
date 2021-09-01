#pragma once
#include "TUser.h"


class TServer: public TSingleton<TServer>, public TServerObj
{
private:
	friend class TSingleton<TServer>;
public:
	HANDLE									m_hMutexPacket;
	TAcceptor								m_Acceptor;
	TNpcManager								m_NpcMgr;
	TStreamPacket							m_StreamPacket;	
	TActiveCharacter						m_UserCharacterList; // active
	TTargetPointMgr							m_tpMgr;
	std::map<tGUID,TUser*, GUIDComparer>	m_UserList;
	typedef std::map<tGUID,TUser*, GUIDComparer>::iterator	m_ListItor;	
	typedef void(TServer::*CallFuction)(T_PACKET& t);
	std::map<int, CallFuction>				m_fnExecutePacket;
public:
	virtual bool		Init();
	virtual bool		Frame();
	virtual bool		Render();
	virtual bool		Release();
	virtual void		ProcessPacket();
	virtual void		AddPacket(T_PACKET& pack);
	virtual int			SendPacket(TUser* pUser, UPACKET& PacketMsg);
	virtual int			SendPacket(TUser* pUser, stringstream & SendStream);
	void				SendPacket(TUser * pUser, int iSendByte);
	virtual void		Broadcast(T_PACKET& pSendUser);
	virtual void		Broadcast(UPACKET& pSendUser);
	void				Broadcast(stringstream & SendStream);
	void				Broadcast(tGUID tGuidExcept, stringstream & SendStream);
public:
	virtual TUser*		GetUser( int iIndex );
	virtual bool		DelUser( int iIndex );
	virtual bool		DelUser(tGUID tGuid);
	virtual void        AddUser(SOCKET sock);
	virtual void		ReqVerification(T_PACKET& t);
	virtual void		BroadcastNewPlayer(TCharacter& player);
	virtual void		SyncAllCharacters(TUser* pUser);
	virtual void		SendDeadPlayer(TUser* pUser);
	virtual void        SendLogoutPlayer(tGUID tGuid);
public:
	virtual void		MoveAction(T_PACKET& t);
	virtual void		Msg(T_PACKET& t);
	virtual void		AckChatName(T_PACKET& t);
	virtual void		ReqChatName(T_PACKET& t);
	virtual void		CreateAccount(T_PACKET& pSendUser);
	virtual void		DeleteAccount(T_PACKET& pSendUser);
	virtual void		Login(T_PACKET& pSendUser);
	virtual void		Logout(T_PACKET& pSendUser);
	virtual void		CreateCharacter(T_PACKET& pSendUser);
	virtual void		DeleteCharacter(T_PACKET& pSendUser);
	virtual void		HitCharacter(T_PACKET& pSendUser);
	virtual void		HitMonster(T_PACKET& pSendUser);
	virtual void		ReqZoneEntry(T_PACKET& pSendUser);
	virtual void		RecvZoneTargetPoint(T_PACKET& pSendUser);
	virtual void		AttackCharacter(T_PACKET& pSendUser);
	virtual void		AttackMonster(T_PACKET& pSendUser);
	virtual void		DeadCharacters(T_PACKET& pSendUser);
	virtual void		DeadMonster(T_PACKET& pSendUser);
	virtual void		DamageCharacter(T_PACKET& pSendUser);
	virtual void		DamageMonster(T_PACKET& pSendUser);
	virtual void		SpawnCharacters(T_PACKET& pSendUser);
	virtual void		SpawnMonster(T_PACKET& pSendUser);
	virtual void		SyncCharacters(T_PACKET& pSendUser);
	virtual void		SyncMonster(T_PACKET& pSendUser);
public:
	void SpawnMonsterList();
	void PrintGuid(tGUID& guid, const char* msg);
public:
	friend ostream& operator<<(ostream &stream, TServer& info)
	{
		stream << info.m_UserList.size();
		for (auto& user : info.m_UserList)
		{
			stream << user.first;
			stream << *user.second;
		}
		return stream;
	}

	friend istream &operator>>(istream &stream, TServer& info)
	{
		size_t iNumUsers = 0;
		tGUID guid;
		TUser* pUser = nullptr;
		SAFE_NEW(pUser, TUser);
		info.m_UserList.clear();

		stream >> iNumUsers;
		for (int i = 0; i < iNumUsers; i++)
		{
			stream >> guid;
			stream >> *pUser;
			info.m_UserList[guid] = pUser;
		}
		return stream;
	}
public:
	TServer(void);
	virtual ~TServer(void);
};
#define I_Server TServer::GetInstance()