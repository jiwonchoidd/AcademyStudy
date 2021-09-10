#include "TNpcAirObject.h"
#include "TScene.h"
TObject* TNpcAirObject::Clone()
{
	return new TNpcAirObject(*this);
}
void TNpcAirObject::Attack(TObject* pPlayer)
{
	if (m_pProjectile)
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
bool TNpcAirObject::Frame()
{
	//TNpcObject::Frame();
	// 목표 위치 도착여부 판단->프레임->디렉션 = 0;
	/*float fDistance = (m_tTargetPos - m_ptPos).Length();
	if (fDistance <= 10.0f)
	{
		m_ptDirection = { 0, 0 };
		m_tTargetPos = m_ptPos;
	}*/
	m_ptPos.x += m_ptDirection.x * m_fSpeed * g_fSecondPerFrame;
	m_ptPos.y += m_ptDirection.y * m_fSpeed * g_fSecondPerFrame;
	SetPos(m_ptPos);

	// TODO : Npc Projectle frame
	for (std::vector<TProjectileInfo>::iterator iter = m_ProjectileList.begin();iter != m_ProjectileList.end();	)
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
	// TODO :  NPC Projectile : GamePlayer Body
	for (TProjectileInfo& bullet : m_ProjectileList)
	{
		if (TCollision::Rect2Rect(bullet.m_rtCollide,TScene::m_pGamePlayer->m_rtCollide))
		{
			TScene::m_pCurrentScene->AddEffect(L"rtExplosion", bullet.p, { 0,0 });
			bullet.m_bDead = true;
			if (TScene::m_pGamePlayer->Damage())
			{
				TScene::m_pCurrentScene->m_bSceneChange = true;
				TScene::m_iStageCounter = 0;
			}
		}
	}
	return true;
}
bool TNpcAirObject::Render()
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
bool TNpcAirObject::Damage()
{
	return false;
}
bool TNpcAirObject::Dead()
{
	return true;
}