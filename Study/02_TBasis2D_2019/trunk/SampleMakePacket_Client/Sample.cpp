#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include "TProtocol.h"
#pragma comment(lib, "ws2_32.lib")

HANDLE g_hEvent;
SOCKET sock;
bool	g_bConnect = false;

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
		iSend = send(sock,
			&pMsg[sendbytes],
			iTotalsize - sendbytes, 0);
		if (iSend <= 0)
		{
			printf("소켓 오류\n");
			return iSend;
		}
		sendbytes += iSend;
	} while (sendbytes < iTotalsize);
	return iTotalsize;
}

DWORD WINAPI ConnectThread(LPVOID arg)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;//sockaddr name;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);// host byte 정렬 to network byte 정렬 short
	sa.sin_addr.s_addr = inet_addr("192.168.0.111");
	int iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR) { return 1; }
	linger optval;
	int iLength = sizeof(optval);
	getsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&optval, &iLength);
	optval.l_onoff = 1;
	optval.l_linger = 1000;
	setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&optval, iLength);

	SetEvent(g_hEvent);
	//ResetEvent(g_hEvent);
	g_bConnect = true;
	return 0;
}
DWORD WINAPI SendThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char buf[256] = { 0, };
	while (1)
	{
		ZeroMemory(buf, sizeof(char) * 256);
		fgets(buf, 256, stdin);
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0;
		if (strlen(buf) == 0 ||	_stricmp(buf, "quit") == 0)
		{
			SendMsg(sock, buf, PACKET_GAME_END);
			break; // 엔터는 종료!
		}

		int iSendByte = SendMsg(sock, buf, PACKET_CHAT_MSG);
		if (iSendByte == SOCKET_ERROR || iSendByte == 0)
		{
			break;
		}
		printf("\t%d 바이트를 전송하였습니다.", iSendByte);
	}
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	return 0;
}
DWORD WINAPI RecvThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char buf[2048] = { 0, };
	printf("\n보낼 데이터 입력하시오?");
	int iRecvByte = 0;
	int iStartByte = 0;
	ZeroMemory(buf, sizeof(char) * 2048);
	bool bConnect = true;
	while (bConnect)
	{		
		ZeroMemory(buf, sizeof(char) * 2048);
		int iLen = 0;
		iLen = recv(sock, &buf[iStartByte], PACKET_HEADER_SIZE-iRecvByte, 0);
		iRecvByte += iLen;
		if (iLen == 0 || iLen == SOCKET_ERROR)
		{
			printf("[연결종료]: %s", "서버 문제로 인하여 종료되었다.");
			break;
		}
		if (iRecvByte == PACKET_HEADER_SIZE)
		{
			UPACKET* pPacket = (UPACKET*)&buf;
			int iRecvMsg = 0;
			while (iRecvMsg < (pPacket->ph.len))
			{
				iRecvMsg = recv(sock, &buf[iRecvByte],pPacket->ph.len, 0);
				iRecvByte += iRecvMsg;
				if (iRecvMsg == 0 || iRecvMsg == SOCKET_ERROR)
				{
					printf("[연결종료]: %s", "서버 문제로 인하여 종료되었다.");
					bConnect = false;
					break;
				}
			}			
			UPACKET recvmsg;
			memset(&recvmsg, 0, sizeof(recvmsg));
			memcpy(&recvmsg, pPacket, pPacket->ph.len + PACKET_HEADER_SIZE);
			switch (recvmsg.ph.type)
			{
				//case PACKET_JOIN;
				//case PACKET_DROP;
				case PACKET_CHAT_MSG:
				{
					printf("\n[받은메세지]: %s", recvmsg.msg);
					printf("\n보낼 데이터 입력하시오?");
				}break;
			}		
			iStartByte = iRecvByte = 0;			
		}
		else
		{
			iStartByte += iRecvByte;
		}
	}
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	return 0;
}
int main(int argc, char* argv[])
{
	HANDLE hExecuteMutex= CreateMutex(NULL, FALSE, L"OnceClinetMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hExecuteMutex);
		MessageBox(NULL, L"이미 다른 인스턴스가 실행 중입니다.", L"알림", MB_OK);
		return 1;
	}
	// 2 : FALSE(자동리셋 이벤트 ),TRUE( 수동리셋 이벤트 )
	// 3 : 초기값(FALSE: 논시그널, TRUE:시그널)
	g_hEvent = CreateEvent(NULL, TRUE, FALSE, L"ConnectEvent");

	HANDLE hSendThread, hRecvThread, hConnectThread;
	WSADATA wsa;
	int iRet;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 1; }
	{
		// 정상 접속
		DWORD dwConnectThreadID;
		hConnectThread = CreateThread(0, 0,
			ConnectThread,
			(LPVOID)0,
			0,
			&dwConnectThreadID);

		// 1)스레드의 종료를 확인하는 방법
			//WaitForSingleObject(hConnectThread, INFINITE);
		// 2)전역변수 사용방법
			//while (g_bConnect == false);
		// 3)접속 성공의 이벤트 사용방법
			WaitForSingleObject(g_hEvent, INFINITE);
			//ResetEvent(g_hEvent);
		
		{
			// 정상 접속
			DWORD dwSendThreadID;

			hSendThread = CreateThread(0, 0,
				SendThread,
				(LPVOID)sock,
				0,
				&dwSendThreadID);
			DWORD dwRecvThreadID;
			hRecvThread = CreateThread(0, 0,
				RecvThread,
				(LPVOID)sock,
				0,
				&dwRecvThreadID);
		}

	}
	// 스레드가 종료되었는지 확인
	WaitForSingleObject(hSendThread, INFINITE);
	WaitForSingleObject(hRecvThread, INFINITE);

	CloseHandle(hSendThread);
	CloseHandle(hRecvThread);

	// 윈도우 소켓 소멸
	iRet = WSACleanup();
	CloseHandle(hExecuteMutex);
	CloseHandle(g_hEvent);
	return iRet;
}

