#include "KNetwork.h"

//���� Select ��
std::list<KNetworkUser> userlist;
int SendMsg(SOCKET sock, char* msg, WORD type)
{
	//1. ��Ŷ�� ��ü ����
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	//��Ŷ�� ���̴� ��� ������ + �޼��� ������
	packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
	packet.ph.type = type;
	//�޼��� �����ɷ� memcpy�� ��Ŷ ����
	memcpy(packet.msg, msg, strlen(msg));

	// 2�� ��Ŷ ���� : �ü�� sendbuffer(short����Ʈ)
	char* pMsg = (char*)&packet;
	int iSize = 0;

	do {
		//TCP�� ������������ ����Ʈ ������ ������ �������� ����
		int iSendByte = send(sock, &pMsg[iSize],
			packet.ph.len - iSize, 0);

		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}

		iSize += iSendByte;
	} while (iSize < packet.ph.len);
	return iSize;
}
//��Ŷ�� �޾�����.
int SendMsg(SOCKET sock, UPACKET& packet)
{
	char* pMsg = (char*)&packet;
	int iSize = 0;
	do {
		int iSendByte = send(sock, &pMsg[iSize],
			packet.ph.len - iSize, 0);

		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}

		iSize += iSendByte;
	} while (iSize < packet.ph.len);
	return iSize;
}
int AddUser(SOCKET sock)
{
	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);
	SOCKET clientSock = accept(sock,
		(sockaddr*)&clientAddr, &iLen);
	if (clientSock == SOCKET_ERROR)
	{
		return -1;
	}
	else
	{
		KNetworkUser user;
		user.set(clientSock, clientAddr);
		userlist.push_back(user);
		std::cout
			<< "ip =" << inet_ntoa(clientAddr.sin_addr)
			<< "port =" << ntohs(clientAddr.sin_port)
			<< "  " << std::endl;
		std::cout << userlist.size() << " �� ������.." << std::endl;
	}
	return 1;
}
int RecvUser(KNetworkUser& user)
{
	char szRecvBuffer[1024] = { 0, };
	int iRecvByte = recv(user.m_Sock, szRecvBuffer, 1024, 0);
	if (iRecvByte == 0)
	{
		return 0;
	}
	if (iRecvByte == SOCKET_ERROR)
	{
		return -1;
	}
	user.DispatchRead(szRecvBuffer, iRecvByte);
	return 1;
}
void main()
{
	KNetwork net;
	net.InitNetwork();
	net.InitServer(SOCK_STREAM, 10000);

	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);

	std::cout<< "Server Start." << std::endl;

	FD_SET	rSet;
	FD_SET	wSet;
	timeval timeOut;

	timeOut.tv_sec = 1;
	timeOut.tv_usec = 0;

	while (1)
	{
		//�Ź� �ʱ�ȭ ����ü�� �ֿ� ������ �迭 
		FD_ZERO(&rSet);
		FD_ZERO(&wSet);
		//���� ���Ͽ� rset�� ������
		FD_SET(net.m_ListenSocket, &rSet);

		std::list<KNetworkUser>::iterator useriter;
		for (useriter = userlist.begin();
			useriter != userlist.end();)
		{
			//���� �߿� ���� ���� ����� �ִٸ� ����Ʈ���� ������
			if ((*useriter).m_bConnect == false)
			{
				std::cout << (*useriter).m_csName << " ���������." << std::endl;
				useriter = userlist.erase(useriter);
				continue;
			}
			//������ ������ rset�� ������
			FD_SET((*useriter).m_Sock, &rSet);
			// ���� user���� ���� ��Ŷ�� ������ wSet ����ü�� ���� ����
			if ((*useriter).m_lPacketPool.size() > 0)
			{
				FD_SET((*useriter).m_Sock, &wSet);
			}
			useriter++;
		}
		//select ���� ���
		//select �Լ��� fd_set ����ü�� �Ҵ���
		// FD�� �̺�Ʈ�� �߻��ϸ� �̸� �����ϰ�
		// � FD �̺�Ʈ�� �߻��ߴ��� �˷��ش�.
		int iRet = select(0,
			&rSet,
			&wSet,
			nullptr,
			&timeOut);

		//�ƹ� Ȱ���� ������ 0�� ��ȯ�Ѵ�.
		std::cout << ".";
		if (iRet == 0)
		{
			continue;
		}
		//fdset�� ���� fd�� �ش��ϴ� ��Ʈ�� ��Ʈ�Ǿ� ������ ������� fd�� �����Ѵ�.
		if (FD_ISSET(net.m_ListenSocket, &rSet))
		{
			if (AddUser(net.m_ListenSocket) <= 0)
			{
				break;
			}
		}
		//���� ����Ʈ�� ���鼭 �ش� ���� ������ ����Ǿ� �ִٸ�
		for (KNetworkUser& user : userlist)
		{
			//���� ������� Ȯ�� ���⼭�� �Ұ� üũ��
			if (FD_ISSET(user.m_Sock, &rSet))
			{
				int iRet = RecvUser(user);
				if (iRet <= 0)
				{
					user.m_bConnect = false;
				}
			}
		}
		for (KNetworkUser& user : userlist)
		{
			if (FD_ISSET(user.m_Sock, &wSet))
			{
				if (user.m_lPacketPool.size() > 0)
				{
					std::list<KPacket>::iterator iter;
					for (iter = user.m_lPacketPool.begin();
						iter != user.m_lPacketPool.end(); )
					{
						for (KNetworkUser& senduser : userlist)
						{
							int iRet = SendMsg(senduser.m_Sock, (*iter).m_uPacket);
							if (iRet <= 0)
							{
								senduser.m_bConnect = false;
							}
						}
						iter = user.m_lPacketPool.erase(iter);
					}
				}
			}
		}
	}
	net.CloseNetwork();
}