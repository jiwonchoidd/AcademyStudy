#pragma once
#include "TServerObj.h"
class TSynchronize
{
public:	
	TServerObj* m_pSerObj;
public:
	TSynchronize( TServerObj* pObj );
	virtual ~TSynchronize(void);
};

