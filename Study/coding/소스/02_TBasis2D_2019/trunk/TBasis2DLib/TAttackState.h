#pragma once
#include "TProjectile.h"
#include "TEnemyState.h"
class TAttackState :public TEnemyState
{
public:
	void Process(TObject* pPlayer);
public:
	TAttackState(TObject* owner);
};

