#include "TThread.h"

TThread::TThread(void) : m_bStarted(false)
{	
}
TThread::~TThread(void)
{
	CloseHandle((HANDLE)m_hThread);
}
unsigned int  WINAPI TThread::HandleRunner( LPVOID parameter )
{
	TThread* pThread = (TThread*)parameter;
	pThread->Run();
	return 0;
}
void TThread::CreateThread()
{
	if( m_bStarted == true ) return;
	// 유저 당 1개 스레드 붙임.
	//unsigned int dwThread;
	//int hThreadID = 
	m_hThread = _beginthreadex(NULL, 0, HandleRunner,
					(LPVOID)this, 0, &m_iThreadID );				
	

	
	//m_hThread = ::CreateThread( 0, 0,  HandleRunner, this, 0, &m_iThreadID );
	m_bStarted = true;
}

