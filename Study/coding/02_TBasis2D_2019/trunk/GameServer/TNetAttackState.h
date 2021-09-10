#pragma once
#include "TProjectile.h"
#include "TAttackState.h"
class TNetAttackState :public TAttackState
{
public:
	void Process(TObject* pPlayer);
public:
	TNetAttackState(TObject* owner);
};

