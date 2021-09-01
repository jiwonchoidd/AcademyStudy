#include "TStandState.h"

void TStandState::Process(TObject* pPlayer)
{
	if (rand() % 10000 < 3)
	{
		m_pOwner->SetTransition(EVENT_TIMEMOVE);
	}

	TPoint ptDist = pPlayer->m_ptPos - m_pOwner->m_ptPos;
	float fDistance = ptDist.Length();
	if (fDistance < 500)
	{
		m_pOwner->SetTransition(EVENT_FINDTARGET);
	}
}
TStandState::TStandState(TObject* owner) 
				: TEnemyState(owner)
{
	m_dwState = STATE_STAND;
}