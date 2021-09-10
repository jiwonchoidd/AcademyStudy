#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <iostream>
#include <map>
#include "TProtocol.h"
#include "TPacket.h"
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
	TUser(SOCKET client, SOCKADDR_IN  addr)
	{
		sock = client;
		clientaddr = addr;
	}
};

std::map<SOCKET, TUser> g_allClientSocket;
typedef map<SOCKET, TUser>::iterator ITOR;

void T_ERROR(bool bPrint);
int CheckReturn(int iRet);
int SendMsg(SOCKET sock, char* msg, WORD type);
int SendMsg(SOCKET sock, UPACKET* packet, WORD type);
int PacketSend(SOCKET sock, char* msg, WORD type);
int Broadcastting(UPACKET* pPacket);
void DeleteUser(SOCKET sock);
void GreetMessage(TUser& user);

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
		iSend = send(sock, &pMsg[sendbytes], iTotalsize - sendbytes, 0);
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
		iSend = send(sock, &pMsg[sendbytes], iTotalsize - sendbytes, 0);
		if (CheckReturn(iSend) <= 0)
		{
			return iSend;
		}
		sendbytes += iSend;
	} while (sendbytes < iTotalsize);
	return sendbytes;
}
int PacketSend(SOCKET sock, char* msg, WORD type)
{
	TPacket packet(type);
	packet << msg;

	int sendbytes = 0;
	int iTotalsize = packet.m_uPacket.ph.len + PACKET_HEADER_SIZE;

	char* pMsg = (char*)&packet.m_uPacket;
	int iSend = 0;
	do {
		iSend = send(sock, &pMsg[sendbytes], iTotalsize - sendbytes, 0);
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
			if (pUser != nullptr)
			{
				int iSendByte = PacketSend(pUser->sock, (char*)pPacket, PACKET_CHAT_MSG);
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
	PacketSend(user.sock, message, PACKET_CHAT_MSG);

	DWORD dwRet = WaitForSingleObject(g_hMutex, INFINITE);// 대기함수
	g_allClientSocket.insert(make_pair(user.sock, user));
	ReleaseMutex(g_hMutex);
}
