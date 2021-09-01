// ExclusiveAddruse.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
//http://mirine35.egloos.com/m/5057698   옵션문서
#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#pragma comment(lib, "ws2_32.lib")

int _tmain(int argc, _TCHAR * argv[])
{
	SOCKET sdf1, sdf2;
	struct sockaddr_in addrin;
	int rc;

	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	sdf1 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	int opt​​val = 1; 
	setsockopt (sdf1, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
				(char *)&opt​​val, sizeof(opt​​val));

	addrin.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrin.sin_port = htons(55001);
	addrin.sin_family = AF_INET;
	rc = ::bind(sdf1, (SOCKADDR*)&addrin, sizeof(SOCKADDR));
	DWORD dwError = WSAGetLastError();
	switch (dwError)
	{
		case WSAEADDRINUSE: // 이미 다른 프로세스에서 사용중이다.
		{
			printf("port no 55001 binded \ n");
			break;
		}
		case WSAEACCES: // 사용 권한이 거부 되었다.
		{
			printf("port no 55001 binded \ n");
			break;
		}
	}

	sdf2 = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	addrin.sin_addr.s_addr = htonl(INADDR_ANY);
	//addrin.sin_addr.s_addr = inet_addr ( "127.0.0.1"); 
	addrin.sin_port = htons(55001);
	addrin.sin_family = AF_INET;
	rc = bind(sdf2, (SOCKADDR*)&addrin, sizeof(SOCKADDR));
	dwError = WSAGetLastError();
	switch (dwError)
	{
		case WSAEADDRINUSE:
		{
			printf("port no 55001 binded \ n");
		}break;
		case WSAEACCES:
		{
			printf("port no 55001 binded \ n");
			break;
		}
	}
	
	printf("port no 55001 binded \ n");
	printf("netstat -ao execute \ n");	
	system("netstat -ao");


	WSACleanup();
	

	return 0;
}


