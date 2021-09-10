#pragma once
#include "TBasisStd.h"

class TServerObj
{
public:
	CRITICAL_SECTION   m_CS;
public:
	TServerObj(void);
	virtual ~TServerObj(void);
};
