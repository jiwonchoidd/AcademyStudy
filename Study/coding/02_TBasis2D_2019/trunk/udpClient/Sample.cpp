#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

void Error(char* msg)
{
	LPVOID lpMsgBuffer;
	int iError = WSAGetLastError();
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		iError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuffer,
		0, NULL);
	printf("%s", lpMsgBuffer);
	LocalFree(lpMsgBuffer);
	exit(-1);
}
void main()
{
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		return;
	}
	SOCKET sock = socket(AF_INET,
		SOCK_DGRAM,
		0);
	if (sock == INVALID_SOCKET)
	{
		Error("sock invalid");
	}

	SOCKADDR_IN serveradd;
	ZeroMemory(&serveradd, sizeof(serveradd));
	serveradd.sin_family = AF_INET;
	serveradd.sin_port = htons(9000);
	serveradd.sin_addr.s_addr =
		inet_addr("127.0.0.1");

	//bind(sock, (SOCKADDR*)&serveradd, sizeof(serveradd));

	SOCKADDR_IN clientAddr;
	char buf[256] = { 0, };
	char recvbuf[256]={ 0, };
	int addlen;
	int iCount = 0;
	while (1)
	{
		sprintf(buf, "%s:%d", "kgca", iCount++);
		int iRet = sendto(sock, buf, strlen(buf),
			0,
			(SOCKADDR*)&serveradd,
			sizeof(serveradd));
		if (iRet == SOCKET_ERROR)
		{
			Error("sock invalid");
		}
		addlen = sizeof(clientAddr);
		iRet = recvfrom(sock, recvbuf, 256, 0,
			(SOCKADDR*)&clientAddr,
			&addlen);
		if (iRet == SOCKET_ERROR)
		{
			Error("sock invalid");
		}
		recvbuf[iRet] = 0;

		printf("\n[%s:%d]:%s%s",
			inet_ntoa(clientAddr.sin_addr),
			ntohs(clientAddr.sin_port),
			recvbuf, recvbuf);
		
		//Sleep(1000);
	}

	WSACleanup();
}