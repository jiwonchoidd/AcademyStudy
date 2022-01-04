#define ServerIP "192.168.0.56"
#define ServerIP2 "192.168.0.12"
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <conio.h>
#include "KPacket.h"
#pragma comment	(lib, "ws2_32.lib")
int SendMsg(SOCKET sock, char* msg, WORD type);
int SendPacket(SOCKET sock, char* msg, WORD type);
int RecvPacketHeader(SOCKET sock, UPACKET& recvPacket);
int RecvPacketData(SOCKET sock, UPACKET& recvPacket);
//Ŭ���̾�Ʈ
void main()
{
	/*WSACleanup �Լ��� ���� �̷� ���� ���α׷��� ���۰� ���� ��Ÿ��*/
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return;
	}
	//TCP - SOCK_STREAM , UDP - SOCK_DGRAM
	SOCKET sock = socket(AF_INET,
		SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = inet_addr(ServerIP);
	int iRet = connect(
		sock,
		(sockaddr*)&sa,
		sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}
	std::cout << "���Ӽ���!" << std::endl;

	//����ŷ ���� 0�� �ƴ϶�� ����ŷ ���� ����
	u_long on = 1;
	ioctlsocket(sock, FIONBIO, &on);

	char szBuffer[256] = { 0, };
	int iEnd = 0;
	while (1)
	{
		if (_kbhit() == 1)
		{
			int iValue = _getche();
			// ���͸� �������� �׸��� ���ۿ� ���� ������
			if (iValue == '\r' && strlen(szBuffer) == 0)
			{
				std::cout << "����Ǿ����ϴ�." << std::endl;
				break;
			}
			// ���͸� ��������
			if (iValue == '\r')
			{
				// send msg() ����ü �״�� �����ϴ� ���
				// ��ſ� ��Ŷ�� �̿��ؼ� ����
				// ��Ŷ�� ����ϸ� ������ �޸𸮷�
				// �������� �ݸ� ����ü�� �ִ� ���̷� 
				// �޸� ���񰡵�
				int iSendByte = SendPacket(sock, szBuffer, PACKET_CHAT_MSG);
				if (iSendByte < 0)
				{
					std::cout << "�������� ����" << std::endl;
					break;
				}
				iEnd = 0;
				ZeroMemory(szBuffer, sizeof(char) * 256);
			}
			else
			{
				szBuffer[iEnd++] = iValue;
			}
		}
		else
		{
			// Recv
			UPACKET packet;
			int iRet = RecvPacketHeader(sock, packet);
			if (iRet < 0) break;
			if (iRet == 1)
			{
				int iRet = RecvPacketData(sock, packet);
				if (iRet < 0) break;
				if (iRet == 0) continue;
				// �޼��� ó��
				std::cout << "\n" << packet.msg;
			}
		}
	}
	std::cout << "��������" << std::endl;
	closesocket(sock);
	WSACleanup();
}

int SendMsg(SOCKET sock, char* msg, WORD type)
{
	// 1�� ��Ŷ ����
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	//�޼��� ���뿡�� ��� ������ �߰�
	packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
	packet.ph.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	// 2�� ��Ŷ ���� : �ü�� sendbuffer(short����Ʈ), recvbuffer
	char* pMsg = (char*)&packet;
	int iSendSize = 0;
	do {
		int iSendByte = send(sock, &pMsg[iSendSize],
			packet.ph.len - iSendSize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSendSize += iSendByte;
	} while (iSendSize < packet.ph.len);
	return iSendSize;
}
//��Ŷ ����
int SendPacket(SOCKET sock, char* msg, WORD type)
{

	// 1�� ��Ŷ ����
	KPacket tPacket(type);

	//index, name[20], damage, message[256]
	tPacket << 999 << "������" << (short)50 << msg;
	
	//�����ڷ� ��Ŷ �����ڷ� ��Ŷ�� �޾� ��Ŷ�� ����
	KPacket tPacketTest(tPacket);

	KChatting recvdata;
	ZeroMemory(&recvdata, sizeof(recvdata));
	
	//��Ŷ���� ���� �Ľ�
	tPacketTest >> recvdata.index >> recvdata.name
		>> recvdata.damage >> recvdata.message;

	char* pData = (char*)&tPacket.m_uPacket;

	int iSendSize = 0;
	do {
		int iSendByte = send(sock, &pData[iSendSize],
			tPacket.m_uPacket.ph.len - iSendSize, 0);
		if (iSendByte == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
		}
		iSendSize += iSendByte;
	} while (iSendSize < tPacket.m_uPacket.ph.len);
	return iSendSize;
}
//��Ŷ ����� �޾ƿ���
int RecvPacketHeader(SOCKET sock, UPACKET& recvPacket)
{
	char szRecvBuffer[256] = { 0, };
	//��Ŷ��� �ޱ�
	ZeroMemory(&recvPacket, sizeof(recvPacket));
	bool bRun = true;
	int iRecvSize = 0;
	//��Ŷ ��� (4����Ʈ)�� �ɶ����� do while�� 
	do {
		int iRecvByte = recv(sock, szRecvBuffer,
			PACKET_HEADER_SIZE, 0);
		iRecvSize += iRecvByte;
		if (iRecvByte == 0)
		{
			closesocket(sock);
			std::cout << " ���������." << std::endl;
			return -1;
		}
		if (iRecvByte == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				std::cout << " ������ ���������." << std::endl;
				return -1;
			}
			else
			{
				return 0;
			}
		}
	} while (iRecvSize < PACKET_HEADER_SIZE);
	memcpy(&recvPacket.ph, szRecvBuffer, PACKET_HEADER_SIZE);
	return 1;
}
//��Ŷ ��� ������ ��Ŷ �ޱ�
int RecvPacketData(SOCKET sock, UPACKET& recvPacket)
{
	// ������ �ޱ�
	int iRecvSize = 0;
	do {
		int iRecvByte = recv(sock, recvPacket.msg,
			recvPacket.ph.len - PACKET_HEADER_SIZE - iRecvSize, 0);
		iRecvSize += iRecvByte;
		if (iRecvByte == 0)
		{
			closesocket(sock);
			std::cout << " ���������." << std::endl;
			return -1;
		}
		if (iRecvByte == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				std::cout << " ������ ���������." << std::endl;
				return -1;
			}
			else
			{
				return 0;
			}
		}
	} while (iRecvSize < recvPacket.ph.len - PACKET_HEADER_SIZE);
	return 1;
}