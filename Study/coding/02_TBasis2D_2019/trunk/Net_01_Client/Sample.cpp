#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

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
		if (strlen(buf) == 0) break; // 엔터는 종료!
		int iSendByte = send(sock, buf, strlen(buf), 0);
		if (iSendByte == SOCKET_ERROR) break;
		printf("\t%d 바이트를 전송하였습니다.", iSendByte);
	}
	return 0;
}

int main(int argc, char* argv[])
{	
	HANDLE hExecuteMutex = CreateMutex(NULL, FALSE, L"OnceClinetMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hExecuteMutex);
		MessageBox(NULL, L"이미 다른 인스턴스가 실행 중입니다.", L"알림", MB_OK);
		return 1;
	}

	// 속성->구성요소->디버깅->명령인수=10000 192.168.0.100
	if (argc != 3)
	{
		printf("\n[사용방법] xxx.exe [port] [ip]");
		printf("\n[사용예시] xxx.exe 10000 192.168.0.100");
		return 1;		
	}

	HANDLE hThread;
	unsigned short iPort = atoi(argv[1]);
	char* ip = argv[2];

	WSADATA wsa;
	int iRet;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { return 1; }
	{
		SOCKET sock;
		sock = socket(AF_INET, SOCK_STREAM, 0);
		{
			SOCKADDR_IN sa;//sockaddr name;
			ZeroMemory(&sa, sizeof(sa));
			sa.sin_family = AF_INET;
			sa.sin_port = htons(iPort);// host byte 정렬 to network byte 정렬 short
			sa.sin_addr.s_addr = inet_addr(ip);
			iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
			if (iRet == SOCKET_ERROR) { return 1; }
			// 정상 접속
			DWORD dwSendThreadID;
			hThread = CreateThread(	0, 0, 
							SendThread,
							(LPVOID)sock, 
							0, 
							&dwSendThreadID);
			{
				char buf[256] = { 0, };
				int iLen = 0;
				while (1)
				{
					int iRecvByte = recv(sock, buf, 256, 0);
					if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
					{
						printf("[연결종료]: %s", "서버 문제로 인하여 종료되었다.");
						break;
					}
					buf[iRecvByte] = 0;
					printf("\n[받은메세지]: %s", buf);				
					printf("\n보낼 데이터 입력하시오?");
				}
			}
		}
		closesocket(sock);

	}
	CloseHandle(hThread);
	// 윈도우 소켓 소멸
	iRet = WSACleanup();
	CloseHandle(hExecuteMutex);
	return iRet;
}