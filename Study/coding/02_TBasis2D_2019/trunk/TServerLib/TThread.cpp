#include "TThread.h"

unsigned int WINAPI TThread::HandleRunner(LPVOID prameter)
{
	TThread* ptThread = (TThread*)prameter;
	ptThread->Run();
	return 0;
}
void TThread::CreateThread()
{
	if (m_bStarted != false) return;
	m_hThread = _beginthreadex(NULL, 0, 
		HandleRunner, 
		(LPVOID)this,
		0, &m_iID);
	m_bStarted = true;
}
bool TThread::Run()
{
	return true;
}


TThread::TThread()
{
	m_hThread = 0;
	m_iID = 0;
	m_bStarted = false;
}


TThread::~TThread()
{
}
