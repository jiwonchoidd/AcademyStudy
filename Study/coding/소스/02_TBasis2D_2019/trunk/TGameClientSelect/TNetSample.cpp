#include "Sample.h"
#include "TGamePlayer.h"
static int iCount = 0;

void Sample::DisplayText(const char* fmt, ...)
{
	va_list arg;
	va_start(arg, fmt);
	char buf[1024 + 256] = { 0, };
	vsprintf_s(buf, fmt, arg);	
	OutputDebugStringA((char*)buf);
	va_end(arg);
}

void  Sample::SendAttack()
{
	if (TScene::m_pGamePlayer == nullptr) return;
	UPACKET sendPacket;
	TEnemyPos data;
	data.index = TScene::m_pGamePlayer->m_dwSvrID;
	data.p = TScene::m_pGamePlayer->m_ptPos;
	data.d = TScene::m_pGamePlayer->m_ptDirection;
	data.t = data.p + data.d * TScene::m_pGamePlayer->m_fSpeed * 100.0f;	
	if (data.d == TPoint::zero)
	{
		data.t = data.p;
	}
	m_Network.MakePacket(sendPacket, (char*)&data,sizeof(TEnemyPos),PACKET_ATTACK_CHARACTER);
	m_Network.AddSendPacket(sendPacket);
}
void Sample::SendGamePlayerData()
{
	if (TScene::m_pGamePlayer == nullptr) return;
	UPACKET sendPacket;
	TEnemyPos data;
	data.index = TScene::m_pGamePlayer->m_dwSvrID;
	data.p = TScene::m_pGamePlayer->m_ptPos;
	data.d = TScene::m_pGamePlayer->m_ptDirection;
	data.t = data.p + data.d * TScene::m_pGamePlayer->m_fSpeed * 100.0f;
	if (data.d == TPoint::zero)
	{
		data.t = data.p;
	}
	m_Network.MakePacket(sendPacket, (char*)&data,sizeof(TEnemyPos),PACKET_USER_POSITION);
	m_Network.AddSendPacket(sendPacket);
}
void  Sample::SendTest(const char* pData)
{
	TChatMsg chatmsg;
	memset(&chatmsg, 0, sizeof(chatmsg));
	strcpy_s(chatmsg.szName, m_szName.c_str());
	strcpy_s(chatmsg.buffer, pData);
	chatmsg.iCnt = iCount++;
	UPACKET sendPacket;
	m_Network.MakePacket(sendPacket, (char*)&chatmsg, sizeof(TChatMsg), PACKET_CHAT_MSG);
	m_Network.AddSendPacket(sendPacket);
}
bool  Sample::NetInit()
{
	srand(time(NULL));
	m_szName = "홍길동";
	m_szName += to_string(rand() % 100);
	m_bUseNet = false;
	//if (m_Network.InitNetwork("112.216.123.139", 10000) == false)
	if (m_Network.InitNetwork("127.0.0.1", 10000) == false)
	{
		return false;
	}
	m_bUseNet = true;

	TDemoGameScene* pGameScene = (TDemoGameScene*)TScene::m_pCurrentScene;
	pGameScene->m_pNetwork = &m_Network;
	return true;
}
bool  Sample::NetFrame()
{
#ifdef PACKET_TEST
	static int iRectCount = 0;
	iRectCount += m_Network.m_recvPacket.size();
	static float fTimer = 0.0f;
	fTimer += g_fSecondPerFrame;

	if (fTimer > 1.0f)
	{
		TMsg msg2;
		msg2.msg = L"recv";
		msg2.msg += to_wstring(iRectCount);
		int iTop = g_Write.m_TextList.size();
		msg2.rt = { 0,iTop * 25,g_rtClient.right, g_rtClient.bottom };
		g_Write.push(msg2);

		iRectCount = 0;
		fTimer -= 1.0f;
	}
#endif
	if (m_bUseNet)
	{
		if (m_Network.Frame() == false)
		{
			m_bUseNet = false;
			return true;
		}
		std::vector<TPacket>::iterator senditer;
		for (senditer = m_Network.m_recvPacket.begin();
			senditer != m_Network.m_recvPacket.end();
			senditer++)
		{
			TGamePlayer* pPlayer = (TGamePlayer*)TScene::m_pGamePlayer;
			UPACKET* packet = (UPACKET*)&senditer->packet;
	
			if (packet->ph.type == PACKET_CREATE_ACCOUNT_ACK)
			{
				TNetResult* ret = (TNetResult*)packet->msg;
				//TScene::m_pGamePlayer->m_dwSvrID = ret->id;
				if (ret->iRet == 1)
				{
				}
			}
			if (packet->ph.type == PACKET_DEAD_NPC)
			{
				TEnemyPos* ret = (TEnemyPos*)packet->msg;				
				if (TScene::m_pCurrentScene->DelNpc(ret->index))
				{
					TScene::m_pCurrentScene->AddEffect(L"rtExplosion", ret->p, ret->d);
				}
			}
			if (packet->ph.type == PACKET_DEAD_CHARACTER)
			{
			}
			if (packet->ph.type == PACKET_NPC_TARGETMOVE)
			{
				if (TScene::m_pCurrentScene->m_NpcList.size() <= 0) continue;
				TPACKET_CHARACTER_INFO* info = (TPACKET_CHARACTER_INFO*)packet->msg;
				TEnemyPos* ret = (TEnemyPos*)&info->movement;
				TPoint p = { ret->p[0],ret->p[1] };
				TPoint d = { ret->d[0],ret->d[1] };
				TPoint t = { ret->t[0],ret->t[1] };
				TNpcAirObject* npc = (TNpcAirObject*)TScene::m_pCurrentScene->FindNpc(ret->index);
				if (npc != nullptr)
				{
					npc->m_Info.movement = *ret;
					npc->m_ptDirection = d;
					npc->SetPos(p);
				}
			}
			if (packet->ph.type == PACKET_SYNC_NPC_LIST)
			{
				if (TScene::m_pCurrentScene->m_NpcList.size() <= 0) continue;
				TPACKET_CHARACTER_INFO* pNpcArray = (TPACKET_CHARACTER_INFO*)packet->msg;	
				/*if (TScene::m_pCurrentScene->m_ObjList.size() <= packet->ph.iotype)
				{
					SetWindowText(g_hWnd, L"ERROR");
				}*/
				for (int iNpc = 0; iNpc < packet->ph.iotype; iNpc++)
				{
					TPACKET_CHARACTER_INFO* info = &pNpcArray[iNpc];
					TEnemyPos* ret = (TEnemyPos*)&info->movement;
					TPoint p = { ret->p[0],ret->p[1] };
					TPoint d = { ret->d[0],ret->d[1] };
					TPoint t = { ret->t[0],ret->t[1] };			
					
					TNpcAirObject* npc = (TNpcAirObject*)TScene::m_pCurrentScene->FindNpc(ret->index);
					if (npc != nullptr)
					{
						npc->m_Info.movement = *ret;
						npc->m_ptDirection = d;
					}
				}				
			}
			if (packet->ph.type == PACKET_HIT_NPC)
			{
				TEnemyPos* ret = (TEnemyPos*)packet->msg;
				TPoint p = { ret->p[0],ret->p[1] };						
				TPoint d = { ret->d[0],ret->d[1] };
				TScene::m_pCurrentScene->AddEffect(L"rtExplosion", p,d);
			}
			if (packet->ph.type == PACKET_ATTACK_CHARACTER)
			{
				TEnemyPos* ret = (TEnemyPos*)packet->msg;
				TPoint p = { ret->p[0],ret->p[1] };
				TPoint d = { ret->d[0],ret->d[1] };
				if (ret->index == TScene::m_pGamePlayer->m_dwSvrID)
				{
					pPlayer->m_bAttack = true;
				}
				else
				{
					auto iter = m_userlist.find(ret->index);
					if (iter != m_userlist.end())
					{
						TGameOtherPlayer* user = iter->second;
						user->m_bAttack = true;
					}
				}
				//TMsg msg;
				//static int iCnt = 0;
				//msg.msg = L"Attack";
				//msg.msg += to_wstring(iCnt++);
				//int iTop = g_Write.m_TextList.size();
				//msg.rt = { 0,iTop * 25,g_rtClient.right, g_rtClient.bottom };
				//g_Write.push(msg);
			}
			if (packet->ph.type == PACKET_USER_POSITION)
			{
				TEnemyPos* ret = (TEnemyPos*)packet->msg;
				TPoint p = { ret->p[0],ret->p[1] };
				TPoint d = { ret->d[0],ret->d[1] };			
				if (ret->index == TScene::m_pGamePlayer->m_dwSvrID)
				{
					pPlayer->m_tSendDirection = d;		
				}
				else
				{					
					for (auto userobj : m_userlist)
					{
						TGameOtherPlayer* user = userobj.second;
						if (user->m_Info.movement.index == ret->index)
						{
							user->m_Info.movement = *ret;
							user->m_ptDirection = { user->m_Info.movement.d[0] , 
													user->m_Info.movement.d[1] };
							// 1 거리
							TPoint  t = { user->m_Info.movement.t[0],user->m_Info.movement.t[1] };
							float fDistance = (t - user->m_ptPos).Length();
							user->m_tTargetPos = user->m_ptPos;
							if(fDistance > 10.0f) 
							{
								user->m_ptDirection = (t - user->m_ptPos).Normalize();
								user->m_tTargetPos = t;	
							}
							//DisplayText("\nm_tTargetPos:%10.4f:%10.4f:%10.4f",
							//	user->m_tTargetPos.x, user->m_tTargetPos.y, fDistance);
							//DisplayText("\nm_ptPos:%10.4f:%10.4f", user->m_ptPos.x, user->m_ptPos.y);
							break;
						}						
					}
				}
				//DisplayText("Redv:%10.2f:%10.2f[%10.1f]", d.x, d.y, ret->p[2]);			
			}
			if (packet->ph.type == PACKET_LOGOUT_PLAYER)
			{
				TNetResult* ret = (TNetResult*)packet->msg;
				auto user = m_userlist.find(ret->id);
				if (user != m_userlist.end())
				{
					m_userlist.erase(user);
				}				
			}
			if (packet->ph.type == PACKET_CHAT_MSG)
			{
				static int iCount = 0;
				iCount++;
				TChatMsg* pMsg = (TChatMsg*)&packet->msg;
				TMsg msg;
				msg.msg = to_mw(pMsg->buffer);
				//msg.msg += to_wstring(iCount);
				int iTop = (int)g_Write.m_TextList.size();
				msg.rt = { 0,iTop * 25,g_rtClient.right, g_rtClient.bottom };
				g_Write.push(msg);
			}
			if (packet->ph.type == PACKET_LOGIN_ACK)
			{
				TNetResult* ret = (TNetResult*)packet->msg;
				TScene::m_pGamePlayer->m_dwSvrID = ret->id;
				if (ret->iRet == 2)
				{
					m_Network.m_bLogin = true;		
					TScene::m_pCurrentScene->m_bSceneChange = true;
					TScene::m_pCurrentScene->m_iNextScene = TScene::TSCENE_LOBBY;
				}
			}
			if (packet->ph.type == PACKET_ZONE_ENTRY_ACK)
			{
				m_Network.m_bZoneEntry = true;
				TScene::m_pCurrentScene->m_bSceneChange = true;
				TScene::m_pCurrentScene->m_iNextScene = TScene::TSCENE_ZONE;
			}
			if (packet->ph.type == PACKET_ZONE_NEW_PLAYER)
			{
				TPACKET_CHARACTER_INFO* info = (TPACKET_CHARACTER_INFO*)packet->msg;
				if (TScene::m_pGamePlayer->m_dwSvrID == info->movement.index)
				{
					TPoint t = { info->movement.p[0],info->movement.p[1] };
					TScene::m_pGamePlayer->SetPos(t);
				}			
				else
				{
					// AddUser
					AddUser(*info);					
				}
			}
			if (packet->ph.type == PACKET_ZONE_PLAYERS_INFO)
			{
				TPACKET_CHARACTER_INFO* pInfo = (TPACKET_CHARACTER_INFO*)packet->msg;
				for (int iUser = 0; iUser < packet->ph.iotype; iUser++)
				{
					TPACKET_CHARACTER_INFO* info = &pInfo[iUser];
					if (TScene::m_pGamePlayer->m_dwSvrID != info->movement.index)
					{
						// AddUser
						AddUser(*info);
					}
				}
			}
			if (packet->ph.type == PACKET_SPAWN_NPC_LIST)
			{
				TScene::m_pCurrentScene->m_NpcList.clear();
				TPACKET_CHARACTER_INFO* pInfo = (TPACKET_CHARACTER_INFO*)packet->msg;
				for (int iUser = 0; iUser < packet->ph.iotype; iUser++)
				{
					TPACKET_CHARACTER_INFO* info = &pInfo[iUser];
					AddNpc(*info);
				}
			}
			if (packet->ph.type == PACKET_SPAWN_NPC)
			{
				TPACKET_CHARACTER_INFO* pInfo = (TPACKET_CHARACTER_INFO*)packet->msg;
				for (int iUser = 0; iUser < packet->ph.iotype; iUser++)
				{
					TPACKET_CHARACTER_INFO* info = &pInfo[iUser];
					AddNpc(*info);
				}
			}
		}
		m_Network.m_recvPacket.clear();
	}
	return true;
}
bool  Sample::NetRelease()
{
	m_Network.m_sendPacket.clear();
	m_Network.m_recvPacket.clear();
	m_Network.DeleteNetwork();
	return true;
}
bool  Sample::NetRender()
{
	if (m_bUseNet )
	{
#ifdef PACKET_TEST
		static int iSendCount = 0;
		TGamePlayer* pPlayer = (TGamePlayer*)TScene::m_pGamePlayer;
		iSendCount += m_Network.m_sendPacket.size() + pPlayer->m_SendList.size();
		static float fTimer = 0.0f;
		fTimer += g_fSecondPerFrame;

		if (fTimer > 1.0f)
		{
			TMsg msg;
			msg.msg = L"send";
			msg.msg += to_wstring(iSendCount);
			int iTop = g_Write.m_TextList.size();
			msg.rt = { 0,iTop * 25,g_rtClient.right, g_rtClient.bottom };
			g_Write.push(msg);
			iSendCount = 0;
			fTimer -= 1.0f;
		}
#endif
		if ( m_Network.m_bLogin && TScene::m_pCurrentScene->m_iSceneID == TScene::TSCENE_ZONE)
		{
			TGamePlayer* pPlayer = (TGamePlayer*)TScene::m_pGamePlayer;
			pPlayer->m_fKeyTimer += g_fSecondPerFrame;
			if (pPlayer->m_dwPreMove != pPlayer->m_dwMove && 
				pPlayer->m_fKeyTimer >= pPlayer->m_fSendKeyInterval)
			{
				// 타켓위치 전송
				SendGamePlayerData();
				pPlayer->m_dwPreMove = pPlayer->m_dwMove;
				pPlayer->m_fKeyTimer -= pPlayer->m_fSendKeyInterval;
			}

			//if (pPlayer->m_fKeyTimer >= 1.1f)
			//{
			//	SendGamePlayerData();
			//	pPlayer->m_dwPreMove = pPlayer->m_dwMove;
			//	pPlayer->m_fKeyTimer -= 1.1f;
			//}

			for (int icnt = 0; icnt < pPlayer->m_SendList.size(); icnt++)
			{
				m_Network.AddSendPacket(pPlayer->m_SendList[icnt]);
			}
			pPlayer->m_SendList.clear();
		}
		if (!m_Network.SendPackets(m_Network.m_User))
		{
			m_bUseNet = false;
			return true;
		}
	}
	return true;
}