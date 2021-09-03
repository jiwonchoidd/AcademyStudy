#pragma once
#include "TNetBase.h"
class TLock
{
	TNetBase*  m_pObj;
public:
	TLock(TNetBase*  pObj);
	virtual ~TLock();
};

