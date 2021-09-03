#include "TLoginServer.h"
#include "TIocp.h"
#include "TNpcMgr.h"
#include "TWrite.h"
#include "TObjectMgr.h"
bool TLoginServer::Init()
{	
	TServer::Init();
	//m_fnExecutePacket[PACKET_SYNC_NPC_LIST] = &TLoginServer::SyncNpc; // ºÒ°¡´É
	//m_fnExecutePacket[PACKET_SYNC_NPC_LIST]  = bind(&TLoginServer::SyncNpc, this, std::placeholders::_1);
	//m_fnExecutePacket[PACKET_NPC_TARGETMOVE] = bind(&TLoginServer::SyncNpc, this, std::placeholders::_1);
	m_fnExecutePacket[PACKET_HIT_NPC]		 = bind(&TLoginServer::HitNpc, this, std::placeholders::_1);
	I_Session.Init(this);
	

	m_pAcceptor = new TLoginAcceptor(this);
	m_pIocp		= new TIocp(this);	
	m_pSessionMgr = &I_Session;
	m_pIocp->Init();	
	m_pAcceptor->InitNetwork("192.168.0.151",10000);

	I_NpcMgr.Init(this);

	m_SceneNpc.Load(L"npc.txt");

	m_pAcceptor->CreateThread();
	CreateThread();
	SetThreadPriority((HANDLE)m_hThread, THREAD_PRIORITY_HIGHEST);
	m_bRun = true;	
	return false;
}
bool TLoginServer::PreRun()
{
#ifdef PACKET_TEST
	static int iRectCount = 0;	

	iRectCount += m_RecvPacketPool.m_list.size();
	static float fPostTimer = g_fGameTimer;
	static float fPreTimer = 0;
	fPreTimer = g_fGameTimer - fPostTimer;
	
	static int iPacketCount = 0;
	if (fPreTimer > 3.0f)
	{		
		TMsg msg2;
		msg2.msg = L"recv";
		msg2.msg += to_wstring(iRectCount/3);
		int iTop = g_Write.m_TextList.size();
		msg2.rt = { 0,iTop * 25,g_rtClient.right, g_rtClient.bottom };
		g_Write.push(msg2);

		iRectCount = 0;
		fPostTimer = g_fGameTimer;
	}
#endif
	return true;
}
bool TLoginServer::PostRun()
{	
#ifdef PACKET_TEST
	static int iSendCount = 0;
	iSendCount += m_SendBroadcastPacketPool.m_list.size() + m_SendPacketPool.m_list.size();
	static float fPostTimer = g_fGameTimer;
	static float fPreTimer = 0;
	fPreTimer = g_fGameTimer - fPostTimer;

	static int iPacketCount = 0;
	if (fPreTimer > 1.0f)
	{
		TMsg msg;
		msg.msg = L"send";
		msg.msg += to_wstring(iSendCount);
		int iTop = g_Write.m_TextList.size();
		msg.rt = { 0,iTop * 25,g_rtClient.right, g_rtClient.bottom };
		g_Write.push(msg);

		iSendCount = 0;
		fPostTimer = g_fGameTimer;
	}
#endif
	TServer::PostRun();
	return true;
}
bool TLoginServer::Run()
{
	TServer::Run();
	WaitForSingleObject((HANDLE)m_pAcceptor->m_hThread, INFINITE);
	return false;
}
void TLoginServer::ZoneEntry(TPacket& t)
{
	// client
	{
		TLock lock((TServerObj*)&I_Session);
		int iNumUsers = (int)I_Session.m_ZoneUserList.size();
		UPACKET sendPacket;
		TPACKET_CHARACTER_INFO* info =
			new TPACKET_CHARACTER_INFO[iNumUsers];


		std::map<SOCKET, TNetUser*>::iterator iter;
		int iCnt = 0;
		for (iter = I_Session.m_ZoneUserList.begin();
			iter != I_Session.m_ZoneUserList.end();
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

		// npc
		{
			TLock lock(&m_SceneNpc);
			int iNumNpc = (int)m_SceneNpc.m_NpcList.size();
			memset(&sendPacket, 0, sizeof(sendPacket));
			TPACKET_CHARACTER_INFO* npcinfo =	new TPACKET_CHARACTER_INFO[iNumNpc];
			int iCnt = 0;
			for (auto iter : m_SceneNpc.m_NpcList)
			{
				TObject* pNpc = (TObject*)iter.second;
				//pNpc->SetPos(TPoint(rand() % 100, rand() % 500));
				npcinfo[iCnt].movement.index = pNpc->m_iIndex;
				npcinfo[iCnt].movement.p = pNpc->m_ptPos.get();				
				npcinfo[iCnt].movement.d = pNpc->m_ptDirection.get();				
				npcinfo[iCnt].movement.t = { 0,0 };

				iCnt++;
				/*std::wstring msg;
				msg += to_wstring(pNpc->m_ptPos.x);
				msg += L":";
				msg += to_wstring(pNpc->m_ptPos.y);
				g_Write.push({g_rtClient, msg});*/
			}
			MakePacket(sendPacket, (char*)npcinfo,
				sizeof(TPACKET_CHARACTER_INFO)*iNumNpc,
				PACKET_SPAWN_NPC_LIST);
			if (t.pUser != nullptr)
			{
				TPacket tPacket = t;
				tPacket.pUser = t.pUser;
				tPacket.packet = sendPacket;
				tPacket.packet.ph.iotype = iNumNpc;
				m_SendPacketPool.AddPacket(tPacket);
			}
			delete npcinfo;
		}		
	}
}
void TLoginServer::SyncNpc(TPacket& t)
{
	m_SendBroadcastPacketPool.AddPacket(t);
}
void TLoginServer::HitNpc(TPacket& t)
{
	// TODO : HitNpc
	TEnemyPos* enemy = (TEnemyPos*)t.packet.msg;
	I_NpcMgr.AddDel(enemy->index);
	t.packet.ph.type = PACKET_DEAD_NPC;
	m_SendBroadcastPacketPool.AddPacket(t);						
}
bool TLoginServer::Release()
{	
	TServer::Release();
	m_SceneNpc.Release();
	m_pIocp->Release();
	delete m_pAcceptor;
	delete m_pIocp;
	return false;
}
void TLoginServer::Login(TPacket& t)
{
	TLogin* login = (TLogin*)t.packet.msg;
	T_STR szInUserID = to_mw(login->szID);
	T_STR szInUserPS = to_mw(login->szPS);
	T_STR szID;
	T_STR szPS;
	TNetResult ret;
	ZeroMemory(&ret, sizeof(TNetResult));
	// TODO : Call Stored Precedure 
	//{
	//	bool bRet = m_Query.ExecLogin(szInUserID.c_str(), szInUserPS.c_str());
	//	if (bRet == true)
	//	{
	//		if (m_Query.m_RecordResultList.size())
	//		{
	//			szID = m_Query.m_RecordResultList[0][1];
	//			szPS = m_Query.m_RecordResultList[0][2];
	//		}
	//	}		
	//	ret.id = t.pUser->m_Sock;
	//	ret.iRet = m_Query.m_iRetRaram;
	//}
	// TODO : Direct Query
	{
		bool bRet = m_Query.ExecLogin(szInUserID.c_str());
		if (bRet == true)
		{
			if (m_Query.m_RecordResultList.size())
			{
				szID = m_Query.m_RecordResultList[0][1];
				szPS = m_Query.m_RecordResultList[0][2];
			}
		}
		ret.id = t.pUser->m_Sock;		
		if (szID == szInUserID)
		{
			ret.iRet = 1;
			if (szPS == szInUserPS)
			{
				ret.iRet = 2;
			}
		}
		else
		{
			ret.iRet = 0;
		}
	}

	UPACKET sendPacket;
	MakePacket(sendPacket, (char*)&ret, sizeof(TNetResult),	PACKET_LOGIN_ACK);
	if (t.pUser != nullptr)
	{
		//t.pUser->m_SendPacket.push_back(sendPacket);
		TPacket tPacket = t;
		tPacket.pUser = t.pUser;
		tPacket.packet = sendPacket;
		m_SendPacketPool.AddPacket(tPacket);
	}

	if (ret.iRet == 1)
	{
		// other		
		ZeroMemory(&t.pUser->m_info, sizeof(TPACKET_CHARACTER_INFO));
		t.pUser->m_info.movement.p[0] = 50 + (rand() % 600);
		t.pUser->m_info.movement.p[1] = 500;
		t.pUser->m_info.movement.index = t.pUser->m_Sock;

		UPACKET sendPacket;
		MakePacket(sendPacket, (char*)&t.pUser->m_info, sizeof(TPACKET_CHARACTER_INFO),	PACKET_ZONE_NEW_PLAYER);
		if (t.pUser != nullptr)
		{
			TPacket tPacket = t;
			tPacket.pUser = t.pUser;
			tPacket.packet = sendPacket;
			m_SendBroadcastPacketPool.AddPacket(tPacket);
		}
	}
}

void TLoginServer::MoveAction(TPacket& t)
{
	TEnemyPos* pMsg = (TEnemyPos*)&t.packet.msg;
	TChatUser* pUser = (TChatUser*)t.pUser;
	t.pUser->m_info.movement = *pMsg;
	if (pUser != nullptr)
	{
		pUser->m_Object.SetPos(t.pUser->m_info.movement.p.get());
		pUser->m_Object.SetDir(t.pUser->m_info.movement.p.get());
	}
	m_SendBroadcastPacketPool.AddPacket(t);
}
TLoginServer::TLoginServer() : m_bRun(false)
{
}

TLoginServer::~TLoginServer()
{

}
