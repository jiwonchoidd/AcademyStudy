#include "KServer.h"

int KServer::Broadcast(KNetworkUser& user)
{
	if (user.m_lPacketPool.size() > 0)
	{
		std::list<KPacket>::iterator iter;
		for (iter = user.m_lPacketPool.begin();
			iter != user.m_lPacketPool.end();)
		{
			for (KNetworkUser& senduser : m_UserList)
			{
				int iRet = m_Net.SendMsg(senduser.m_ListenSocket, (*iter).m_uPacket);
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

bool KServer::Init(int port)
{
	//뮤텍스 생성
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
	//서버 소켓 초기화
	if (!m_Net.InitNetwork())
	{
		return false;
	}
	if (!m_Net.InitServer(SOCK_STREAM, port, nullptr))
	{
		return false;
	}

	std::cout << "Server Start." << std::endl;
	u_long on = 1;
	ioctlsocket(m_Net.m_ListenSocket, FIONBIO, &on);

	return true;
}

bool KServer::Run()
{
	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);

	while (1)
	{
		SOCKET clientSock = accept(m_Net.m_ListenSocket,
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
		else
		{
			KNetworkUser user;
			user.set(clientSock, clientAddr);
			WaitForSingleObject(m_hMutex, INFINITE);
			m_UserList.push_back(user);
			//LeaveCriticalSection(&g_CS);
			ReleaseMutex(m_hMutex);

			std::cout
				<< "ip =" << inet_ntoa(clientAddr.sin_addr)
				<< "port =" << ntohs(clientAddr.sin_port)
				<< "  " << std::endl;
			u_long on = 1;
			ioctlsocket(clientSock, FIONBIO, &on);
			std::cout << m_UserList.size() << " 명 접속중 " << std::endl;
		}
		Sleep(1);
	}
	return true;
}

bool KServer::Release()
{
	closesocket(m_Net.m_ListenSocket);
	WSACleanup();

	CloseHandle(m_hMutex);
	return true;
}
