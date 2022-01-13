#include "Sample.h"
#include "KServer.h"
DWORD WINAPI RecvThread(LPVOID lparam)
{
	KServer* server = (KServer*)lparam;
	SOCKET sock = server->m_Net.m_ListenSocket;
	while (1)
	{
		//스레드가 특정 시그널이 발생할때까지 기다림
		WaitForSingleObject(server->m_hMutex, INFINITE);

		std::list<KNetworkUser>::iterator iter;
		for (iter = server->m_UserList.begin();
			iter != server->m_UserList.end();)
		{
			int iRet = server->m_Net.RecvUser(*iter);
			//0보다 작거나 같음, 받아온 데이터가 없거나, 에러일 경우 
			if (iRet <= 0)
			{
				iter = server->m_UserList.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		//운영체제한테 제어권 넘겨줌
		ReleaseMutex(server->m_hMutex);
		//의도적으로 Context Switching
		Sleep(1);
	}
}

DWORD WINAPI SendThread(LPVOID lparam)
{
	KServer* pServer = (KServer*)lparam;
	SOCKET sock = pServer->m_Net.m_ListenSocket;
	while (1)
	{
		//스레드가 특정 시그널이 발생할때까지 기다림
		WaitForSingleObject(pServer->m_hMutex, INFINITE);

		//임계구역 논시그널 상태가 되어 다른 뮤텍스는 기다림
		std::list<KNetworkUser>::iterator iter;
		for (iter = pServer->m_UserList.begin();
			iter != pServer->m_UserList.end();)
		{
			//전체에게 보내줌
			int iRet = pServer->Broadcast(*iter);
			//0보다 작거나 같음, 받아온 데이터가 없거나, 에러일 경우 
			if (iRet <= 0)
			{
				iter = pServer->m_UserList.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		//운영체제한테 제어권 넘겨줌
		ReleaseMutex(pServer->m_hMutex);
		//의도적으로 Context Switching
		Sleep(1);
	}
}

void main()
{
	setlocale(LC_ALL, ""); // Korean해도 되지만 공백은 시스템 지역설정으로 따르게 한다.
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