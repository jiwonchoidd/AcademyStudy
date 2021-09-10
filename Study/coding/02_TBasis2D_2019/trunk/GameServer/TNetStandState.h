#pragma once
#include "TStandState.h"
class TNetStandState :	public TStandState
{
public:
	void Process(TObject* pPlayer);
	TNetStandState(TObject* owner);
};

