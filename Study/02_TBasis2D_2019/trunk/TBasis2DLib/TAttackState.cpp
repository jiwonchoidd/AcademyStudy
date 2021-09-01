#include "TAttackState.h"
#include "TNpcObject.h"
void TAttackState::Process(TObject* pPlayer)
{
	TPoint vDir = pPlayer->m_ptPos - m_pOwner->m_ptPos;
	float fDistance = vDir.Length();
	if (fDistance > 500)
	{
		m_pOwner->SetTransition(EVENT_LOSTTARGET);
	}
	m_pOwner->m_ptPos = m_pOwner->m_ptPos + vDir.Normalize() * g_fSecondPerFrame* 100.0f;
	m_pOwner->SetPos(m_pOwner->m_ptPos);

	m_pOwner->m_fAttackTimer += g_fSecondPerFrame;
	if (m_pOwner->m_fAttackTimer > 1.0f)
	{
		m_pOwner->Attack(pPlayer);
		m_pOwner->m_fAttackTimer -= 1.0f;
	}
}
TAttackState::TAttackState(TObject* owner)
	: TEnemyState(owner)
{
	m_dwState = STATE_ATTACK;
}