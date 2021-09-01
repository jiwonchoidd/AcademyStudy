#include "TNetAttackState.h"
#include "TNetObject.h"
#include "TGameSessionMgr.h"
#include "TServer.h"
#include "TChatUser.h"

void TNetAttackState::Process(TObject* pPlayer)
{
	//m_fTimer += g_fSecondPerFrame;
	//if (m_fTimer >= m_fTransitionInterval)
	//{
	//}

	TNetObject* pNetObj = (TNetObject*)m_pOwner;
	TChatUser* pTargetUser = pNetObj->m_pTargetUser;
	float fMinDistance = 200;
	if (pTargetUser != nullptr)
	{
		if (pTargetUser->m_dwCurrentServer == TServer::TSERVER_ZONE)
		{
			TPoint p = pTargetUser->m_Object.m_ptPos;
			TPoint ptTargetDir = p - m_pOwner->m_ptPos;
			float fDistance = ptTargetDir.Length();
			//m_pOwner->m_ptDirection = (p - m_pOwner->m_ptPos).Normalize();
			if (fDistance < fMinDistance)
			{				
				m_pOwner->m_fAttackTimer += g_fSecondPerFrame;
				if (m_pOwner->m_fAttackTimer > 1.0f)
				{
					m_pOwner->Attack(&pTargetUser->m_Object);
					m_pOwner->m_fAttackTimer -= 1.0f;
				}
			}
			else
			{
				pNetObj->m_pTargetUser = nullptr;
			}
		}		
	}
	else
	{
		m_pOwner->SetTransition(EVENT_LOSTTARGET);		
		m_pOwner->m_ptDirection = { 0, 0 };
		return;
	}
}
TNetAttackState::TNetAttackState(TObject* owner): TAttackState(owner)
{
	m_dwState = STATE_ATTACK;
	m_fTransitionInterval = 3.0f;
}