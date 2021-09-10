#include "TServerObj.h"


TServerObj::TServerObj(void)
{
	InitializeCriticalSection(&m_cs);
}


TServerObj::~TServerObj(void)
{
	DeleteCriticalSection(&m_cs);
}
