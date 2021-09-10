// EchoServer.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <Ws2tcpip.h> //InetNtop(), inet_ntop()
#include <vector>
#include <list>
#include <tchar.h>
#pragma comment( lib, "ws2_32.lib")
#define MAX_BUFFER_SIZE 256
const int g_MaxUser = 3;

static void T_ERR_EXIT(TCHAR *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(TCHAR*)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (TCHAR*)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	//exit(-1);
}
int main()
{
	WSADATA wd;
	int iRet = WSAStartup(MAKEWORD(2, 2), &wd);
	if (iRet != 0)
	{
		return 1;
	}
	SOCKET listensock = socket(
		AF_INET,
		SOCK_STREAM,  //SOCK_DGRAM
		0);// IPPROTO_TCP );  //IPPROTO_UDP

	//u_long on = TRUE;
	//ioctlsocket(listensock, FIONBIO, &on);

	SOCKADDR_IN sa;//socket address
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY); //inet_addr("192.168.0.13");
	sa.sin_port = htons(10000);
	iRet = bind(listensock, (sockaddr*)&sa, sizeof(sa));// socket + (ip+port) 연결
	if (iRet == SOCKET_ERROR) return 1;

	iRet = listen(listensock, SOMAXCONN);
	if (iRet == SOCKET_ERROR) return 1;

	SOCKADDR_IN clientAddr;

	int iUserCnt = 0;
	std::list<SOCKET> userlist;
	SOCKET client;
	while (iUserCnt < g_MaxUser)
	{
		int addlen = sizeof(clientAddr);
	
		client = accept(listensock,	(SOCKADDR*)&clientAddr, &addlen);		
		if (client == SOCKET_ERROR)// 오류!
		{
			// 논블록형 소켓일 경우에 오류(WSAEWOULDBLOCK)라면 정상적인 반환이다.
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				T_ERR_EXIT(_T("accept"));
				break;
			}
		}
		else
		{
			// 서버소켓과 무관하게 클라이언트 소켓을 넌블록형소켓으로 변환한다.
			u_long on = 1;
			ioctlsocket(client, FIONBIO, &on);

			char ip[INET_ADDRSTRLEN]; 
			//printf("\nip= %s, port=%d", inet_ntoa(clientAddr.sin_addr),	ntohs(clientAddr.sin_port));
			printf("\nip= %s, port=%d", inet_ntop(AF_INET, &(clientAddr.sin_addr), ip, INET_ADDRSTRLEN), ntohs(clientAddr.sin_port));
			//char ip6[INET6_ADDRSTRLEN]; 
			//struct sockaddr_in6 sa6;    
			//inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);
			userlist.push_back(client);
			iUserCnt++;
		}
	}

	char buf[MAX_BUFFER_SIZE] = { 0, };
	while (userlist.size() > 0)
	{
		memset(buf, 0, sizeof(char) * MAX_BUFFER_SIZE);
		std::list<SOCKET>::iterator iter;
		for (iter = userlist.begin(); iter != userlist.end(); )
		{
			SOCKET clientSock = *iter;
			int iRecvSize = recv(clientSock, buf, MAX_BUFFER_SIZE-1, 0);
			if (iRecvSize == 0)
			{				
				char ip[INET_ADDRSTRLEN];
				//printf("\nExit! ip= %s, port=%d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
				printf("\nExit! ip= %s, port=%d", inet_ntop(AF_INET, &(clientAddr.sin_addr), ip, INET_ADDRSTRLEN), ntohs(clientAddr.sin_port));
				closesocket(clientSock);
				iter = userlist.erase(iter);				
			}
			if (iRecvSize == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSAEWOULDBLOCK)
				{
					// 치명적인 오류.
					T_ERR_EXIT(_T("recv"));
					closesocket(clientSock);
					iter = userlist.erase(iter);
				}
			}
			// 리스트 선단의 유저가 계속 메시지를 전달하면 뒤 유저의 메시지를 받지 못하는 문제가 있다.
			if (iRecvSize > 0)
			{
				break;
			}
		}

		if (strlen(buf) > 0)
		{
			printf("\n%s", buf);
			// 브로드케스팅
			for (iter = userlist.begin(); iter != userlist.end(); )
			{
				SOCKET clientSock = *iter;
				int iSendSize = send(clientSock, buf, strlen(buf), 0);
				if (iSendSize == SOCKET_ERROR)
				{
					if (WSAGetLastError() != WSAEWOULDBLOCK)
					{
						T_ERR_EXIT(_T("send"));
						closesocket(clientSock);
						iter = userlist.erase(iter);
					}
				}
			}
		}
		Sleep(1);
	}

	printf("\n%s", "서버종료됨.");
	closesocket(listensock);
	// 윈속해제
	WSACleanup();
	return 0;
}

