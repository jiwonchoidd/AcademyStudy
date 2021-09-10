#include "TMoveState.h"

void TMoveState::Process(TObject* pPlayer)
{
	if (m_pOwner->m_ptPos.x < g_rtClient.left)
	{
		m_pOwner->m_ptDirection.x *= -1.0f;
		m_pOwner->m_ptPos.x = g_rtClient.left;
	}
	if (m_pOwner->m_ptPos.y < g_rtClient.top)
	{
		m_pOwner->m_ptDirection.y *= -1.0f;
		m_pOwner->m_ptPos.y = g_rtClient.top;
	}
	if (m_pOwner->m_ptPos.x > g_rtClient.right)
	{
		m_pOwner->m_ptDirection.x *= -1.0f;
		m_pOwner->m_ptPos.x = g_rtClient.right;
	}
	if (m_pOwner->m_ptPos.y > g_rtClient.bottom)
	{
		m_pOwner->m_ptDirection.y *= -1.0f;
		m_pOwner->m_ptPos.y = g_rtClient.bottom;
	}
	m_pOwner->m_ptPos.x += m_pOwner->m_ptDirection.x *
		m_pOwner->m_fSpeed *
				 g_fSecondPerFrame;
	m_pOwner->m_ptPos.y += m_pOwner->m_ptDirection.y *
		m_pOwner->m_fSpeed *
				 g_fSecondPerFrame;

	m_pOwner->SetPos(m_pOwner->m_ptPos);

	if (rand() % 10000 < 3)
	{
		m_pOwner->SetTransition(EVENT_STOPMOVE);
	}
	TPoint ptDist = pPlayer->m_ptPos - m_pOwner->m_ptPos;
	float fDistance = ptDist.Length();
	if (fDistance < 500)
	{
		m_pOwner->SetTransition(EVENT_FINDTARGET);
	}
}
TMoveState::TMoveState(TObject* owner)
	: TEnemyState(owner)
{
	m_dwState = STATE_MOVE;
}