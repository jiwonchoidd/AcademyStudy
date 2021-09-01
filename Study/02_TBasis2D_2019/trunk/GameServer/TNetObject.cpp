#include "TNetObject.h"
#include "TScene.h"
#include "TGameSessionMgr.h"
#include "TChatUser.h"
#include "TObjectMgr.h"
#include "TServer.h"
void TNetObject::FSM()
{
	m_pActionList.push_back(new TNetStandState(this));
	m_pActionList.push_back(new TNetMoveState(this));
	m_pActionList.push_back(new TNetAttackState(this));
	m_pAction = m_pActionList[0];

	m_pTargetUser = nullptr;
	m_fAttackTimer = 1.0f;
	m_pProjectile = (TEffect*)g_ObjectMgr.GetPtr(L"rtProjectile");

}
void TNetObject::Attack(TObject* pPlayer)
{
	if (m_pProjectile && pPlayer !=nullptr)
	{
		TProjectileInfo info;
		info.m_bDead = false;
		info.m_fLifeTime = 10.0f;
		info.id = 3;// rand() % 4;
		info.iMaxCount = m_pProjectile->m_rtList.size();
		info.m_fStep = 0.1f;
		info.p = { m_ptPos.x, m_ptPos.y };
		TPoint vDir = pPlayer->m_ptPos - m_ptPos;
		info.m_vDir = TPoint(0.0f, 1.0f);
		m_ProjectileList.push_back(info);
	}
}
bool TNetObject::Frame()
{
	if (m_bDead == true) return true;
	m_pAction->Process(nullptr);

	m_ptPos = m_ptPos + m_ptDirection *	m_fSpeed *	g_fSecondPerFrame;
	SetPos(m_ptPos);
		
	//m_fAttackTimer -= g_fSecondPerFrame;
	//if (m_pProjectile && m_fAttackTimer <= 0.0f)
	//{
	//	TProjectileInfo info;
	//	info.m_bDead = false;
	//	info.m_fLifeTime = 10.0f;
	//	info.id = 3;// rand() % 4;
	//	info.iMaxCount = m_pProjectile->m_rtList.size();
	//	info.m_fStep = 0.1f;
	//	info.p = { m_ptPos.x, m_ptPos.y };
	//	info.m_vDir = TPoint(0.0f, 1.0f);
	//	m_ProjectileList.push_back(info);

	//	m_fAttackTimer = 3.0f;
	//}
	for (std::vector<TProjectileInfo>::iterator iter = m_ProjectileList.begin();
		iter != m_ProjectileList.end();
		)
	{
		iter->Frame();
		if (iter->m_bDead == true)
		{
			iter = m_ProjectileList.erase(iter);
			continue;
		}
		else
		{
			iter++;
		}
	}
	//for (TProjectileInfo& project : m_ProjectileList)
	//{
	//	for (auto& user : I_Session.m_ZoneUserList)
	//	{
	//		TChatUser* pUser = (TChatUser*)user.second;
	//		if (TCollision::Rect2Rect(
	//			project.m_rtCollide,
	//			pUser->m_Object.m_rtCollide))
	//		{
	//			if (TScene::m_pCurrentScene != nullptr)
	//			{
	//				TScene::m_pCurrentScene->AddEffect(L"rtExplosion", project.p);
	//				project.m_bDead = true;
	//				if (TScene::m_pGamePlayer->Damage())
	//				{
	//					//TScene::m_pCurrentScene->m_bSceneChange = true;
	//					//TScene::m_iStageCounter = 0;
	//				}
	//			}
	//		}
	//	}
	//}		
	
	return true;
}
bool TNetObject::Render()
{
	TNpcObject::Render();
	for (auto& pInfo : m_ProjectileList)
	{
		m_pProjectile->Set(pInfo.p,
			m_pProjectile->m_rtList[pInfo.m_iRectIndex]);
		pInfo.m_rtCollide = m_pProjectile->m_rtCollide;
		m_pProjectile->Render();
	}
	return true;
}
bool TNetObject::Damage()
{
	return false;
}
bool TNetObject::Dead()
{
	return true;
}
void  TNetObject::SetPos(TPoint p)
{
	m_ptPos = p;
	Set(p, m_rtSrc);
}