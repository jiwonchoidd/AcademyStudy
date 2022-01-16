#include "KServerObj.h"

KServerObj::KServerObj()
{
	InitializeCriticalSection(&m_cs);
}

KServerObj::~KServerObj()
{
	DeleteCriticalSection(&m_cs);
}
