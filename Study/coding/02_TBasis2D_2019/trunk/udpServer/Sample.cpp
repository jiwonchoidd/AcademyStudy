#define _WINSOCK_DEPRECATED_NO_WARNINGS
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
	//exit(-1);
}
void main()
{
	WSADATA wd;
	if (WSAStartup(MAKEWORD(2, 2), &wd)!=0)
	{
		return;
	}
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
	{
		Error("sock invalid");
	}
	SOCKADDR_IN serveradd;
	ZeroMemory(&serveradd, sizeof(serveradd));
	serveradd.sin_family = AF_INET;
	serveradd.sin_port = htons(9000);
	serveradd.sin_addr.s_addr = htonl(INADDR_ANY);	
	int iRet = bind(sock, (SOCKADDR*)&serveradd,sizeof(serveradd));
	if (iRet == SOCKET_ERROR)
	{
		Error("sock invalid");
	}
	SOCKADDR_IN clientAddr;
	char buf[256] = { 0, };
	int addlen;
	while (1)
	{
		addlen = sizeof(clientAddr);
		iRet = recvfrom(sock, buf, 256, 0, (SOCKADDR*)&clientAddr, &addlen);
		if (iRet == SOCKET_ERROR)
		{
			Error("sock invalid");
		}
		buf[iRet] = 0;
		printf("\n[%s:%d]:%s",inet_ntoa(clientAddr.sin_addr),ntohs(clientAddr.sin_port),buf);
		iRet = sendto(sock, buf, iRet, 0,(SOCKADDR*)&clientAddr,sizeof(clientAddr));
		if (iRet == SOCKET_ERROR)
		{
			Error("sock invalid");
		}
	}

	WSACleanup();
}