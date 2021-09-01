#pragma once
#include "TEnemy.h"
class TAttackAction : public TEnemyState
{
public:
	TAttackAction(TEnemy* parent);
	virtual ~TAttackAction();
	virtual	void		Process(xObject* ch) override;;
};
