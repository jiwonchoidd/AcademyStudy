#include "TSynchronize.h"

TSynchronize::TSynchronize(
	TServerObj* pObj): m_pSerObj(pObj)
{
	if( pObj == NULL ) return;
	EnterCriticalSection(&pObj->m_cs);
}


TSynchronize::~TSynchronize(void)
{
	if( m_pSerObj == NULL ) return;
	LeaveCriticalSection(&m_pSerObj->m_cs);
}
