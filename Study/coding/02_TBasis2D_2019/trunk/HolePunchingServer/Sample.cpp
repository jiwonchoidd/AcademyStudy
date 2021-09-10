#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
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

struct TList
{
	char ip[256];
	int  iport;
	SOCKET  sock;
	SOCKADDR_IN clientaddr;
};
int main(int argc, char* argv[])
{
	int retval;

	vector<TList> list;
	char* serverIP = "192.168.0.16";
	if (argc > 1)
	{
		serverIP = argv[1];
	}

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// ������ ��ſ� ����� ����
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	// Ŭ���̾�Ʈ�� ������ ���
	while (1)
	{
		// ������ �ޱ�
		addrlen = sizeof(clientaddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0, (SOCKADDR *)&clientaddr, &addrlen);
		if (retval == SOCKET_ERROR)
		{
			err_display("recvfrom()");
			continue;
		}

		// ���ӵ� ��� ������ ip�� ��� TList�� �����Ѵ�.
		TList data;
		memset(&data, 0, sizeof(TList));
		strcpy(data.ip, inet_ntoa(clientaddr.sin_addr));
		data.iport = htons(clientaddr.sin_port);
		data.clientaddr = clientaddr;
		data.sock = socket(AF_INET, SOCK_DGRAM, 0);
		list.push_back(data);
		// ���� ������ ���
		buf[retval] = '\0';
		printf("[UDP/%s:%d][%d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), list.size(), buf);

		// 2���� Ŭ���̾�Ʈ�� �����Ѵ�.
		// �̴� ���� �ٸ� ��Ʈ��ũ���� ���� ����� �׽�Ʈ�ϴ� �����̴�.
		if (list.size() > 1)
		{
			for (int i = 0; i< list.size(); i++)
			{
				memset(&buf, 0, sizeof(buf));
				// 2���� Ŭ���̾�Ʈ �������� ������ �ּҿ� ��Ʈ�� �˷��ش�.
				if (i == 0)
				{
					sprintf(buf, "%s %d", list[1].ip, list[1].iport);
				}
				else
				{
					sprintf(buf, "%s %d", list[0].ip, list[0].iport);
				}

				retval = sendto(sock, buf, strlen(buf), 0,
					(SOCKADDR *)&list[i].clientaddr, sizeof(clientaddr));
				if (retval == SOCKET_ERROR)
				{
					err_display("sendto()");
					continue;
				}
			}
		}
	}

	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}