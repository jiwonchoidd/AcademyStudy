#pragma once
#include "TNetStd.h"
class TServerObj
{
public:
	CRITICAL_SECTION m_cs;
public:
	TServerObj();
	virtual ~TServerObj();
};

