#include "TNetBase.h"



TNetBase::TNetBase()
{
	InitializeCriticalSection(&m_cs);
}


TNetBase::~TNetBase()
{
	DeleteCriticalSection(&m_cs);
}
