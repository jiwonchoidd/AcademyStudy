#pragma once
#include "TServerObj.h"
class TLock
{
	TServerObj*  m_pObj;
public:
	TLock(TServerObj* pObj);
	virtual ~TLock();
};

