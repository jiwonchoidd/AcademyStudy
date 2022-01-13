#include "Sample.h"
#include "KServer.h"
DWORD WINAPI RecvThread(LPVOID lparam)
{
	KServer* server = (KServer*)lparam;
	SOCKET sock = server->m_Net.m_ListenSocket;
	while (1)
	{
		//�����尡 Ư�� �ñ׳��� �߻��Ҷ����� ��ٸ�
		WaitForSingleObject(server->m_hMutex, INFINITE);

		std::list<KNetworkUser>::iterator iter;
		for (iter = server->m_UserList.begin();
			iter != server->m_UserList.end();)
		{
			int iRet = server->m_Net.RecvUser(*iter);
			//0���� �۰ų� ����, �޾ƿ� �����Ͱ� ���ų�, ������ ��� 
			if (iRet <= 0)
			{
				iter = server->m_UserList.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		//�ü������ ����� �Ѱ���
		ReleaseMutex(server->m_hMutex);
		//�ǵ������� Context Switching
		Sleep(1);
	}
}

DWORD WINAPI SendThread(LPVOID lparam)
{
	KServer* pServer = (KServer*)lparam;
	SOCKET sock = pServer->m_Net.m_ListenSocket;
	while (1)
	{
		//�����尡 Ư�� �ñ׳��� �߻��Ҷ����� ��ٸ�
		WaitForSingleObject(pServer->m_hMutex, INFINITE);

		//�Ӱ豸�� ��ñ׳� ���°� �Ǿ� �ٸ� ���ؽ��� ��ٸ�
		std::list<KNetworkUser>::iterator iter;
		for (iter = pServer->m_UserList.begin();
			iter != pServer->m_UserList.end();)
		{
			//��ü���� ������
			int iRet = pServer->Broadcast(*iter);
			//0���� �۰ų� ����, �޾ƿ� �����Ͱ� ���ų�, ������ ��� 
			if (iRet <= 0)
			{
				iter = pServer->m_UserList.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		//�ü������ ����� �Ѱ���
		ReleaseMutex(pServer->m_hMutex);
		//�ǵ������� Context Switching
		Sleep(1);
	}
}

void main()
{
	setlocale(LC_ALL, ""); // Korean�ص� ������ ������ �ý��� ������������ ������ �Ѵ�.
	KServer server;
	server.Init(10000);
	
	DWORD idThread_Recv;
	DWORD idThread_Send;

	HANDLE hThread_Recv = ::CreateThread(0, 0,RecvThread ,(LPVOID)&server, 0, &idThread_Recv);
	HANDLE hThread_Send = ::CreateThread(0, 0,SendThread ,(LPVOID)&server, 0, &idThread_Send);
	CloseHandle(hThread_Recv);
	CloseHandle(hThread_Send);
	server.Run();
	server.Release();

	std::cout << "Server Close" << std::endl;

}