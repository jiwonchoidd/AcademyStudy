#include "KServer.h"

//���� ��ŶǮ ����� ������� ��ü �������� ������. ��ε�ĳ��Ʈ
int KServer::Broadcast(KNetworkUser* user)
{
	if (user->m_lPacketPool.size() > 0)
	{
		std::list<KPacket>::iterator iter;
		for (iter = user->m_lPacketPool.begin();
			iter != user->m_lPacketPool.end();)
		{
			for (KNetworkUser* senduser : m_UserList)
			{
				int iRet = m_Net.SendMsg(senduser->m_Sock, (*iter).m_uPacket);
				if (iRet <= 0)
				{
					senduser->m_bConnect = false;
				}
			}
			iter = user->m_lPacketPool.erase(iter);
		}
	}
	return 1;
}

bool KServer::AddUser(SOCKET clientSock, SOCKADDR_IN clientAddr)
{


	return true;
}

//������ ��ɿ� �°Բ�.
bool KServer::Init(int port)
{
	//���ؽ� ����
	//m_hMutex = CreateMutex(NULL, FALSE, NULL);
	//���� ���� �ʱ�ȭ
	if (!m_Net.InitNetwork())
	{
		return false;
	}
	if (!m_Net.InitServer(SOCK_STREAM, port, nullptr))
	{
		return false;
	}

	std::cout << "\n::::::::::::::::Server Start.::::::::::::::::" << std::endl;
	u_long on = 1;
	ioctlsocket(m_Net.m_ListenSocket, FIONBIO, &on);

	return true;
}

bool KServer::Run()
{
	return true;
}

bool KServer::Release()
{
	closesocket(m_Net.m_ListenSocket);
	WSACleanup();

	return true;
}

KServer::KServer()
{
}

KServer::~KServer()
{
}
