#pragma once
#include "TServerObj.h"
class TLock
{
private:
	TServerObj*		m_pServerObj;
public:
	TLock(TServerObj* pObj);
	virtual ~TLock();
};

