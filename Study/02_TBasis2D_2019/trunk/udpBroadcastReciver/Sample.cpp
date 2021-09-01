//#include <winsock2.h>
//#include <stdio.h>
//#pragma comment(lib, "ws2_32.lib")
//#define MAX_BUF_SIZE 1048
//
//void main()
//{
//	HANDLE m_hWriteFile =
//		::CreateFile(L"xxx.bmp",
//			GENERIC_WRITE, 0, 0,
//			CREATE_ALWAYS,
//			FILE_ATTRIBUTE_NORMAL, 0);
//
//	WSADATA wsa;
//	WSAStartup(MAKEWORD(2, 2),	&wsa);
//	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
//		
//	BOOL bEnable = TRUE;
//	if (setsockopt(sock, SOL_SOCKET,
//		SO_BROADCAST,
//		(char*)&bEnable,
//		sizeof(bEnable)) < 0)
//	{
//		return ;
//	}
//
//	SOCKADDR_IN addr;
//	ZeroMemory(&addr, sizeof(addr));
//	addr.sin_family = AF_INET;
//	addr.sin_port = htons(9009);
//	addr.sin_addr.s_addr = INADDR_ANY;
//	bind(sock, (SOCKADDR*)&addr, sizeof(addr));
//
//	SOCKADDR_IN peeraddr;
//	char buf[MAX_BUF_SIZE+1] = { 0, };
//	int addrlen;
//	int iTotalSize = 0;
//	while (1)
//	{
//		addrlen = sizeof(peeraddr);
//		int iRet = recvfrom(sock, buf, MAX_BUF_SIZE+1, 0, (SOCKADDR*)&peeraddr, &addrlen);
//		if (iRet == SOCKET_ERROR)
//		{
//			break;
//		}
//		if (_stricmp(buf, "end") == 0)
//		{
//			break;
//		}
//		DWORD writeBytes = 0;
//		WriteFile(m_hWriteFile, buf, iRet, &writeBytes, NULL);
//		iTotalSize += writeBytes;
//		printf("\n%d", iTotalSize);
//	}
//
//	CloseHandle(m_hWriteFile);
//	closesocket(sock);
//	WSACleanup();
//}

#include "winsock2.h"
#include <iostream>
#include <conio.h>
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
		closesocket(sock);
		return 0;
	}
	struct sockaddr_in Recv_addr;
	struct sockaddr_in Sender_addr;
	int len = sizeof(struct sockaddr_in);
	char recvbuff[50];
	int recvbufflen = 50;
	char sendMSG[] = "Broadcast message from READER";
	Recv_addr.sin_family = AF_INET;
	Recv_addr.sin_port = htons(MYPORT);
	Recv_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (sockaddr*)&Recv_addr, sizeof(Recv_addr)) < 0)
	{
		cout << "Error in BINDING" << WSAGetLastError();
		_getch();
		closesocket(sock);
		return 0;
	}
	recvfrom(sock, recvbuff, recvbufflen, 0, (sockaddr *)&Sender_addr, &len);
	cout << "\n\n\tReceived Message is : " << recvbuff;
	cout << "\n\n\tPress Any to send message";
	_getch();
	if (sendto(sock, sendMSG, strlen(sendMSG) + 1, 0, (sockaddr *)&Sender_addr, sizeof(Sender_addr)) < 0)
	{
		cout << "Error in Sending." << WSAGetLastError();
		cout << "\n\n\t\t Press any key to continue....";
		_getch();
		closesocket(sock);
		return 0;
	}
	else
		cout << "\n\n\n\tREADER sends the broadcast message Successfully";

	cout << "\n\n\tpress any key to CONT...";
	_getch();
	closesocket(sock);
	WSACleanup();
}