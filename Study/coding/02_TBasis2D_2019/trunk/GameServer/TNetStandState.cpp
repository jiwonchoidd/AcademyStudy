#include "TNetStandState.h"
#include "TGameSessionMgr.h"
#include "TServer.h"
#include "TChatUser.h"

void TNetStandState::Process(TObject* pPlayer)
{	
	m_pOwner->m_ptDirection = { 0, 0 };
	m_fTimer += g_fSecondPerFrame;

	if (m_fTimer >= m_fTransitionInterval)
	{
		m_pOwner->m_ptDirection = { randstep(-1,1), randstep(-1,1) };
		m_pOwner->m_ptDirection.Normalize();
		m_pOwner->SetTransition(EVENT_TIMEMOVE);
		m_fTimer = 0.0f;
		return;
	}
	
	TChatUser* pFindUser = nullptr;
	float fMinDistance = 200;
	for (auto& user : I_Session.m_ZoneUserList)
	{
		TChatUser* pUser = (TChatUser*)user.second;
		if (pUser->m_dwCurrentServer == TServer::TSERVER_ZONE)
		{
			TPoint p = { pUser->m_info.movement.p[0], pUser->m_info.movement.p[1] };		
			TPoint ptDist = p - m_pOwner->m_ptPos;
			float fDistance = ptDist.Length();
			if (fDistance < fMinDistance)
			{
				pFindUser = pUser;
				fMinDistance = fDistance;
			}
		}
	}
	TNetObject* pNetObj = (TNetObject*)m_pOwner;
	pNetObj->m_pTargetUser = nullptr;
	if (pNetObj && pFindUser != nullptr)
	{
		pNetObj->m_pTargetUser = pFindUser;
		m_pOwner->SetTransition(EVENT_FINDTARGET);
		return;
	}
}
TNetStandState::TNetStandState(TObject* owner): TStandState(owner)
{
	m_dwState = STATE_STAND;
	m_fTransitionInterval = 1.0f;
}