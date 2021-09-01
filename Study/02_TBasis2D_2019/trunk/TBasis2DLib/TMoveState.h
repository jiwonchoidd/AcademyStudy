#pragma once
#include "TEnemyState.h"
class TMoveState :
	public TEnemyState
{
public:
	void Process(TObject* pPlayer);
	TMoveState(TObject* owner);
};

