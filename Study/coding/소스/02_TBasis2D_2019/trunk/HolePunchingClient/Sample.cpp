#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
using namespace std;

#define BUFSIZE 512

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (LPCSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

int GetIPList(vector<string>& ipList)
{
	char szHostName[1024];
	string strHostName;
	string strIPAddr = "";
	int  i;
	WSADATA WsaData;
	if (WSAStartup(0x202, &WsaData) == SOCKET_ERROR)
	{
		return 0;
	}
	if (!::gethostname(szHostName, 1024))
	{
		strHostName = szHostName;
		struct hostent* pHost;
		pHost = gethostbyname(szHostName);

		if (pHost != NULL)
		{

			for (i = 0; i<255; i++)
			{
				if (pHost->h_addr_list[i])
				{
					BYTE    bAddr[4];
					char    strIPAddr[256] = { 0, };
					bAddr[0] = (BYTE)pHost->h_addr_list[i][0];
					bAddr[1] = (BYTE)pHost->h_addr_list[i][1];
					bAddr[2] = (BYTE)pHost->h_addr_list[i][2];
					bAddr[3] = (BYTE)pHost->h_addr_list[i][3];
					sprintf(strIPAddr, "%d.%d.%d.%d", bAddr[0], bAddr[1], bAddr[2], bAddr[3]);
					ipList.push_back(strIPAddr);
				}
				else
				{
					break;
				}
			}
		}
	}
	WSACleanup();
	return ipList.size();
}


int main(int argc, char* argv[])
{
	int retval;
	vector<string> ipList;
	GetIPList(ipList);
	char* serverIP = &ipList[0].at(0);

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	int optval = 1;
	SOCKET SendSock = socket(AF_INET, SOCK_DGRAM, 0);
	if (SendSock == INVALID_SOCKET) err_quit("socket()");
	setsockopt(SendSock, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval));

	/////////////////////////////////////////////////////////////////
	// 서버 중계 전송
	/////////////////////////////////////////////////////////////////
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = inet_addr("192.168.0.16");
	// 데이터 통신에 사용할 변수
	int addrlen;
	char buf[BUFSIZE + 1] = "start";
	int len;
	// 데이터 보내기
	retval = sendto(SendSock, buf, strlen(buf), 0, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("socket()");
	}

	//// 소켓 주소 구조체 초기화
	SOCKADDR_IN recvaddr;

	// 데이터 받기
	addrlen = sizeof(recvaddr);
	retval = recvfrom(SendSock, buf, BUFSIZE, 0, (SOCKADDR *)&recvaddr, &addrlen);
	if (retval == SOCKET_ERROR)
	{
		err_quit("socket()");
	}
	buf[retval] = '\0';
	printf("[UDP 클라이언트] %d바이트를 받았습니다.\n", retval);
	printf("[UDP/%s:%d] %s\n", inet_ntoa(recvaddr.sin_addr), htons(recvaddr.sin_port), buf);


	/////////////////////////////////////////////////////////////////
	// 직접 전송
	/////////////////////////////////////////////////////////////////
	char port[256] = { 0, };
	int  iport;
	sscanf(buf, "%s %d", port, &iport);
	SOCKADDR_IN peeraddr;
	ZeroMemory(&peeraddr, sizeof(peeraddr));
	peeraddr.sin_family = AF_INET;
	peeraddr.sin_port = htons(iport);
	peeraddr.sin_addr.s_addr = inet_addr(port);

	// 서버와 데이터 통신
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		strcpy(buf, serverIP);

		retval = sendto(SendSock, buf, strlen(buf), 0, (SOCKADDR *)&peeraddr, sizeof(peeraddr));
		if (retval == SOCKET_ERROR)
		{
			err_quit("socket()");
		}
		printf("[UDP 클라이언트] %d바이트를 보냈습니다.\n", retval);
		printf("[UDP/%s:%d] %s\n", inet_ntoa(peeraddr.sin_addr),
			ntohs(peeraddr.sin_port), buf);

		memset(buf, 0, sizeof(buf));
		addrlen = sizeof(recvaddr);
		retval = recvfrom(SendSock, buf, BUFSIZE, 0, (SOCKADDR *)&recvaddr, &addrlen);
		if (retval == SOCKET_ERROR)
		{
			err_quit("socket()");
		}
		buf[retval] = '\0';
		printf("[UDP 클라이언트] %d바이트를 받았습니다.\n", retval);
		printf("[UDP/%s:%d] %s\n", inet_ntoa(recvaddr.sin_addr),
			ntohs(recvaddr.sin_port), buf);
		Sleep(1000);
	}
	closesocket(SendSock);
	// 윈속 종료
	WSACleanup();
	return 0;
}