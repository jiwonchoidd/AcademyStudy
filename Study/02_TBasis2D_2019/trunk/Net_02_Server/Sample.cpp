#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

CRITICAL_SECTION	g_crit;  // �������
HANDLE			    g_hMutex;// Ŀ�� ���
SOCKET	g_allClientSocket[100];
int		g_iNumClient = 0;
void GreetMessage(SOCKET sock)
{
	char message[MAX_PATH] = { 0, };
	sprintf_s(message, "[����]ȯ���մϴ�.\n");
	send(sock, message, strlen(message), 0);

	g_allClientSocket[g_iNumClient] = sock;
	g_iNumClient++;
	// temp = sock
}
DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char buf[256] = { 0, };
	//printf("\n ���� ������ �Է��Ͻÿ�?");
	SOCKADDR_IN  clientaddr;
	while (1)
	{
		ZeroMemory(&buf, sizeof(char) * 256);
		int iRecvByte = recv(sock, buf, 256, 0);
		if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
		{
			printf("Ŭ���̾�Ʈ ���� ����\n");
			//printf("Ŭ���̾�Ʈ ���� ���� : IP:%s, PORT:%d\n",
			//	inet_ntoa(clientaddr.sin_addr),
			//	ntohs(clientaddr.sin_port));
			break; // Ŭ���̾�Ʈ ����
		}
		buf[iRecvByte] = 0;
		printf("\n%s", buf);

		// ��ε� �ɽ���
		//EnterCriticalSection(&g_crit);
		DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);// ����Լ�
		if (dwRet != WAIT_TIMEOUT)
		{
			for (int iClient = 0; iClient < g_iNumClient; iClient++)
			{
				int iSendByte = send(g_allClientSocket[iClient], buf, iRecvByte, 0);
				if (iSendByte == 0 || iSendByte == SOCKET_ERROR)
				{
					printf("Ŭ���̾�Ʈ ���� ����\n");
					break; // Ŭ���̾�Ʈ ����
				}
			}
		}
		//LeaveCriticalSection(&g_crit);
		ReleaseMutex(g_hMutex);
	}
	// Ŭ���̾�Ʈ ���� ó��
	//EnterCriticalSection(&g_crit);
	WaitForSingleObject(g_hMutex, INFINITE);// ����Լ�

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

// ���� ����
// ������, TCP��������(SOCK_STREAM,IPPROTO_TCP)
// �񿬰���, UDP ��������(SOCK_DGRAM,IPPROTO_UDP)
int main(int argc, char* argv[])
{
	HANDLE hExecuteMutex = CreateMutex(NULL, FALSE,L"OnceMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hExecuteMutex);
		return 1;
	}
	

	InitializeCriticalSection(&g_crit);
	// TRUE =  ������ �����尡 ������
	// FALSE = �����ڰ� ����.(��ȣ����) : ���ȣ����
	g_hMutex = CreateMutex(NULL, FALSE, L"CHAT_MUTEX");

	// ������ ����(����) �ʱ�ȭ
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
			// ������ ��� ip�� �����ض�.
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
				printf("Ŭ���̾�Ʈ ���� : IP:%s, PORT:%d\n",
					inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
				
				//EnterCriticalSection(&g_crit);
				// ��ȣ���� �� ��츸 ����
				WaitForSingleObject(g_hMutex, INFINITE );// ����Լ�
					GreetMessage(client);
				//LeaveCriticalSection(&g_crit);
				ReleaseMutex(g_hMutex);// ���ȣ���·� ��ȯ

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
	// ������ ���� �Ҹ�
	iRet = WSACleanup();

	DeleteCriticalSection(&g_crit);
	CloseHandle(g_hMutex);
	CloseHandle(hExecuteMutex);
	return 0;
}