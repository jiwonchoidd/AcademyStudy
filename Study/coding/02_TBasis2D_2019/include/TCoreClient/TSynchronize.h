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

class TSynchronize : public TServerObj
{
public:
	TServerObj* m_pSerObj;
public:
	TSynchronize(TServerObj* pObj);
	virtual ~TSynchronize(void);
};

