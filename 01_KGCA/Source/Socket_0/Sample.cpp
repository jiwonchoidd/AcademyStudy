#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Sample.h"
#include <iostream>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")

//Ŭ���̾�Ʈ 
int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 0;
	}
	//��Ĺ
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);//IP, STREAM - TCP // DGRAM- UDP, �������� �ȳ־ ��

	//sockaddr addr;
	//addr.sa_family = AF_INET; //IP �������� ����.
	//addr.sa_data[14]; // ip. port
	SOCKADDR_IN sa; //����ü�Ἥ ���� �빮�� ����
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = inet_addr("192.118.0.0");
	int iRet = connect(sock, (sockaddr*)&sa, sizeof(sa)); //����


	char szBuffer[256] = "Hello World";
	char szRecvBuffer[256] = { 0, };

	int sendByte = send(sock, szBuffer, sizeof(szBuffer), 0);

	recv(sock, szRecvBuffer, 256, 0);

	std::cout << szBuffer << "\n";

	WSACleanup();
}
