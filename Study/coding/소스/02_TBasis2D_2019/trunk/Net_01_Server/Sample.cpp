#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char buf[256] = { 0, };
	//printf("\n 보낼 데이터 입력하시오?");
	SOCKADDR_IN  clientaddr;
	while (1)
	{		
		ZeroMemory(&buf, sizeof(char) * 256);
		int iRecvByte = recv(sock, buf, 255, 0);
		if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
		{
			printf("클라이언트 접속 종료\n");
			//printf("클라이언트 접속 종료 : IP:%s, PORT:%d\n",
			//	inet_ntoa(clientaddr.sin_addr),
			//	ntohs(clientaddr.sin_port));
			break; // 클라이언트 종료
		}
		buf[iRecvByte] = 0;
		printf("\n%s", buf);
		int iSendByte = send(sock, buf, iRecvByte, 0);
		if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
		{
			printf("클라이언트 접속 종료\n");
			break; // 클라이언트 종료
		}
	}
	closesocket(sock);
	return 0;
}

// 소켓 생성
// 연결형, TCP프로토콜(SOCK_STREAM,IPPROTO_TCP)
// 비연결형, UDP 프로토콜(SOCK_DGRAM,IPPROTO_UDP)
int main(int argc, char* argv[])
{
	// 속성->구성요소->디버깅->명령인수=10000
	if (argc != 2)
	{
		printf("\n[사용방법] xxx.exe [port]");
		printf("\n[사용예시] xxx.exe 10000");
		return 1;
	}
	//unsigned short iPort = 10000;// atoi(argv[1]);
	unsigned short iPort = atoi(argv[1]);

	// 윈도우 소켓(윈속) 초기화
	WSADATA wsa;
	int iRet;
	HANDLE hThread;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 1; }
	{
		SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
		{
			SOCKADDR_IN sa;
			ZeroMemory(&sa, sizeof(sa));
			sa.sin_family = AF_INET;
			sa.sin_port = htons(iPort);
			sa.sin_addr.s_addr = htonl(INADDR_ANY);
			// 소켓을 대상 ip에 연결해라.
			iRet = bind(listenSock, (SOCKADDR*)&sa, sizeof(sa));
			if (iRet == SOCKET_ERROR) return -1;
			iRet = listen(listenSock, SOMAXCONN);
			if (iRet == SOCKET_ERROR) return -1;
			SOCKADDR_IN  clientaddr;
			SOCKET client;
			while (1)
			{
				int addlen = sizeof(clientaddr);
				client = accept(listenSock, (SOCKADDR*)&clientaddr, &addlen);
				printf("클라이언트 접속 : IP:%s, PORT:%d\n",
					inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
				
				DWORD dwUserThreadID;
				hThread = CreateThread(0, 0,					
					ClientThread,
					(LPVOID)client,
					0,
					&dwUserThreadID);
			}
		}
		closesocket(listenSock);
	}
	// 윈도우 소켓 소멸
	iRet = WSACleanup();
	return iRet;
}