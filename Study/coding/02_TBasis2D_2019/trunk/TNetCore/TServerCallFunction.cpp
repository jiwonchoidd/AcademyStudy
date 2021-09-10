#include "TServer.h"
//void TServer::SyncNpc(TPacket& t)
//{
//	m_SendBroadcastPacketPool.AddPacket(t);
//}
void TServer::ChatMsg(TPacket& t)
{
	TChatMsg* pMsg = (TChatMsg*)&t.packet.msg;
	printf("\n[%s]%s:%d", pMsg->szName,
		pMsg->buffer, pMsg->iCnt);
	m_SendBroadcastPacketPool.AddPacket(t);	
}
void TServer::MoveAction(TPacket& t)
{
	TEnemyPos* pMsg = (TEnemyPos*)&t.packet.msg;
	t.pUser->m_info.movement = *pMsg;
	m_SendBroadcastPacketPool.AddPacket(t);	
}
void TServer::AttackAction(TPacket& t)
{
	TEnemyPos* pMsg = (TEnemyPos*)&t.packet.msg;
	t.pUser->m_info.movement = *pMsg;
	m_SendBroadcastPacketPool.AddPacket(t);
}
void TServer::CreateAccount(TPacket & t)
{
	TLogin* login = (TLogin*)t.packet.msg;
	T_STR szInUserID = to_mw(login->szID);
	T_STR szInUserPS = to_mw(login->szPS);
	T_STR szID;
	T_STR szPS;

	bool bRet = m_Query.ExecAccount(
		szInUserID.c_str(),
		szInUserPS.c_str());

	TNetResult ret;
	ZeroMemory(&ret, sizeof(TNetResult));
	ret.id = 999;
	if (bRet)
	{
		ret.iRet = 1;
	}
	else
	{
		ret.iRet = 0;
	}

	UPACKET sendPacket;
	MakePacket(sendPacket, (char*)&ret, sizeof(TNetResult),
		PACKET_CREATE_ACCOUNT_ACK);
	if (t.pUser != nullptr)
	{
		TPacket tPacket = t;
		tPacket.pUser = t.pUser;
		tPacket.packet = sendPacket;
		m_SendPacketPool.AddPacket(tPacket);
	}
}
void TServer::Login(TPacket& t)
{
}
void TServer::ZoneEntryReq(TPacket& t)
{	
	{
		UPACKET sendPacket;
		MakePacket(sendPacket, nullptr, 0, PACKET_ZONE_ENTRY_ACK);		
		if (t.pUser != nullptr)
		{
			t.pUser->m_dwCurrentServer = TServer::TSERVER_ZONE;
			TPacket tPacket = t;
			tPacket.pUser = t.pUser;
			tPacket.packet = sendPacket;
			m_SendPacketPool.AddPacket(tPacket);
		}
	}

	// other	
	{
		ZeroMemory(&t.pUser->m_info, sizeof(TPACKET_CHARACTER_INFO));
		t.pUser->m_info.movement.p[0] = 50 + (rand() % 600);
		t.pUser->m_info.movement.p[1] = 500;
		t.pUser->m_info.movement.index = t.pUser->m_Sock;

		UPACKET sendPacket;
		MakePacket(sendPacket, (char*)&t.pUser->m_info, sizeof(TPACKET_CHARACTER_INFO), PACKET_ZONE_NEW_PLAYER);
		if (t.pUser != nullptr)
		{
			TPacket tPacket = t;
			tPacket.pUser = t.pUser;
			tPacket.packet = sendPacket;
			m_SendBroadcastPacketPool.AddPacket(tPacket);
		}
	}	
}
void TServer::ZoneEntry(TPacket& t)
{		
	// client
	{
		TLock lock((TServerObj*)m_pSessionMgr);
		int iNumUsers = (int)m_pSessionMgr->m_ZoneUserList.size();
		UPACKET sendPacket;
		TPACKET_CHARACTER_INFO* info = 
			new TPACKET_CHARACTER_INFO[iNumUsers];


		std::map<SOCKET, TNetUser*>::iterator iter;		
		int iCnt = 0;
		for (iter = m_pSessionMgr->m_ZoneUserList.begin();
			 iter != m_pSessionMgr->m_ZoneUserList.end();
			 iter++)
		{
			TNetUser* pUser = iter->second;	
			info[iCnt++] = pUser->m_info;				
		}
		MakePacket(sendPacket, (char*)info, 
			sizeof(TPACKET_CHARACTER_INFO)*iNumUsers,
			PACKET_ZONE_PLAYERS_INFO);
		if (t.pUser != nullptr)
		{
			TPacket tPacket = t;
			tPacket.pUser = t.pUser;
			tPacket.packet = sendPacket;
			tPacket.packet.ph.iotype = iNumUsers;
			m_SendPacketPool.AddPacket(tPacket);
		}
		delete info;
	}	
}
void TServer::LogoutUser(TPacket& t)
{
	m_SendBroadcastPacketPool.AddPacket(t);
	m_pSessionMgr->DelUser(t.pUser);
}
