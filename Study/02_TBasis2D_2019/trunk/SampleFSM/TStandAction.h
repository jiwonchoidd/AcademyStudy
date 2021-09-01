#pragma once
#include "TEnemy.h"
class TStandAction : public TEnemyState
{
public:
	virtual	void		Process(xObject* ch) override;
public:
	TStandAction(TEnemy* parent);
	virtual ~TStandAction();
};

