#include "Sample.h"
#include "TLoginServer.h"
#include "TFiniteStateMachine.h"
#include "TNpcMgr.h"
int Sample::g_iNpcIndex = 0;

bool  Sample::Init()
{
	OpenDebugConsole();
	m_fNpcSpawnTimer = 0.0f;
	m_fNpcSpawnInterval = 1.0f;
	m_iMaxNpcSpawn = 10;
	m_fSyncNpcInterval = 0.33f;
	m_fSyncNpcTimer = 0.0f;

	g_Fsm.AddStateTrasition(STATE_STAND, EVENT_TIMEMOVE, STATE_MOVE);
	g_Fsm.AddStateTrasition(STATE_STAND, EVENT_POINTMOVE, STATE_MOVE);
	g_Fsm.AddStateTrasition(STATE_STAND, EVENT_FINDTARGET, STATE_ATTACK);
	g_Fsm.AddStateTrasition(STATE_MOVE, EVENT_STOPMOVE, STATE_STAND);
	g_Fsm.AddStateTrasition(STATE_MOVE, EVENT_FINDTARGET, STATE_ATTACK);
	g_Fsm.AddStateTrasition(STATE_ATTACK, EVENT_LOSTTARGET, STATE_STAND);

	I_Server.Init();
	return true;
}
void  Sample::SendAllNpcMovement()
{
	UPACKET sendPacket;
	int iNumNpc = (int)I_Server.m_SceneNpc.m_NpcList.size();
	memset(&sendPacket, 0, sizeof(sendPacket));
	TPACKET_CHARACTER_INFO* npcinfo =	new TPACKET_CHARACTER_INFO[iNumNpc];

	int iNpc = 0;
	for (auto iter : I_Server.m_SceneNpc.m_NpcList)
	{
		TObject* pNpc = (TObject*)iter.second;
		npcinfo[iNpc].movement.index = pNpc->m_iIndex;
		npcinfo[iNpc].movement.p = pNpc->m_ptPos.get();
		npcinfo[iNpc].movement.d = pNpc->m_ptDirection.get();
		pNpc->m_ptPreDirection = pNpc->m_ptDirection;
		iNpc++;
	}
	I_Server.MakePacket(sendPacket, (char*)npcinfo,sizeof(TPACKET_CHARACTER_INFO)*iNumNpc,
		PACKET_SYNC_NPC_LIST);
	TPacket tPacket;
	tPacket.pUser = nullptr;
	tPacket.packet = sendPacket;
	tPacket.packet.ph.iotype = iNumNpc;
	I_Server.m_SendBroadcastPacketPool.AddPacket(tPacket);
	delete npcinfo;
}
void  Sample::SendNpcMovement(int iIndex)
{
	UPACKET sendPacket;
	memset(&sendPacket, 0, sizeof(sendPacket));
	TPACKET_CHARACTER_INFO npcinfo;
	TObject* pNpc = (TObject*)I_Server.m_SceneNpc.FindNpc(iIndex);
	npcinfo.movement.index = iIndex;
	npcinfo.movement.p = pNpc->m_ptPos.get();
	npcinfo.movement.d = pNpc->m_ptDirection.get();
	I_Server.MakePacket(sendPacket, (char*)&npcinfo,
		sizeof(TPACKET_CHARACTER_INFO),	PACKET_NPC_TARGETMOVE);
	TPacket tPacket;
	tPacket.pUser = nullptr;
	tPacket.packet = sendPacket;
	tPacket.packet.ph.iotype = 1;
	I_Server.m_SendBroadcastPacketPool.AddPacket(tPacket);
}
void  Sample::SendNpcSpawn(int iIndex)
{
	int iModelType = 0;
	UPACKET sendPacket;
	memset(&sendPacket, 0, sizeof(sendPacket));
	TPACKET_CHARACTER_INFO npcinfo;
	TObject* pNpc = (TObject*)I_Server.m_SceneNpc.FindNpc(iIndex);
	npcinfo.movement.index = iIndex;
	npcinfo.movement.p = pNpc->m_ptPos.get();
	npcinfo.movement.d = pNpc->m_ptDirection.get();
	I_Server.MakePacket(sendPacket, (char*)&npcinfo,
		sizeof(TPACKET_CHARACTER_INFO), PACKET_SPAWN_NPC);
	TPacket tPacket;
	tPacket.pUser = nullptr;
	tPacket.packet = sendPacket;
	tPacket.packet.ph.iotype = 1;
	I_Server.m_SendBroadcastPacketPool.AddPacket(tPacket);
}
bool  Sample::Frame()
{
	{
		TLock lock1(&I_Session);
		TLock lock2(&I_Server.m_SceneNpc);

		I_Server.m_SceneNpc.Frame();

		m_fNpcSpawnTimer += g_fSecondPerFrame;
		if (m_fNpcSpawnTimer >= m_fNpcSpawnInterval && I_Server.m_SceneNpc.m_NpcList.size() < m_iMaxNpcSpawn)
		{
			TPoint p = { randstep(100,700), 100 };
			std::wstring szName = L"npc";
			szName += to_wstring(g_iNpcIndex);
			I_Server.m_SceneNpc.AddNpc(szName, p, { 0, 0}, g_iNpcIndex);
			SendNpcSpawn(g_iNpcIndex);
			g_iNpcIndex++;
			m_fNpcSpawnTimer -= m_fNpcSpawnInterval;
		}

#ifdef SYNC_NPC_DIRECTION	
		/// TODO : 주의 -> 디렉션 변경시에 프레임에 마다 전송된다.
		for (auto iter : I_Server.m_SceneNpc.m_NpcList)
		{
			TNetObject* pNpc = (TNetObject*)iter.second;
			if (pNpc->m_ptPreDirection != pNpc->m_ptDirection)
			{
				SendNpcMovement(pNpc->m_iIndex);
				pNpc->m_ptPreDirection = pNpc->m_ptDirection;
			}
		}		
#else
		m_fSyncNpcTimer += g_fSecondPerFrame;
		if (m_fSyncNpcTimer >= m_fSyncNpcInterval)
		{
			SendAllNpcMovement();			
			m_fSyncNpcTimer -= m_fSyncNpcInterval;
		}
#endif
	}
	return true;
}
bool  Sample::Release()
{
	I_Server.Release();
	WaitForSingleObject((HANDLE)I_Server.m_hThread, INFINITE);
	return true;
}

bool  Sample::Render()
{
	{
		TLock lock(&I_Server.m_SceneNpc);
		I_Server.m_SceneNpc.Render();

		std::vector<int>  DelList = I_NpcMgr.copy();
		/*for (auto index : DelList)
		{
			if (I_Server.m_SceneNpc.DelNpc(index))
			{
			}
		}*/
		auto delList = [](auto& i){ I_Server.m_SceneNpc.DelNpc(i); };
		std::for_each(DelList.begin(), DelList.end(), delList);

		I_NpcMgr.m_DelList.clear();
	}
	return true;
}

TGAME_RUN;