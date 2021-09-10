#pragma once
#include "xObject.h"
class TEnemy : public xObject
{
	int			m_iDirection[2];
public:
	bool	Frame();
public:
	TEnemy();
	virtual ~TEnemy();
};

