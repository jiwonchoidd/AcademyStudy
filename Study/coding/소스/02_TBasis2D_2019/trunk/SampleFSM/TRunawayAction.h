#pragma once
#include "TEnemy.h"
class TRunawayAction : public TEnemyState
{
public:
	TRunawayAction(TEnemy* parent);
	virtual ~TRunawayAction();
public:
	virtual	void		Process(xObject* ch)override;;
};

