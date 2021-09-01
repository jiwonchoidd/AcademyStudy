#pragma once
#include "TEnemyState.h"
class TStandState :
	public TEnemyState
{
public:
	void Process(TObject* pPlayer);
	TStandState(TObject* owner);
};

