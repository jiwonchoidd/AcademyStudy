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

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	int optval = 1;
	SOCKET SendSock = socket(AF_INET, SOCK_DGRAM, 0);
	if (SendSock == INVALID_SOCKET) err_quit("socket()");
	setsockopt(SendSock, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval));

	/////////////////////////////////////////////////////////////////
	// ���� �߰� ����
	/////////////////////////////////////////////////////////////////
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = inet_addr("192.168.0.16");
	// ������ ��ſ� ����� ����
	int addrlen;
	char buf[BUFSIZE + 1] = "start";
	int len;
	// ������ ������
	retval = sendto(SendSock, buf, strlen(buf), 0, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR)
	{
		err_quit("socket()");
	}

	//// ���� �ּ� ����ü �ʱ�ȭ
	SOCKADDR_IN recvaddr;

	// ������ �ޱ�
	addrlen = sizeof(recvaddr);
	retval = recvfrom(SendSock, buf, BUFSIZE, 0, (SOCKADDR *)&recvaddr, &addrlen);
	if (retval == SOCKET_ERROR)
	{
		err_quit("socket()");
	}
	buf[retval] = '\0';
	printf("[UDP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
	printf("[UDP/%s:%d] %s\n", inet_ntoa(recvaddr.sin_addr), htons(recvaddr.sin_port), buf);


	/////////////////////////////////////////////////////////////////
	// ���� ����
	/////////////////////////////////////////////////////////////////
	char port[256] = { 0, };
	int  iport;
	sscanf(buf, "%s %d", port, &iport);
	SOCKADDR_IN peeraddr;
	ZeroMemory(&peeraddr, sizeof(peeraddr));
	peeraddr.sin_family = AF_INET;
	peeraddr.sin_port = htons(iport);
	peeraddr.sin_addr.s_addr = inet_addr(port);

	// ������ ������ ���
	while (1)
	{
		memset(buf, 0, sizeof(buf));
		strcpy(buf, serverIP);

		retval = sendto(SendSock, buf, strlen(buf), 0, (SOCKADDR *)&peeraddr, sizeof(peeraddr));
		if (retval == SOCKET_ERROR)
		{
			err_quit("socket()");
		}
		printf("[UDP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);
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
		printf("[UDP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
		printf("[UDP/%s:%d] %s\n", inet_ntoa(recvaddr.sin_addr),
			ntohs(recvaddr.sin_port), buf);
		Sleep(1000);
	}
	closesocket(SendSock);
	// ���� ����
	WSACleanup();
	return 0;
}