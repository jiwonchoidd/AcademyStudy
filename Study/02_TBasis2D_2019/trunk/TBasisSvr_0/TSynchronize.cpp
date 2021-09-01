#include "TSynchronize.h"

TSynchronize::TSynchronize(TServerObj* pSvrObj):
                             m_pServerObj( pSvrObj )
{
	if( m_pServerObj ==NULL ) return;
	EnterCriticalSection( &m_pServerObj->m_CS );
}

TSynchronize::~TSynchronize(void)
{
	if( m_pServerObj==NULL ) return;
	LeaveCriticalSection( &m_pServerObj->m_CS );
}
