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

DWORD WINAPI SendThread(LPVOID IpThreadParameter);
DWORD WINAPI RecvThread(LPVOID IpThreadParameter);
//SendThread�� ������ �Ǹ� �����尡 ���ᰡ �ȴ�.
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

	//������ŷ ���� 0�� �ƴ϶�� ������ŷ ���� ����
	u_long on = 1;
	ioctlsocket(sock, FIONBIO, &on);

	//������ �������� �ΰ��� ����� ����
	// Window API ���, C++ 11 ���, :: �����ذ���
	//c++
	DWORD ThreadIndex;
	HANDLE hThread_Send = ::CreateThread(0, 0, SendThread,
		(LPVOID)sock, 0, &ThreadIndex);

	HANDLE hThread_Recv = ::CreateThread(0, 0, RecvThread,
		(LPVOID)sock, 0, &ThreadIndex);

	//���� ������ �۾�
	while (1)
	{
		Sleep(100);
	}
	std::cout << "��������" << std::endl;

	CloseHandle(hThread_Send); //������ ���ᰡ �ƴ� ��� �ü������ �Ѱ��ִ� ��
	CloseHandle(hThread_Recv); //������ ���ᰡ �ƴ� ��� �ü������ �Ѱ��ִ� ��
	closesocket(sock);
	WSACleanup();
}

DWORD __stdcall RecvThread(LPVOID IpThreadParameter)
{
	SOCKET sock = (SOCKET)IpThreadParameter;
	while (1)
	{
		UPACKET packet;
		int iRet = RecvPacketHeader(sock, packet);
		if (iRet < 0) break;
		if (iRet == 1)
		{
			int iRet = RecvPacketData(sock, packet);
			if (iRet < 0) break;
			if (iRet == 0) continue;
			// �޼��� ó��
			KPacket data;
			data.m_uPacket = packet;
			KChatting recvdata;
			ZeroMemory(&recvdata, sizeof(recvdata));
			data >> recvdata.index >> recvdata.name
				>> recvdata.damage >> recvdata.message;
			std::cout << "\n" <<
				"[" << recvdata.name << "]"
				<< recvdata.message;
		}
	}
	return 0;
}

DWORD __stdcall SendThread(LPVOID IpThreadParameter)
{
	SOCKET sock = (SOCKET)IpThreadParameter;
	char szBuffer[256] = { 0, };
	while (1)
	{
		ZeroMemory(szBuffer, sizeof(char) * 256);
		fgets(szBuffer, 256, stdin);
		if (strlen(szBuffer) == 0)
		{
			std::cout << "���� �����.." << std::endl;
			break;
		}

		// ��� 1
		//int iSendByte = SendMsg(sock, szBuffer, PACKET_CHAT_MSG);
		// ��� 2
		int iSendByte = SendPacket(sock, szBuffer, PACKET_CHAT_MSG);
		if (iSendByte < 0)
		{
			std::cout << "������ �����.." << std::endl;
			break;
		}
	}
	return 0;
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

