#include "TLock.h"



TLock::TLock(TNetBase*  pObj) : m_pObj(pObj)
{
	if (m_pObj !=nullptr)
	{
		EnterCriticalSection(&m_pObj->m_cs);
	}
}


TLock::~TLock()
{
	if (m_pObj != nullptr)
	{
		LeaveCriticalSection(&m_pObj->m_cs);
	}
}
