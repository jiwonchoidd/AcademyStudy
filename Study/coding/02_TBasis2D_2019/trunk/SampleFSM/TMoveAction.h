#pragma once
#include "TEnemy.h"
class TMoveAction : public TEnemyState
{
public:
	TMoveAction(TEnemy* parent);
	virtual ~TMoveAction();
	virtual	void		Process(xObject* ch) override;;
private:
	POINT		m_vTargetPos;
};

