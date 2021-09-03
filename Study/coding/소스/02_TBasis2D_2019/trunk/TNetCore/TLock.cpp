#include "TLock.h"

TLock::TLock(TServerObj * pObj) : m_pServerObj(pObj)
{
	if (m_pServerObj == nullptr) return;
	EnterCriticalSection(&m_pServerObj->m_cs);
}

TLock::~TLock()
{
	if (m_pServerObj == nullptr) return;
	LeaveCriticalSection(&m_pServerObj->m_cs);
}
