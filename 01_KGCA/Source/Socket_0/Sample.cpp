#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Sample.h"
#include <iostream>
#include <winsock2.h>
#pragma comment (lib, "ws2_32.lib")

//클라이언트 
int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 0;
	}
	//소캣
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);//IP, STREAM - TCP // DGRAM- UDP, 프로토콜 안넣어도 됨

	//sockaddr addr;
	//addr.sa_family = AF_INET; //IP 프로토콜 쓴다.
	//addr.sa_data[14]; // ip. port
	SOCKADDR_IN sa; //공용체써서 편한 대문자 버젼
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = inet_addr("192.118.0.0");
	int iRet = connect(sock, (sockaddr*)&sa, sizeof(sa)); //연결


	char szBuffer[256] = "Hello World";
	char szRecvBuffer[256] = { 0, };

	int sendByte = send(sock, szBuffer, sizeof(szBuffer), 0);

	recv(sock, szRecvBuffer, 256, 0);

	std::cout << szBuffer << "\n";

	WSACleanup();
}
