#include "Sample.h"


DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	TUser* pUser = &g_allClientSocket[sock];

	char buffer[256] = { 0, };
	int recvbyte = 0;
	int iRet = 0;
	while (1)
	{
		iRet = recv(sock, &buffer[recvbyte],
			PACKET_HEADER_SIZE - recvbyte, 0);
		if (CheckReturn(iRet) <= 0) break;

		recvbyte += iRet;
		if (recvbyte == PACKET_HEADER_SIZE)
		{
			UPACKET packet;
			ZeroMemory(&packet, sizeof(packet));
			memcpy(&packet.ph, buffer,
				PACKET_HEADER_SIZE);
			int rByte = 0;
			int iRecvByte = 0;
			do
			{
				rByte = recv(sock, (char*)&packet.msg[iRecvByte], packet.ph.len - iRecvByte, 0);
				if (CheckReturn(iRet) <= 0) break;
				iRecvByte += rByte;
			} while (packet.ph.len > iRecvByte);

			if (CheckReturn(iRet) <= 0) break;

			recvbyte = 0;
			switch (packet.ph.type)
			{
			case PACKET_CHAT_MSG:
			{
				std::cout << "패킷완성:--->" << packet.msg;
				Broadcastting(&packet);
			}break;
			}
		}
		Sleep(1);
	}
	DeleteUser(sock);
	return 0;
}

// 소켓 생성
// 연결형, TCP프로토콜(SOCK_STREAM,IPPROTO_TCP)
// 비연결형, UDP 프로토콜(SOCK_DGRAM,IPPROTO_UDP)
int main(int argc, char* argv[])
{
	HANDLE hExecuteMutex = CreateMutex(NULL, FALSE, L"OnceMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hExecuteMutex);
		return 1;
	}
	InitializeCriticalSection(&g_crit);
	// TRUE =  현재의 스레드가 소유자
	// FALSE = 소유자가 없다.(신호상태) : 비신호상태
	g_hMutex = CreateMutex(NULL, FALSE, L"CHAT_MUTEX");

	unsigned short iPort = 10000;// atoi(argv[1]);
								 // 윈도우 소켓(윈속) 초기화
	WSADATA wsa;
	int iRet;
	HANDLE hThread=NULL;
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
				if (client == INVALID_SOCKET) break;

				TUser user(client, clientaddr);
				//EnterCriticalSection(&g_crit);
				// 신호상태 일 경우만 진입
				WaitForSingleObject(g_hMutex, INFINITE);// 대기함수
				GreetMessage(user);
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
	CloseHandle(hThread);
	CloseHandle(g_hMutex);
	CloseHandle(hExecuteMutex);
	return 0;
}