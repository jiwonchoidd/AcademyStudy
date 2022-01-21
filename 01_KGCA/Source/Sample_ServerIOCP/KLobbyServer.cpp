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
			if (dwTransfer == 0)
			{
				pOV->type == KOV::MODE_EXIT;
				if(pUser->m_bConnect == true)
				pUser->m_bConnect = false;
				//����� �� �ƴѵ� ť�� ��ȯ�� �� ����
				//PostQueued
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

bool KLobbyServer::Init(int port)
{
	KServer::Init(port);
	m_hIOCP = CreateIoCompletionPort(
			INVALID_HANDLE_VALUE, 0, 0, 0);

	//SYSTEM_INFO system_info;

	//GetSystemInfo(&system_info);

	//������ ������ ����
	for (int i = 0; i < MAX_WORKER_THREAD; i++)
	{
		DWORD id;
		//�ڱ� �ڽ��� ���� ���ڸ� �ѱ�. this 
		m_hWorkThread[i] = CreateThread(0, 0, WorkerThread, this, 0, &id);
	}

	return true;
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
	
	//WSARecv�� �Ǵ�. 
	user->Recv();

	//delete user;
	return true;
}

bool KLobbyServer::Run()
{
	while (1)
	{
		//�Ӱ豸��
		EnterCriticalSection(&m_cs);

		//��Ŷ Ÿ���� �Ǻ��ؼ� �ش� �´� �۾��� �ϸ� ��
		std::list<KPacket>::iterator iter_packet;
		for (iter_packet = m_lPacketPool.begin();
			iter_packet != m_lPacketPool.end();)
		{
			switch ((*iter_packet).m_uPacket.ph.type)
			{
				case PACKET_USER_POSITION:
				{
					std::cout << "";

				}break;
				case PACKET_CHAT_MSG:
				{
					std::cout << "";
				}break;
			}
		}

		//�ֱ����� ����ȭ
		for (KNetworkUser* user : m_UserList)
		{
			if (user->m_lPacketPool.size() > 0)
			{
				Broadcast(user);
			}
		}
		//Ŀ��Ʈ�� false�� ������ ó������
		std::list<KNetworkUser*>::iterator user_iter;
		for (user_iter = m_UserList.begin();
			user_iter != m_UserList.end();)
		{
			if ((*user_iter)->m_bConnect == false)
			{
				DelUser((*user_iter));
				delete (*user_iter);
				user_iter = m_UserList.erase(user_iter);
				std::cout <<"\nCurrent : " << m_UserList.size() <<" �� ������.."<<std::endl;
			}
			else
			{
				user_iter++;
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
