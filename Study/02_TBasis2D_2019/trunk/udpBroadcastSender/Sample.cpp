#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
//#include <winsock2.h>
//#include <windows.h>
//#include <stdio.h>
//#pragma comment(lib, "ws2_32.lib")
//#define MAX_BUF_SIZE 1048
//void Error(char* msg)
//{
//	LPVOID lpMsgBuffer;
//	int iError = WSAGetLastError();
//	FormatMessageA(
//		FORMAT_MESSAGE_ALLOCATE_BUFFER |
//		FORMAT_MESSAGE_FROM_SYSTEM, NULL,
//		iError,
//		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
//		(LPSTR)&lpMsgBuffer,
//		0, NULL);
//	printf("%s", lpMsgBuffer);
//	LocalFree(lpMsgBuffer);
//	exit(-1);
//}
//
//void main()
//{
//	HANDLE m_hReadFile = ::CreateFile(L"../../data/bitmap1.bmp", GENERIC_READ, 0, 0, OPEN_EXISTING,
//		FILE_ATTRIBUTE_NORMAL, 0);
//
//	WSADATA wd;
//	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
//	{
//		return;
//	}
//	SOCKET sock = socket(AF_INET,
//		SOCK_DGRAM,
//		0);
//	if (sock == INVALID_SOCKET)
//	{
//		Error("sock invalid");
//	}
//	BOOL bEnable = TRUE;
//	setsockopt(sock, SOL_SOCKET, 
//		SO_BROADCAST, 
//		(char*)&bEnable,
//		sizeof(bEnable));
//
//	SOCKADDR_IN serveradd;
//	ZeroMemory(&serveradd, sizeof(serveradd));
//	serveradd.sin_family = AF_INET;
//	serveradd.sin_port = htons(9009);
//	serveradd.sin_addr.s_addr =	INADDR_BROADCAST;
//
//	char buf[MAX_BUF_SIZE] = { 0, };
//	char recvbuf[MAX_BUF_SIZE] = { 0, };
//	char endbuffer[] = "end";
//	int addlen= sizeof(serveradd);;
//	int iCount = 0;
//	int iTotalSize = 0;
//
//	while (1)
//	{
//		DWORD byteRead = 0;
//		DWORD ret = ReadFile(m_hReadFile,
//			buf,
//			MAX_BUF_SIZE, &byteRead,
//			NULL);
//		if (byteRead == 0 || ret == FALSE)
//		{
//			//if (ERROR_HANDLE_EOF == ::GetLastError())
//			{
//				sendto(sock, buf, strlen(endbuffer), 0,	(SOCKADDR*)&serveradd, addlen);
//				break;
//			}
//		}
//		int iRet = sendto(sock, buf, byteRead, 0,	(SOCKADDR*)&serveradd, addlen);
//		if (iRet == 0) break;
//		iTotalSize += iRet;
//		printf("\n%d[%d]", iTotalSize, iCount++);
//
//		Sleep(1000);
//	}
//	CloseHandle(m_hReadFile);
//	WSACleanup();
//}


#include <winsock2.h>
#include<iostream>
#include<conio.h>
using namespace std;
#pragma comment(lib, "ws2_32.lib")
#define MYPORT 9009    // the port users will be connecting to

int main()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SOCKET sock;
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	char broadcast = '1';
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
	{
		cout << "Error in setting Broadcast option";
		closesocket(sock);
		return 0;
	}
	struct sockaddr_in Recv_addr;
	struct sockaddr_in Sender_addr;
	int len = sizeof(struct sockaddr_in);
	char sendMSG[] = "Broadcast message from KGCA GAME ACADEMY";
	char recvbuff[50] = "";
	int recvbufflen = 50;

	Recv_addr.sin_family = AF_INET;
	Recv_addr.sin_port = htons(MYPORT);
	Recv_addr.sin_addr.s_addr = INADDR_BROADCAST;

	sendto(sock, sendMSG, strlen(sendMSG) + 1, 0, (sockaddr *)&Recv_addr, sizeof(Recv_addr));
	recvfrom(sock, recvbuff, recvbufflen, 0, (sockaddr *)&Recv_addr, &len);
	cout << "\n\n\tReceived message from Reader is =>  " << recvbuff;
	cout << "\n\n\tpress any key to CONT...";
	_getch();
	closesocket(sock);
	WSACleanup();
}