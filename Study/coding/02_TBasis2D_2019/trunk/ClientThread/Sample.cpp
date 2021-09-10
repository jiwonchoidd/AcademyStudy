#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")

unsigned WINAPI KeyInputProc(LPVOID arg) {
	SOCKET sock = (SOCKET)arg;
	while (1) 
	{
		char buf[256] = { 0, };
		fgets(buf, 256, stdin);
		if (buf[0] == '\n')break;
		int iSize = strlen(buf);
		int iSendByte = send(sock, buf, iSize, 0);
		if (iSendByte == SOCKET_ERROR || iSendByte == 0)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK) 
			{
				printf("\n서버 실행종료");
				break;
			}
		}
	}
	closesocket(sock);
	return 0;
}
unsigned WINAPI RecvProc(LPVOID arg) 
{
	SOCKET sock = (SOCKET)arg;
	while (1) {
		char recvbuf[256] = { 0, };
		ZeroMemory(recvbuf, sizeof(char) * 256);
		int iRecvByte = recv(sock, recvbuf, sizeof(char) * 255, 0);
		if (iRecvByte == SOCKET_ERROR || iRecvByte == 0)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				printf("\n서버 실행종료");
				break;
			}
		}
		else
		{
			printf("\n%s", recvbuf);
		}
	}
	closesocket(sock);
	return 0;
}
void main()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_addr.S_un.S_addr =inet_addr("192.168.0.101");
	sa.sin_port = htons(10000);
	int iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
	{
		return;
	}

	u_long on = 1;
	ioctlsocket(sock, FIONBIO, &on);

	unsigned int id;
	unsigned long hKeyInput = _beginthreadex(NULL, 0, KeyInputProc, (LPVOID)sock, 0, &id);
	unsigned long hRecvProc = _beginthreadex(NULL, 0, RecvProc, (LPVOID)sock, 0, &id);
	// 대기함수
	WaitForSingleObject((HANDLE)hRecvProc, INFINITE);
	WaitForSingleObject((HANDLE)hKeyInput, INFINITE);
	CloseHandle((HANDLE)hKeyInput);
	CloseHandle((HANDLE)hRecvProc);
	closesocket(sock);
	WSACleanup();
}