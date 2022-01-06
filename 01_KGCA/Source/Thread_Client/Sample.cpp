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
//SendThread가 리턴이 되면 쓰레드가 종료가 된다.
//클라이언트
void main()
{
	/*WSACleanup 함수와 쌍을 이뤄 소켓 프로그램의 시작과 끝을 나타냄*/
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
	std::cout << "접속성공!" << std::endl;

	//논블로킹 소켓 0이 아니라면 논블로킹 모드로 설정
	u_long on = 1;
	ioctlsocket(sock, FIONBIO, &on);

	//쓰레드 생성에는 두가지 방법이 있음
	// Window API 방법, C++ 11 방법, :: 범위해결자
	//c++
	DWORD ThreadIndex;
	HANDLE hThread_Send = ::CreateThread(0, 0, SendThread,
		(LPVOID)sock, 0, &ThreadIndex);

	HANDLE hThread_Recv = ::CreateThread(0, 0, RecvThread,
		(LPVOID)sock, 0, &ThreadIndex);

	//메인 쓰레드 작업
	while (1)
	{
		Sleep(100);
	}
	std::cout << "접속종료" << std::endl;

	CloseHandle(hThread_Send); //쓰레드 종료가 아닌 제어를 운영체제에게 넘겨주는 것
	CloseHandle(hThread_Recv); //쓰레드 종료가 아닌 제어를 운영체제에게 넘겨주는 것
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
			// 메세지 처리
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
			std::cout << "정상 종료됨.." << std::endl;
			break;
		}

		// 방법 1
		//int iSendByte = SendMsg(sock, szBuffer, PACKET_CHAT_MSG);
		// 방법 2
		int iSendByte = SendPacket(sock, szBuffer, PACKET_CHAT_MSG);
		if (iSendByte < 0)
		{
			std::cout << "비정상 종료됨.." << std::endl;
			break;
		}
	}
	return 0;
}



int SendMsg(SOCKET sock, char* msg, WORD type)
{
	// 1번 패킷 생성
	UPACKET packet;
	ZeroMemory(&packet, sizeof(packet));
	//메세지 내용에서 헤더 사이즈 추가
	packet.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
	packet.ph.type = type;
	memcpy(packet.msg, msg, strlen(msg));
	// 2번 패킷 전송 : 운영체제 sendbuffer(short바이트), recvbuffer
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
//패킷 전송
int SendPacket(SOCKET sock, char* msg, WORD type)
{

	// 1번 패킷 생성
	KPacket tPacket(type);

	//index, name[20], damage, message[256]
	tPacket << 999 << "최지원" << (short)50 << msg;
	
	//생성자로 패킷 생성자로 패킷을 받아 패킷을 만듬
	KPacket tPacketTest(tPacket);

	KChatting recvdata;
	ZeroMemory(&recvdata, sizeof(recvdata));
	
	//패킷에서 정보 파싱
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
//패킷 헤더만 받아오기
int RecvPacketHeader(SOCKET sock, UPACKET& recvPacket)
{
	char szRecvBuffer[256] = { 0, };
	//패킷헤더 받기
	ZeroMemory(&recvPacket, sizeof(recvPacket));
	bool bRun = true;
	int iRecvSize = 0;
	//페킷 헤더 (4바이트)가 될때까지 do while문 
	do {
		int iRecvByte = recv(sock, szRecvBuffer,
			PACKET_HEADER_SIZE, 0);
		iRecvSize += iRecvByte;
		if (iRecvByte == 0)
		{
			closesocket(sock);
			std::cout << " 접속종료됨." << std::endl;
			return -1;
		}
		if (iRecvByte == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				std::cout << " 비정상 접속종료됨." << std::endl;
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
//패킷 헤더 제외한 패킷 받기
int RecvPacketData(SOCKET sock, UPACKET& recvPacket)
{
	// 데이터 받기
	int iRecvSize = 0;
	do {
		int iRecvByte = recv(sock, recvPacket.msg,
			recvPacket.ph.len - PACKET_HEADER_SIZE - iRecvSize, 0);
		iRecvSize += iRecvByte;
		if (iRecvByte == 0)
		{
			closesocket(sock);
			std::cout << " 접속종료됨." << std::endl;
			return -1;
		}
		if (iRecvByte == SOCKET_ERROR)
		{
			int iError = WSAGetLastError();
			if (iError != WSAEWOULDBLOCK)
			{
				std::cout << " 비정상 접속종료됨." << std::endl;
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


