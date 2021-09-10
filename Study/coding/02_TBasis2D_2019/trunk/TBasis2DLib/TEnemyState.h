#pragma once
#include "TObject.h"
#include "TFiniteState.h"
class TEnemyState
{
public:
	TObject*    m_pOwner;
	DWORD		m_dwState;
	float		m_fTimer;
	float		m_fTransitionInterval;
public:
	virtual void Process(TObject* pPlayer) = 0;
	TEnemyState(TObject* owner);
};

