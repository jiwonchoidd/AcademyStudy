#include "KLobbyServer.h"
DWORD WINAPI WorkerThread(LPVOID param)
{
	KLobbyServer* pServer = (KLobbyServer*)param;
	DWORD dwTransfer;
	ULONG_PTR KeyValue;
	OVERLAPPED* pOverlapped;
	while (1)
	{
		//������ ų�̺�Ʈ�� ��ȣ�� �޾Ҵٸ� break
		if (WaitForSingleObject(pServer->m_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
		//�Ϸ�ť�� �����Ͱ� ������ �۾�����
		BOOL bRet = ::GetQueuedCompletionStatus(
			pServer->m_hIOCP, &dwTransfer, &KeyValue, &pOverlapped, 1);

		KNetworkUser* pUser = (KNetworkUser*)KeyValue;
		KOV* pOV = (KOV*)pOverlapped;

		if (bRet == TRUE && pUser && pOV)
		{
			//�۾�
			if (pOV->type == 1000 && dwTransfer == 0)
			{
				if(pUser->m_bConnect == true)
				pUser->m_bConnect = false;
			}
			else
			{
				pUser->Dispatch(dwTransfer, pOV);
			}
		}
		else
		{
			//����
			if (GetLastError() != WAIT_TIMEOUT)
			{
				::SetEvent(pServer->m_hKillEvent);
				break;
			}
		}
	}
	return TRUE;
}
//���Ӽ����� new delete�� ���� 
//�����Ҵ��� �̸� �޸� �޴����� ���� �ؾ���
bool KLobbyServer::AddUser(SOCKET clientSock, SOCKADDR_IN clientAddr)
{
	KNetworkUser* user = new KNetworkUser();
	user->Set(clientSock, clientAddr);

	u_long on = 1;
	ioctlsocket(clientSock, FIONBIO, &on);

	//��������Ʈ �߰�
	EnterCriticalSection(&m_cs);
		m_UserList.push_back(user);
	LeaveCriticalSection(&m_cs);

	//�񵿱� �۾��� �ؾ��ϴϱ� ������ �����̵Ǹ� ���ú긦 �ɾ����
	//������ ���� �����͸� �ѱ��.
	::CreateIoCompletionPort((HANDLE)clientSock, m_hIOCP, (ULONG_PTR)user, 0);
	user->Recv();

	return true;
}
bool KLobbyServer::Init(int port)
{
	KServer::Init(port);
	m_hIOCP = CreateIoCompletionPort(
			INVALID_HANDLE_VALUE, 0, 0, 0);

	//������ ������ ����
	for (int i = 0; i < MAX_WORKER_THREAD; i++)
	{
		DWORD id;
		//�ڱ� �ڽ��� ���� ���ڸ� �ѱ�. this 
		m_hWorkThread[i] = CreateThread(0, 0, WorkerThread, this, 0, &id);
	}
	return true;
}

bool KLobbyServer::Run()
{
	while (1)
	{
		//�Ӱ豸��
		EnterCriticalSection(&m_cs);
		for (KNetworkUser* user : m_UserList)
		{
			if (user->m_lPacketPool.size() > 0)
			{
				Broadcast(user);
			}
		}
		//Ŀ��Ʈ�� false�� ������ ó������
		std::list<KNetworkUser*>::iterator iter;
		for (iter = m_UserList.begin(); 
			iter != m_UserList.end();)
		{
			if ((*iter)->m_bConnect == false)
			{
				(*iter)->Disconnect();
				delete (*iter);
				iter = m_UserList.erase(iter);
				std::cout <<"current : " << m_UserList.size() <<" �� ������.."<<std::endl;
			}
			else
			{
				iter++;
			}
		}
		LeaveCriticalSection(&m_cs);
	}
	return true;
}

bool KLobbyServer::Release()
{
	CloseHandle(m_hIOCP);
	KServer::Release();
	return true;
}
