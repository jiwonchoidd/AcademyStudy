#pragma once
#include "TServerStd.h"
class TNetBase
{
public:
	CRITICAL_SECTION  m_cs;
public:
	TNetBase();
	virtual ~TNetBase();
};

