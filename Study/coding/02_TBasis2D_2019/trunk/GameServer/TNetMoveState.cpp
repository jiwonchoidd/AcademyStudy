#include "TNetMoveState.h"
#include "TGameSessionMgr.h"
#include "TServer.h"
#include "TChatUser.h"
void TNetMoveState::Process(TObject* pPlayer)
{	
	if (m_pOwner->m_ptPos.x < g_rtClient.left + (m_pOwner->m_rtDesk.right / 2))
	{
		m_pOwner->m_ptDirection.x *= -1.0f;
		m_pOwner->m_ptPos.x = g_rtClient.left + (m_pOwner->m_rtDesk.right / 2);
	}
	if (m_pOwner->m_ptPos.y < g_rtClient.top + (m_pOwner->m_rtDesk.bottom / 2))
	{
		m_pOwner->m_ptDirection.y *= -1.0f;
		m_pOwner->m_ptPos.y = g_rtClient.top + (m_pOwner->m_rtDesk.bottom / 2);
	}
	if (m_pOwner->m_ptPos.x > g_rtClient.right - (m_pOwner->m_rtDesk.right / 2))
	{
		m_pOwner->m_ptDirection.x *= -1.0f;
		m_pOwner->m_ptPos.x = g_rtClient.right - (m_pOwner->m_rtDesk.right / 2);
	}
	if (m_pOwner->m_ptPos.y > g_rtClient.bottom - (m_pOwner->m_rtDesk.bottom / 2))
	{
		m_pOwner->m_ptDirection.y *= -1.0f;
		m_pOwner->m_ptPos.y = g_rtClient.bottom - (m_pOwner->m_rtDesk.bottom / 2);
	}
	m_pOwner->m_ptDirection.Normalize();
	//m_pOwner->m_ptPos = m_pOwner->m_ptPos + m_pOwner->m_ptDirection *	m_pOwner->m_fSpeed *	g_fSecondPerFrame;
	//m_pOwner->SetPos(m_pOwner->m_ptPos);

	m_fTimer += g_fSecondPerFrame;
	if (m_fTimer >= m_fTransitionInterval)
	{
		m_pOwner->SetTransition(EVENT_STOPMOVE);
		m_pOwner->m_ptDirection = { 0, 0 };
		m_fTimer = 0;
		return;
	}
	
	TChatUser* pFindUser = nullptr;
	float fMinDistance = 200;
	for (auto& user : I_Session.m_ZoneUserList)
	{
		TChatUser* pUser = (TChatUser*)user.second;
		if (pUser->m_dwCurrentServer == TServer::TSERVER_ZONE)
		{
			TPoint p = { pUser->m_info.movement.p[0],pUser->m_info.movement.p[1] };
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
TNetMoveState::TNetMoveState(TObject* owner): TMoveState(owner)
{
	m_dwState = STATE_MOVE;
	m_fTransitionInterval = 3.0f;
}