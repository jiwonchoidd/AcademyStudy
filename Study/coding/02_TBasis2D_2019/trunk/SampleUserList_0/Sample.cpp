#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include "TUser.h"

#pragma comment(lib, "ws2_32.lib")

CRITICAL_SECTION	  g_crit;  // 유저모드
std::list<TUser*>     g_allClientList;
TPacketPool			  g_PacketList;
int		g_iNumClient = 0;

int SendMsg(TUser* pUser, char* msg, WORD type)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph.type = type;// PACKET_CHAT_MSG;
						   // 방법 1
	memcpy(sendmsg.msg, msg, strlen(msg));
	// 방법 2
	//sprintf_s(sendmsg.msg,"%s", "[서버]환영합니다.");
	sendmsg.ph.len = strlen(sendmsg.msg) + PACKET_HEADER_SIZE;
	int iRet = send(pUser->m_Socket, (char*)&sendmsg, sendmsg.ph.len, 0);
	return iRet;
}
DWORD WINAPI WorkThread(LPVOID arg)
{
	while (1)
	{
		// 브로드 케스팅
		EnterCriticalSection(&g_crit);
		{
			std::list<UPACKET>::iterator  iterPacket;
			for (iterPacket = I_Pool.m_RecvPacketList.begin();
			iterPacket != I_Pool.m_RecvPacketList.end();
				iterPacket++)
			{
				UPACKET* pPacket = &(UPACKET)*iterPacket;
				printf("\n[받은데이터]:%s", pPacket->msg);

				std::list<TUser*>::iterator  iter;
				for (iter = g_allClientList.begin();
				iter != g_allClientList.end();
					iter++)
				{
					TUser* pSendUser = (TUser*)*iter;
					//int iSendByte = SendMsg(pSendUser, buf, PACKET_CHAT_MSG);
					int iSendByte = send(pSendUser->m_Socket, (char*)pPacket,
						pPacket->ph.len, 0);
					
					if (iSendByte == SOCKET_ERROR)
					{
						printf("소켓 오류\n");
						break; // 클라이언트 종료
					}
				}
			}
		}
		LeaveCriticalSection(&g_crit);
		I_Pool.m_RecvPacketList.clear();
		Sleep(1);
	}
	return 0;
}
DWORD WINAPI ClientThread(LPVOID arg)
{
	TUser* pUser = (TUser*)arg;
	char buf[1024] = { 0, };
	//printf("\n 보낼 데이터 입력하시오?");
	//ZeroMemory(&buf, sizeof(char) * 1024);
	SOCKADDR_IN  clientaddr;
	while (1)
	{		
		int iRecvByte = recv(pUser->m_Socket, buf, 1024, 0);
		if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
		{
			printf("클라이언트 접속 종료\n");
			printf("클라이언트 접속 종료 : IP:%s, PORT:%d\n",
				pUser->m_szIP.c_str(), pUser->m_dwPort);
			break; // 클라이언트 종료
		}
		pUser->m_RecvStreamPacket.Put(buf, iRecvByte, pUser);
		Sleep(1);
	}
	// 클라이언트 종료 처리
	EnterCriticalSection(&g_crit);
	std::list<TUser*>::iterator  iter;
	for (iter = g_allClientList.begin();
	iter != g_allClientList.end();
		iter++)
	{
		TUser* pDelUser = (TUser*)*iter;
		if (pDelUser->m_Socket == pUser->m_Socket)
		{
			closesocket(pDelUser->m_Socket);
			delete pDelUser;
			g_allClientList.erase(iter);
			break;
		}
	}
	g_iNumClient = g_allClientList.size();
	LeaveCriticalSection(&g_crit);
	return 0;
}


void GreetMessage(SOCKET sock, SOCKADDR_IN  clientaddr)
{	
	TUser* pUser = new TUser;
	pUser->m_Socket = sock;
	pUser->m_UserAddress = clientaddr;
	pUser->m_szIP = inet_ntoa(clientaddr.sin_addr);
	pUser->m_dwPort = ntohs(clientaddr.sin_port);
	g_allClientList.push_back(pUser);

	DWORD dwRecvThreadID;
	pUser->m_hThread = CreateThread(0, 0,
		ClientThread,
		(LPVOID)pUser,
		0,
		&pUser->m_dwRecvThreadID);

	printf("클라이언트 접속 : IP:%s, PORT:%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	char message[MAX_PATH] = { 0, };
	sprintf_s(message, "[서버]환영합니다.\n");
	SendMsg(pUser, message, PACKET_CHAT_MSG);
	g_iNumClient = g_allClientList.size();	
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

	unsigned short iPort = 10000;// atoi(argv[1]);
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

			// 패킷 풀 작업 스레드 생성
			DWORD dwRecvThreadID;
			hThread = CreateThread(0, 0,WorkThread,(LPVOID)0,0,&dwRecvThreadID);

			while (1)
			{
				int addlen = sizeof(clientaddr);
				client = accept(listenSock, (SOCKADDR*)&clientaddr, &addlen);
				// 신호상태 일 경우만 진입
				EnterCriticalSection(&g_crit);
				GreetMessage(client, clientaddr);
				LeaveCriticalSection(&g_crit);
			}
		}
		closesocket(listenSock);
	}
	// 윈도우 소켓 소멸
	iRet = WSACleanup();
	DeleteCriticalSection(&g_crit);
	CloseHandle(hExecuteMutex);
	g_allClientList.clear();
	return 0;
}