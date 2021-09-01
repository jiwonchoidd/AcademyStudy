#pragma once
#include "TServerStd.h"
class TServerObj
{
public:
	CRITICAL_SECTION  m_cs;
public:
	TServerObj(void);
	virtual ~TServerObj(void);
};

