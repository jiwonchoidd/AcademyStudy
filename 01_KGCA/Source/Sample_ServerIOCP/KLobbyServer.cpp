#include "KLobbyServer.h"
DWORD WINAPI WorkerThread(LPVOID param)
{
	KLobbyServer* pServer = (KLobbyServer*)param;
	DWORD dwTransfer;
	ULONG_PTR KeyValue;
	OVERLAPPED* pOverlapped;
	while (1)
	{
		//지정된 킬이벤트가 신호를 받았다면 break
		if (WaitForSingleObject(pServer->m_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
		//완료큐에 데이터가 있으면 작업시작
		BOOL bRet = ::GetQueuedCompletionStatus(
			pServer->m_hIOCP, &dwTransfer, &KeyValue, &pOverlapped, 1);

		KNetworkUser* pUser = (KNetworkUser*)KeyValue;
		KOV* pOV = (KOV*)pOverlapped;

		if (bRet == TRUE && pUser && pOV)
		{
			//작업
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
			//오류
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

	SYSTEM_INFO system_info;

	GetSystemInfo(&system_info);

	//쓰레드 여러개 생성
	for (int i = 0; i < system_info.dwNumberOfProcessors * 2; i++)
	{
		DWORD id;
		//자기 자신의 서버 인자를 넘김. this 
		m_hWorkThread[i] = CreateThread(0, 0, WorkerThread, this, 0, &id);
	}

	return true;
}

//게임서버는 new delete를 막아 
//동적할당을 미리 메모리 메니져로 만들어서 해야함
bool KLobbyServer::AddUser(SOCKET clientSock, SOCKADDR_IN clientAddr)
{
	KNetworkUser* user = new KNetworkUser();
	user->Set(clientSock, clientAddr);

	u_long on = 1;
	ioctlsocket(clientSock, FIONBIO, &on);

	//유저리스트 추가
	EnterCriticalSection(&m_cs);
		m_UserList.push_back(user);
	LeaveCriticalSection(&m_cs);

	//비동기 작업을 해야하니까 유저가 접속이되면 리시브를 걸어놔라
	//유저에 대한 포인터를 넘긴다.
	::CreateIoCompletionPort((HANDLE)clientSock, m_hIOCP, (ULONG_PTR)user, 0);
	
	//WSARecv를 건다. 
	user->Recv();

	//delete user;
	return true;
}

bool KLobbyServer::Run()
{
	while (1)
	{
		//임계구역
		EnterCriticalSection(&m_cs);

		//패킷 타입을 판별해서 해당 맞는 작업을 하면 됨
		std::list<KPacket>::iterator iter_packet;
		for (iter_packet = m_lPacketPool.begin();
			iter_packet != m_lPacketPool.end();)
		{
			switch ((*iter_packet).m_uPacket.ph.type)
			{
				case PACKET_USER_POSITION:
				{
					std::cout << "d";
				}break;
				case PACKET_CHAT_MSG:
				{
					std::cout << "d";
				}break;
			}
		}

		//주기적인 동기화
		for (KNetworkUser* user : m_UserList)
		{
			if (user->m_lPacketPool.size() > 0)
			{
				Broadcast(user);
			}
		}
		//커넥트가 false면 나가는 처리까지
		std::list<KNetworkUser*>::iterator user_iter;
		for (user_iter = m_UserList.begin();
			user_iter != m_UserList.end();)
		{
			if ((*user_iter)->m_bConnect == false)
			{
				(*user_iter)->Disconnect();
				delete (*user_iter);
				user_iter = m_UserList.erase(user_iter);
				std::cout <<"\nCurrent : " << m_UserList.size() <<" 명 접속중.."<<std::endl;
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
