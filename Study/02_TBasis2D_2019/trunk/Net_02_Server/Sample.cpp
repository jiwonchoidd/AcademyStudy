#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

CRITICAL_SECTION	g_crit;  // 유저모드
HANDLE			    g_hMutex;// 커널 모드
SOCKET	g_allClientSocket[100];
int		g_iNumClient = 0;
void GreetMessage(SOCKET sock)
{
	char message[MAX_PATH] = { 0, };
	sprintf_s(message, "[서버]환영합니다.\n");
	send(sock, message, strlen(message), 0);

	g_allClientSocket[g_iNumClient] = sock;
	g_iNumClient++;
	// temp = sock
}
DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char buf[256] = { 0, };
	//printf("\n 보낼 데이터 입력하시오?");
	SOCKADDR_IN  clientaddr;
	while (1)
	{
		ZeroMemory(&buf, sizeof(char) * 256);
		int iRecvByte = recv(sock, buf, 256, 0);
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

		// 브로드 케스팅
		//EnterCriticalSection(&g_crit);
		DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);// 대기함수
		if (dwRet != WAIT_TIMEOUT)
		{
			for (int iClient = 0; iClient < g_iNumClient; iClient++)
			{
				int iSendByte = send(g_allClientSocket[iClient], buf, iRecvByte, 0);
				if (iSendByte == 0 || iSendByte == SOCKET_ERROR)
				{
					printf("클라이언트 접속 종료\n");
					break; // 클라이언트 종료
				}
			}
		}
		//LeaveCriticalSection(&g_crit);
		ReleaseMutex(g_hMutex);
	}
	// 클라이언트 종료 처리
	//EnterCriticalSection(&g_crit);
	WaitForSingleObject(g_hMutex, INFINITE);// 대기함수

	for (int iClient = 0; iClient < g_iNumClient; iClient++)
	{
		if (g_allClientSocket[iClient] == sock)
		{
			for (int iUser = iClient; iUser < g_iNumClient; iUser++)
			{
				g_allClientSocket[iUser] = g_allClientSocket[iUser+1];
			}
			break;
		}
	}
	g_iNumClient--;
	//LeaveCriticalSection(&g_crit);
	ReleaseMutex(g_hMutex);
	closesocket(sock);
	return 0;
}

// 소켓 생성
// 연결형, TCP프로토콜(SOCK_STREAM,IPPROTO_TCP)
// 비연결형, UDP 프로토콜(SOCK_DGRAM,IPPROTO_UDP)
int main(int argc, char* argv[])
{
	HANDLE hExecuteMutex = CreateMutex(NULL, FALSE,L"OnceMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hExecuteMutex);
		return 1;
	}
	

	InitializeCriticalSection(&g_crit);
	// TRUE =  현재의 스레드가 소유자
	// FALSE = 소유자가 없다.(신호상태) : 비신호상태
	g_hMutex = CreateMutex(NULL, FALSE, L"CHAT_MUTEX");

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
			sa.sin_port = htons(10000);
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
				
				//EnterCriticalSection(&g_crit);
				// 신호상태 일 경우만 진입
				WaitForSingleObject(g_hMutex, INFINITE );// 대기함수
					GreetMessage(client);
				//LeaveCriticalSection(&g_crit);
				ReleaseMutex(g_hMutex);// 비신호상태로 전환

				DWORD dwRecvThreadID;
				hThread = CreateThread(0, 0,
					ClientThread,
					(LPVOID)client,
					0,
					&dwRecvThreadID);
			}
		}
		closesocket(listenSock);
	}
	// 윈도우 소켓 소멸
	iRet = WSACleanup();

	DeleteCriticalSection(&g_crit);
	CloseHandle(g_hMutex);
	CloseHandle(hExecuteMutex);
	return 0;
}