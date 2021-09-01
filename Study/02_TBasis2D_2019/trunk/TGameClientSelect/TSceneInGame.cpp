#include "TSceneInGame.h"
#include "TTimer.h"

bool  TSceneInGame::SetObject(TObjAttribute& info, TObject* pObj)
{
	if (TScene::SetObject(info, pObj) == false)
	{		
		if (pObj && !(info.szParentName == L"nullptr" || info.szParentName == L"NULL"))
		{
			pObj->m_pParentObject = FindParent(info.szParentName);
			if (pObj->m_pParentObject != nullptr)
			{
				pObj->m_pParentObject->m_pChildObjects.push_back(pObj);
			}
		}

		switch (info.iObjType)
		{
		case TDemoEnum::T_GAMEPLAYER:
		{
			m_pGamePlayer = (TGameUser*)pObj;
			m_ptInitHeroPos = info.pos;
			pObj->Set(info.pos, info.rtSrc);
			pObj->m_bCollisionEnabled = true;
			return true;
		}break;
		case TDemoEnum::T_AIRNPC:
		{
			pObj->Set(info.pos, info.rtSrc);
			pObj->m_bCollisionEnabled = true;
			if (pObj->m_pParentObject != nullptr)
			{
				m_ObjList.insert(make_pair(pObj->m_szName, pObj));
			}
		}break;
		}
	}
	return false;
}
bool  TSceneInGame::CreateScene()
{
	//TScene::Reset();
	for (TObjAttribute& info : m_ObjAttribute)
	{
		// NPC 모델 정보만 저장 한 뒤 -> 스폰한다
		if (info.iObjType >= TEnum::T_NPC && info.iObjType < TEnum::T_NPC + 100)
		{
			continue;
		}
		// 인트로에서 생성되었을 경우에 해당됨.
		if (info.iObjType == TDemoEnum::T_GAMEPLAYER && m_pGamePlayer != nullptr)
		{
			m_ptInitHeroPos = info.pos;
			m_pGamePlayer->Set(info.pos, info.rtSrc);
			continue;
		}
		TObject* pObj = NewObj(info);
		pObj->Init();
		pObj->m_szName = info.szName;
		pObj->m_Attribute = info;
		if (info.bColorKey == false)
		{
			pObj->Load(info.colorbitmap.c_str(), info.maskbitmap.c_str());
		}
		else
		{
			pObj->Load(info.colorbitmap.c_str(), nullptr, info.dwColor);
		}
		// state
		pObj->m_StateBitmap.resize(4);
		pObj->m_StateBitmap[DEFAULT] = pObj->m_pColorBmp;
		if (!info.pushbitmap.empty())
		{
			pObj->m_StateBitmap[PUSH] = g_BitmapMgr.Load(info.pushbitmap.c_str());
		}
		if (!info.selectbitmap.empty())
		{
			pObj->m_StateBitmap[SELECT] = g_BitmapMgr.Load(info.selectbitmap.c_str());
		}
		if (!info.disbitmap.empty())
		{
			pObj->m_StateBitmap[DISABLE] = g_BitmapMgr.Load(info.disbitmap.c_str());
		}

		SetObject(info, pObj);
	}
	m_iMaxCounter = m_ObjList.size();
	SortZValue(m_UIObjList, m_UIDrawObjList);

	UPACKET sendPacket;
	m_pNetwork->MakePacket(sendPacket, nullptr, 0, PACKET_ZONE_NEW_PLAYER);
	m_pNetwork->m_sendPacket.push_back(sendPacket);
	return true;
}
bool  TSceneInGame::Reset()
{
	//TScene::Reset();	
	m_iNextScene = TScene::TSCENE_LOBBY;
	m_bSceneChange = false;
	return true;
}
bool TSceneInGame::Render()
{
	TScene::Render();
	if (m_pGamePlayer != nullptr)
	{
		m_pGamePlayer->Render();
	}
	/*if (m_iMaxCounter <= m_iNpcCounter)
	{
		TScene::m_iStageCounter++;
		CreateScene();

		wstring nextstage = to_wstring(m_iStageCounter);
		nextstage += L"Next Scene Play";
		Sleep(1000);
		g_Timer.Reset();
	}*/
	return true;
}
bool TSceneInGame::Frame()
{
	TScene::Frame();

	if (m_pGamePlayer != nullptr)
	{
		m_pGamePlayer->Frame();
	}

	// TODO : npc : playuser
	for (auto iter : m_NpcList)
	{		
		TObject* pNpcObj = (TObject*)iter.second;
		// todo : NpcBody : PlayerBody
		if (TCollision::Rect2Rect(pNpcObj->m_rtCollide,m_pGamePlayer->m_rtCollide) )
		{
			m_pGamePlayer->m_OverlapState[pNpcObj->m_iObjectType] = OVERLAP_BEGIN;
			if (m_pGamePlayer->Damage()==true)
			{
				/**	
				// TODO : Change Scene	*/
				//m_bSceneChange = true;
				//TScene::m_iStageCounter = 0;
				//m_iNextScene = TScene::TSCENE_LOBBY;
			}
		}
		// TODO :  NpcBody : GamePlayer Projectile
		for (TProjectileInfo& bullet : m_pGamePlayer->m_ProjectileList)
		{			
			if (bullet.m_bDead) continue;
			if (TCollision::Rect2Rect(bullet.m_rtCollide, pNpcObj->m_rtCollide))
			{				
				if (m_pGamePlayer->Overlap(pNpcObj->m_iIndex) == OVERLAP_NONE )
				{
					m_pGamePlayer->Hit(bullet.p, pNpcObj->m_iIndex);
					m_pGamePlayer->m_OverlapState[pNpcObj->m_iIndex] = OVERLAP_BEGIN;
					bullet.m_bDead = true;
				}				
				else if (m_pGamePlayer->Overlap(pNpcObj->m_iIndex) == OVERLAP_BEGIN)
				{
					m_pGamePlayer->m_OverlapState[pNpcObj->m_iIndex] = OVERLAP_STAY;
				}
			}
			else
			{
				if (m_pGamePlayer->Overlap(pNpcObj->m_iIndex) == OVERLAP_BEGIN ||
					m_pGamePlayer->Overlap(pNpcObj->m_iIndex) == OVERLAP_STAY)
				{
					m_pGamePlayer->m_OverlapState[pNpcObj->m_iIndex] = OVERLAP_END;
				}
				else
				{
					m_pGamePlayer->m_OverlapState[pNpcObj->m_iIndex] = OVERLAP_NONE;
				}				
			}
		}
	}	
	return true;
}
void  TSceneInGame::AddNpc(wstring name, TPoint pos, TPoint dir, int index)
{
	TObjAttribute info = m_ObjAttribute[3];
	TObject* pObj = NewObj(info);
	pObj->Init();
	pObj->m_iIndex = index;
	pObj->m_szName = name;
	pObj->m_Attribute = info;
	if (info.bColorKey == false)
	{
		pObj->Load(info.colorbitmap.c_str(),
			info.maskbitmap.c_str());
	}
	else
	{
		pObj->Load(info.colorbitmap.c_str(),
			nullptr,
			info.dwColor);
	}
	TPoint p = { pos.x,pos.y };
	pObj->Set(p, info.rtSrc);
	pObj->m_bCollisionEnabled = true;
	pObj->m_ptDirection = { dir.x, dir.y };
	m_NpcList.insert(make_pair(index,pObj));
	m_iMaxCounter = m_NpcList.size();
}
TSceneInGame::TSceneInGame()
{
	m_bGameFinish	= false;
	m_iSceneID		= TScene::TSCENE_ZONE;
}