#include "TThreadIO.h"
#include "TCompletionPort.h"
#include "TSessionContext.h"
//�Ӽ�->C/C++->��ó����->_WINSOCK_DEPRECATED_NO_WARNINGS;
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
	// �ִ� �ѵ��� ����� �ű� �޸𸮸� �����Ͽ� ��ȯ�ϸ�
	// ���� �ô� ���� �޸𸮸� �����ϰ� �ű� �޸𸮸� �����Ѵ�.
	SendContext* pA = new SendContext(pSession);
	for (int i = 0; i < 32; i++)
	{
		delete pOV[i];
		pOV[i] = nullptr;
	}
	delete pA;
	
	// delete�� �ݵ�� �ؾ��Ѵ�. �׷��߸� Ǯ�� �޸𸮸� �Ҹ��� �� �ִ�.
	SendContext::AllFree();

	// 2�� ����
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