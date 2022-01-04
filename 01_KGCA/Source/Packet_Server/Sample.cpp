#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <list>
#include <string>
#include "KPacket.h"
#pragma comment	(lib, "ws2_32.lib")

//����
struct User
{
	SOCKET		m_Sock;
	SOCKADDR_IN m_Addr;
	std::string m_csName;
	short       m_iPort;
	void set(SOCKET sock, SOCKADDR_IN addr)
	{
		m_Sock = sock;
		m_Addr = addr;
		//��Ʈ��ũ �ּ� ��ȯ �Լ� �򿣵��(32bit) ->  
		m_csName = inet_ntoa(addr.sin_addr);
		//������� �޸� ���ӵ� ����� �迭�ϴ� ������� 
		// Network Byte ������ To Host�� Byte ������ �ٲ�
		m_iPort = ntohs(addr.sin_port);
	}
};

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

void main()
{
	WSADATA wsa;
	//2.2 ���� WSA 
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	SOCKET ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	//���Ͽ� �ּ� �Ҵ�
	int iRet = bind(ListenSock, (sockaddr*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)  return;
	//Ŭ���̾�Ʈ�� ���� ���� ��û�� ó���Ҽ��ִ� ���¸� ���� 
	iRet = listen(ListenSock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)  return;

	SOCKADDR_IN clientAddr;
	int iLen = sizeof(clientAddr);

	std::cout<< "Server Start." << std::endl;

	//non blocking socket 0�̸� ���ŷ ����
	u_long on = 1;
	ioctlsocket(ListenSock, FIONBIO, &on);

	std::list<User> userlist;

	while (1)
	{
		SOCKET clientSock = accept(ListenSock,
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
			User user;
			user.set(clientSock, clientAddr);
			userlist.push_back(user);
			// �� 
			std::cout
				<< "ip =" << inet_ntoa(clientAddr.sin_addr)
				<< "port =" << ntohs(clientAddr.sin_port)
				<< "  " << std::endl;
			u_long on = 1;
			ioctlsocket(clientSock, FIONBIO, &on);
			std::cout << userlist.size() << " �� ������." << std::endl;
		}

		if (userlist.size() > 0)
		{
			std::list<User>::iterator iter;
			for (iter = userlist.begin(); iter != userlist.end(); )
			{
				User user = *iter;
				char szRecvBuffer[256] = { 0, };
				//��Ŷ��� �ޱ�
				UPACKET recvPacket;
				ZeroMemory(&recvPacket, sizeof(recvPacket));
				int iRecvSize = 0;
				do {
					int iRecvByte = recv(user.m_Sock, szRecvBuffer,
						PACKET_HEADER_SIZE, 0);
					iRecvSize += iRecvByte;
					if (iRecvByte == 0)
					{
						closesocket(user.m_Sock);
						iter = userlist.erase(iter);
						std::cout << user.m_csName << " ���������." << std::endl;
						continue;
					}
					if (iRecvByte == SOCKET_ERROR)
					{
						int iError = WSAGetLastError();
						if (iError != WSAEWOULDBLOCK)
						{
							iter = userlist.erase(iter);
							std::cout << user.m_csName << " ������ ���������." << std::endl;
							break;
						}
						else
						{
							break;
						}
					}
				} while (iRecvSize < PACKET_HEADER_SIZE);

				if (iRecvSize == SOCKET_ERROR)
				{
					if (iter != userlist.end())
					{
						iter++;
					}
					continue;
				}

				memcpy(&recvPacket.ph, szRecvBuffer, PACKET_HEADER_SIZE);
				// ������ �ޱ�
				iRecvSize = 0;
				do {
					int iRecvByte = recv(user.m_Sock, recvPacket.msg,
						recvPacket.ph.len - PACKET_HEADER_SIZE - iRecvSize, 0);
					iRecvSize += iRecvByte;
					if (iRecvByte == 0)
					{
						closesocket(user.m_Sock);
						iter = userlist.erase(iter);
						std::cout << user.m_csName << " ���������." << std::endl;
						continue;
					}
					if (iRecvByte == SOCKET_ERROR)
					{
						int iError = WSAGetLastError();
						if (iError != WSAEWOULDBLOCK)
						{
							iter = userlist.erase(iter);
							std::cout << user.m_csName << " ������ ���������." << std::endl;
						}
						else
						{
							iter++;
						}
					}
				} while (iRecvSize < recvPacket.ph.len - PACKET_HEADER_SIZE);

				KPacket data;
				data.m_uPacket = recvPacket;
				KChatting recvdata;
				ZeroMemory(&recvdata, sizeof(recvdata));
				data >> recvdata.index >> recvdata.name
					>> recvdata.damage >> recvdata.message;

				std::cout << "\n" <<
					"[" << recvdata.name << "]"
					<< recvdata.message;

				// ��Ŷ �ϼ�		
				std::list<User>::iterator iterSend;
				for (iterSend = userlist.begin();
					iterSend != userlist.end(); )
				{
					int iSendMsgSize = SendMsg(user.m_Sock, recvPacket);
					if (iSendMsgSize < 0)
					{
						closesocket(user.m_Sock);
						iterSend = userlist.erase(iterSend);
						std::cout << user.m_csName << " ������ ���������." << std::endl;
					}
					else
					{
						iterSend++;
					}
				}
				if (iter != userlist.end())
				{
					iter++;
				}
			}
		}
	}
	closesocket(ListenSock);
	WSACleanup();
}