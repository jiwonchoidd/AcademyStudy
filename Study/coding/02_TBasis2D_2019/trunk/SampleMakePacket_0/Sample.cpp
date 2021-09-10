#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <map>
#include "TProtocol.h"
#pragma comment(lib, "ws2_32.lib")
using namespace std;

CRITICAL_SECTION	g_crit;  // 유저모드
HANDLE			    g_hMutex;// 커널 모드

struct TUser
{
	SOCKET		sock;
	SOCKADDR_IN clientaddr;
	TUser() {}
	TUser(const TUser& copy) {
		sock = copy.sock;
		clientaddr = copy.clientaddr;
	}
	TUser(SOCKET client,SOCKADDR_IN  addr)
	{
		sock = client;
		clientaddr = addr;
	}
};

std::map<SOCKET, TUser> g_allClientSocket;
typedef map<SOCKET, TUser>::iterator ITOR;

void T_ERROR(bool bPrint)
{
	if (WSAGetLastError() != WSA_IO_PENDING)
	{
		char* lpMsgBuf;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM, NULL,
			WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char*)&lpMsgBuf, 0, NULL);
		if (bPrint)
		{
			MessageBoxA(NULL, lpMsgBuf, "ERROR", MB_OK);
		}
		else
		{
			OutputDebugStringA((char*)lpMsgBuf);
			printf("\n%s", (char*)lpMsgBuf);
		}
		LocalFree(lpMsgBuf);
	}
	
}

int CheckReturn(int iRet)
{
	if (iRet == 0)
	{
		printf("\n---->정상퇴장");
		return 0;
	}
	if (iRet <= SOCKET_ERROR)
	{
		printf("\n---->비정상 퇴장");
		T_ERROR(false);
		return -1;
	}
	return iRet;
}

int SendMsg(SOCKET sock, char* msg, WORD type)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph.len = strlen(msg);
	sendmsg.ph.type = type;
	memcpy(sendmsg.msg, msg, strlen(msg));
	
	int sendbytes = 0;
	int iTotalsize = strlen(msg) + PACKET_HEADER_SIZE;
	char* pMsg = (char*)&sendmsg;
	int iSend = 0;
	do {
		iSend = send(sock,&pMsg[sendbytes],iTotalsize - sendbytes, 0);
		if (CheckReturn(iSend) <= 0)
		{
			return iSend;
		}
		sendbytes += iSend;
	} while (sendbytes < iTotalsize);
	return sendbytes;
}
int SendMsg(SOCKET sock, UPACKET* packet, WORD type)
{
	int sendbytes = 0;
	int iTotalsize = packet->ph.len + PACKET_HEADER_SIZE;
	packet->ph.type = type;

	char* pMsg = (char*)packet;
	int iSend = 0;
	do {
		iSend = send(sock,&pMsg[sendbytes],iTotalsize - sendbytes, 0);
		if (CheckReturn(iSend) <= 0)
		{
			return iSend;
		}
		sendbytes += iSend;
	} while (sendbytes < iTotalsize);
	return sendbytes;
}
int Broadcastting(UPACKET* pPacket)
{
	//EnterCriticalSection(&g_crit);
	int iUserCounter = 0;
	DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);// 대기함수
	if (dwRet != WAIT_TIMEOUT)
	{		
		for (ITOR itor = g_allClientSocket.begin(); itor != g_allClientSocket.end();)
		{
			TUser* pUser = (TUser*)&itor->second;
			if (pUser!=nullptr)
			{
				int iSendByte = SendMsg(pUser->sock, (UPACKET*)pPacket, PACKET_CHAT_MSG);
				if (CheckReturn(iSendByte) <= 0)
				{
					printf("\n퇴장(Send) : IP:%s, PORT:%d\n",
							inet_ntoa(pUser->clientaddr.sin_addr), ntohs(pUser->clientaddr.sin_port));
					g_allClientSocket.erase(itor++);
				}
				else
				{
					iUserCounter++;
					++itor;
				}
			}
		}	
	}
	//LeaveCriticalSection(&g_crit);
	ReleaseMutex(g_hMutex);
	return -1;
}
void DeleteUser(SOCKET sock)
{
	//EnterCriticalSection(&g_crit);	
	DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);// 대기함수
		ITOR itor = g_allClientSocket.find(sock);
		if (itor != g_allClientSocket.end())
		{
			TUser* pUser = (TUser*)&itor->second;
			if (pUser != nullptr)
			{
				printf("\n퇴장(sock) : IP:%s, PORT:%d\n",
					inet_ntoa(pUser->clientaddr.sin_addr), ntohs(pUser->clientaddr.sin_port));			
			}
			g_allClientSocket.erase(itor);			
		}
		closesocket(sock);	
	//LeaveCriticalSection(&g_crit);
	ReleaseMutex(g_hMutex);
}
void GreetMessage(TUser& user)
{
	printf("\n클라이언트 접속 : IP:%s, PORT:%d\n",
		inet_ntoa(user.clientaddr.sin_addr), ntohs(user.clientaddr.sin_port));

	char message[MAX_PATH] = { 0, };
	sprintf_s(message, "[서버]환영합니다");	
	SendMsg(user.sock, message, PACKET_CHAT_MSG);	
	DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);// 대기함수
		g_allClientSocket.insert(make_pair(user.sock, user));
	ReleaseMutex(g_hMutex);	
}

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
			memcpy(&packet.ph, buffer,PACKET_HEADER_SIZE);
			int rByte = 0;
			int iRecvByte = 0;
			do
			{
				rByte = recv(sock,(char*)&packet.msg[iRecvByte],packet.ph.len - iRecvByte, 0);
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
				HANDLE hThread = NULL;
				hThread = CreateThread(0, 0,
					ClientThread,
					(LPVOID)client,
					0,
					&dwRecvThreadID);
				CloseHandle(hThread);
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