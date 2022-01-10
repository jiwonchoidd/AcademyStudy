#include "Sample.h"

//서버
CRITICAL_SECTION		g_CS;
HANDLE					g_hMutex;
std::list<KNetworkUser> g_UserList;
KNetwork				g_Net;

int BroadCast(KNetworkUser& user)
{
	if (user.m_lPacketPool.size() > 0)
	{
		std::list<KPacket>::iterator iter;
		for (iter = user.m_lPacketPool.begin();
			iter != user.m_lPacketPool.end();)
		{
			for (KNetworkUser& senduser : g_UserList)
			{
				int iRet = g_Net.SendMsg(senduser.m_Sock, (*iter).m_uPacket);
				if (iRet <= 0)
				{
					senduser.m_bConnect = false;
				}
			}
			iter = user.m_lPacketPool.erase(iter);
		}
	}
	return 1;
}
DWORD WINAPI RecvThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	while (1)
	{
		//스레드가 특정 시그널이 발생할때까지 기다림
		WaitForSingleObject(g_hMutex, INFINITE);

		std::list<KNetworkUser>::iterator iter;
		for (iter = g_UserList.begin();
			iter != g_UserList.end();)
		{
			int iRet = g_Net.RecvUser(*iter);
			//0보다 작거나 같음, 받아온 데이터가 없거나, 에러일 경우 
			if (iRet <= 0)
			{
				iter = g_UserList.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		//운영체제한테 제어권 넘겨줌
		ReleaseMutex(g_hMutex);
		//의도적으로 Context Switching
		Sleep(1);
	}
}

DWORD WINAPI SendThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	while (1)
	{
		//스레드가 특정 시그널이 발생할때까지 기다림
		WaitForSingleObject(g_hMutex, INFINITE);

		std::list<KNetworkUser>::iterator iter;
		for (iter = g_UserList.begin();
			iter != g_UserList.end();)
		{
			//전체에게 보내줌
			int iRet = BroadCast(*iter);
			//0보다 작거나 같음, 받아온 데이터가 없거나, 에러일 경우 
			if (iRet <= 0)
			{
				iter = g_UserList.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		//운영체제한테 제어권 넘겨줌
		ReleaseMutex(g_hMutex);
		//의도적으로 Context Switching
		Sleep(1);
	}
}

void main()
{
	//유저모드 동기화 모드 크리티컬섹션
	//InitializeCriticalSection(&g_CS);
	g_hMutex = CreateMutex(NULL, FALSE, NULL);
	g_Net.InitNetwork();
	g_Net.InitServer(SOCK_STREAM,10000, nullptr);

	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);

	std::cout<< "Server Start." << std::endl;

	//non blocking socket 0이면 블락킹 소켓
	u_long on = 1;
	ioctlsocket(g_Net.m_Sock, FIONBIO, &on);

	DWORD ThreadID_Recv;
	//데이터 받는 스레드
	HANDLE hThreadRecv = ::CreateThread(
		0,
		0,
		RecvThread, // 시작함수를 지정
		(LPVOID)g_Net.m_Sock, // 시작함수 인자값
		0, // 바로 시작할것인지 플래그
		&ThreadID_Recv // 스레드 아이디 반환
	);
	CloseHandle(hThreadRecv);
	
	DWORD ThreadID_Send;
	HANDLE hThreadSend = ::CreateThread(
		0,
		0,
		SendThread,
		(LPVOID)g_Net.m_Sock,
		0,
		&ThreadID_Send
	);
	CloseHandle(hThreadSend);

	//메인 스레드
	while (1)
	{
		SOCKET clientSock = accept(g_Net.m_Sock,
			(sockaddr*)&clientAddr, &iLen);

		if (clientSock == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				std::cout << "ErrorCode=" << iError << std::endl;
				break;
			}
		}
		//클라이언트가 접속 시 시작함
		else
		{
			KNetworkUser user;
			user.set(clientSock, clientAddr);
			//메인 스레드가 특정 시그널이 발생할때까지 기다림
			WaitForSingleObject(g_hMutex, INFINITE);
			//들어온 유저 전역 리스트에 추가
			g_UserList.push_back(user);
			ReleaseMutex(g_hMutex);
			std::cout
				<< "ip =" << inet_ntoa(clientAddr.sin_addr)
				<< "port =" << ntohs(clientAddr.sin_port)
				<< "  " << std::endl;
			u_long on = 1;
			ioctlsocket(clientSock, FIONBIO, &on);
			std::cout << std::to_string(g_UserList.size())<< " 명 접속중." << std::endl;
		}
		Sleep(1);
	}
	closesocket(g_Net.m_Sock);
	WSACleanup();

	CloseHandle(g_hMutex);
}