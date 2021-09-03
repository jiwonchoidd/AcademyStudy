#include "TServerObj.h"

TServerObj::TServerObj(void)
{
	InitializeCriticalSection(&m_CS);
}

TServerObj::~TServerObj(void)
{
	DeleteCriticalSection(&m_CS);
}
