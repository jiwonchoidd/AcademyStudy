#include "TGameUser.h"
#include "TInput.h"
#include "TObjectMgr.h"
TObject* TGameUser::Clone()
{
	return new TGameUser(*this);
}
bool TGameUser::Damage()
{
	return true;
}
bool TGameUser::Dead() 
{
	return true;
}
bool TGameUser::Hit(TPoint p, int iIndex)
{
	return true;
}

bool  TGameUser::Reset()
{
	m_ProjectileList.clear();	
	return true;
}
bool TGameUser::Init()
{
	TObject::Init();
	m_pProjectile = (TEffect*)g_ObjectMgr.GetPtr(L"rtProjectile");
	m_bAttack = false;
	m_fKeyTimer = 0.0f;
	return true;
}
bool TGameUser::ProjectileTick()
{
	if (m_pProjectile && m_bAttack)
	{
		TProjectileInfo info;
		info.m_bDead = false;
		info.m_fLifeTime = 10.0f;
		info.id = 3;// rand() % 4;
		info.iMaxCount = (int)m_pProjectile->m_rtList.size();
		info.m_fStep = 0.1f;
		info.p = { m_ptPos.x, m_ptPos.y };

		m_ProjectileList.push_back(info);
		m_bAttack = false;
	}

	/// TODO : OtherUser projectle frame
	for (std::vector<TProjectileInfo>::iterator iter = m_ProjectileList.begin();
		iter != m_ProjectileList.end();	)
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
	return true;
}
bool TGameUser::Frame()
{		
	m_dwMove = 0;
	TPoint dir(0, 0);
	if (g_bActive)
	{
		if (g_KeyMap.m_bRight)
		{
			dir.x = 1.0f;
			m_dwMove |= 8;
		}
		if (g_KeyMap.m_bLeft)
		{
			dir.x = -1.0f;
			m_dwMove |= 4;
		}
		if (g_KeyMap.m_bFront)
		{
			dir.y = -1.0f;
			m_dwMove |= 2;
		}
		if (g_KeyMap.m_bBack)
		{
			dir.y = 1.0f;
			m_dwMove |= 1;
		}
		m_ptDirection = dir.Normalize();
		m_ptPos.x += m_ptDirection.x * m_fSpeed * g_fSecondPerFrame;
		m_ptPos.y += m_ptDirection.y * m_fSpeed * g_fSecondPerFrame;
	}
	else
	{
		m_ptDirection = dir.Normalize();
		m_dwMove = m_dwPreMove = 0;
	}
	SetPos(m_ptPos);
	ProjectileTick();
	return true;
}

bool TGameUser::Render()
{
	TObject::Render();
	for (auto& pInfo : m_ProjectileList)
	{
		m_pProjectile->Set(pInfo.p,	m_pProjectile->m_rtList[pInfo.m_iRectIndex]);
		pInfo.m_rtCollide = m_pProjectile->m_rtCollide;
		m_pProjectile->Render();
	}
	return true;
}