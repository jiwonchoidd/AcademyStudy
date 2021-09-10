#pragma once
#include "TMoveState.h"
class TNetMoveState : public TMoveState
{
public:
	void Process(TObject* pPlayer);
	TNetMoveState(TObject* owner);
};

