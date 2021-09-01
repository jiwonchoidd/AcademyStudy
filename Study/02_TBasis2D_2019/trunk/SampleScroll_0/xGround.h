#pragma once
#include "xObject.h"
class xGround : public xObject
{
	float	m_fScrollValue;
public:
	bool   AutoDown();
	bool   AutoUp();
	bool   InputTopDown();
	bool   SideScrolling();
public:
	xGround();
	virtual ~xGround();
};

