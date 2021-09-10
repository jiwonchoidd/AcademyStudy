#include "TThreadIO.h"
#include "TCompletionPort.h"
#include "TSessionContext.h"
//속성->C/C++->전처리기->_WINSOCK_DEPRECATED_NO_WARNINGS;
//https://github.com/zeliard/GSP
void main()
{	
	Session* pSession = new Session(0, 1024, 1024);
	Overlapped* pOV[32];
	//SendContext* pA[32];
	for (int i = 0; i < 32; i++)
	{
		pOV[i] = new Overlapped(new SendContext(pSession));
	}

	for (int i = 0; i < 32; i++)
	{
		delete pOV[i];
		pOV[i] = nullptr;
	}
	for (int i = 0; i < 32; i++)
	{
		pOV[i] = new Overlapped(new SendContext(pSession));
	}
	// 최대 한도를 벗어나면 신규 메모리를 생성하여 반환하며
	// 삭제 시는 이전 메모리를 삭제하고 신규 메모리를 저장한다.
	SendContext* pA = new SendContext(pSession);
	for (int i = 0; i < 32; i++)
	{
		delete pOV[i];
		pOV[i] = nullptr;
	}
	delete pA;
	
	// delete를 반드시 해야한다. 그래야만 풀의 메모리를 소멸할 수 있다.
	SendContext::AllFree();

	// 2차 검증
	/*char szRead[50] = { 0, };
	char szData[] = "aadfdfdfaffdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdfdffdad";
	CircularBuffer cb(128);
	if (cb.write(szData, strlen(szData)) == false)
	{		
		cb.read(szRead, strlen(szData));
	}
	if (cb.write(szData, strlen(szData)) == false)
	{
		cb.read(szRead, strlen(szData));
	}
	if (cb.write(szData, strlen(szData)) == false)
	{
		cb.read(szRead, strlen(szData));
	}
	if (cb.write(szData, strlen(szData)) == false)
	{
		cb.read(szRead, strlen(szData));
	}
	cb.read(szRead, strlen(szData));
	if (cb.write(szData, strlen(szData)) == false)
	{
		cb.read(szRead, strlen(szData));
	}

	Socket listensock(ConnectType::TCP);
	Session session(nullptr, 1024, 1024);
	session.preRecv();
	session.acceptCompletion(&listensock);
	session.recv();
	CompletionPort cp(0);
	IOThread io(&cp);
	io.run();*/
}