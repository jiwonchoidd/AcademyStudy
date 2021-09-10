#pragma once
#include "TServerStd.h"
#include "TRunTTy.h"

class TServerObj : public CObject
{
public:
	CRITICAL_SECTION m_cs;
public:
	TServerObj();
	virtual ~TServerObj();
};

