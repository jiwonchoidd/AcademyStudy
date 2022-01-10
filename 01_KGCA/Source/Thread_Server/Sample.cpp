#include "Sample.h"

//����
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
		//�����尡 Ư�� �ñ׳��� �߻��Ҷ����� ��ٸ�
		WaitForSingleObject(g_hMutex, INFINITE);

		std::list<KNetworkUser>::iterator iter;
		for (iter = g_UserList.begin();
			iter != g_UserList.end();)
		{
			int iRet = g_Net.RecvUser(*iter);
			//0���� �۰ų� ����, �޾ƿ� �����Ͱ� ���ų�, ������ ��� 
			if (iRet <= 0)
			{
				iter = g_UserList.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		//�ü������ ����� �Ѱ���
		ReleaseMutex(g_hMutex);
		//�ǵ������� Context Switching
		Sleep(1);
	}
}

DWORD WINAPI SendThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	while (1)
	{
		//�����尡 Ư�� �ñ׳��� �߻��Ҷ����� ��ٸ�
		WaitForSingleObject(g_hMutex, INFINITE);

		std::list<KNetworkUser>::iterator iter;
		for (iter = g_UserList.begin();
			iter != g_UserList.end();)
		{
			//��ü���� ������
			int iRet = BroadCast(*iter);
			//0���� �۰ų� ����, �޾ƿ� �����Ͱ� ���ų�, ������ ��� 
			if (iRet <= 0)
			{
				iter = g_UserList.erase(iter);
			}
			else
			{
				iter++;
			}
		}
		//�ü������ ����� �Ѱ���
		ReleaseMutex(g_hMutex);
		//�ǵ������� Context Switching
		Sleep(1);
	}
}

void main()
{
	//������� ����ȭ ��� ũ��Ƽ�ü���
	//InitializeCriticalSection(&g_CS);
	g_hMutex = CreateMutex(NULL, FALSE, NULL);
	g_Net.InitNetwork();
	g_Net.InitServer(SOCK_STREAM,10000, nullptr);

	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);

	std::cout<< "Server Start." << std::endl;

	//non blocking socket 0�̸� ���ŷ ����
	u_long on = 1;
	ioctlsocket(g_Net.m_Sock, FIONBIO, &on);

	DWORD ThreadID_Recv;
	//������ �޴� ������
	HANDLE hThreadRecv = ::CreateThread(
		0,
		0,
		RecvThread, // �����Լ��� ����
		(LPVOID)g_Net.m_Sock, // �����Լ� ���ڰ�
		0, // �ٷ� �����Ұ����� �÷���
		&ThreadID_Recv // ������ ���̵� ��ȯ
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

	//���� ������
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
		//Ŭ���̾�Ʈ�� ���� �� ������
		else
		{
			KNetworkUser user;
			user.set(clientSock, clientAddr);
			//���� �����尡 Ư�� �ñ׳��� �߻��Ҷ����� ��ٸ�
			WaitForSingleObject(g_hMutex, INFINITE);
			//���� ���� ���� ����Ʈ�� �߰�
			g_UserList.push_back(user);
			ReleaseMutex(g_hMutex);
			std::cout
				<< "ip =" << inet_ntoa(clientAddr.sin_addr)
				<< "port =" << ntohs(clientAddr.sin_port)
				<< "  " << std::endl;
			u_long on = 1;
			ioctlsocket(clientSock, FIONBIO, &on);
			std::cout << std::to_string(g_UserList.size())<< " �� ������." << std::endl;
		}
		Sleep(1);
	}
	closesocket(g_Net.m_Sock);
	WSACleanup();

	CloseHandle(g_hMutex);
}