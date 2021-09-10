#include "TThread.h"
unsigned int WINAPI TThread::HandleRunner(LPVOID parameter)
{
	TThread* pThread = (TThread*)parameter;	
	if( pThread )	pThread->Run();
	return 0;
}
bool TThread::Run()
{
	return true;
}
void TThread::CreateThread()
{
	if( m_bStarted==true ) return;
	m_hThread = _beginthreadex( NULL, 0, 
		HandleRunner, // 시작함수
		(LPVOID)this, // 인자값
		0,
		&m_iThreadID);
	m_bStarted = true;
}

TThread::TThread(void)
{
	m_bStarted = false;
}


TThread::~TThread(void)
{
	CloseHandle( (HANDLE)m_hThread );
}
