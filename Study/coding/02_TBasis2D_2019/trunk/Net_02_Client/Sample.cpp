#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#pragma comment(lib, "ws2_32.lib")

HANDLE g_hEvent;
SOCKET sock;
DWORD WINAPI ConnectThread(LPVOID arg)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;//sockaddr name;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);// host byte 정렬 to network byte 정렬 short
	sa.sin_addr.s_addr = inet_addr("192.168.0.101");
	int iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR) { return 1; }
	SetEvent(g_hEvent);
	//ResetEvent(g_hEvent);
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
		if (strlen(buf) == 0 || 
			_stricmp( buf, "quit")==0 )
		{
			break; // 엔터는 종료!
		}
		int iSendByte = send(sock, buf, strlen(buf), 0);
		if (iSendByte == SOCKET_ERROR) break;
		printf("\t%d 바이트를 전송하였습니다.", iSendByte);
	}
	closesocket(sock);
	return 0;
}
DWORD WINAPI RecvThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char buf[256] = { 0, };
	printf("\n보낼 데이터 입력하시오?");
	while (1)
	{
		ZeroMemory(buf, sizeof(char) * 256);
		int iLen = 0;
		int iRecvByte = recv(sock, buf, 256, 0);
		if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
		{
			printf("[연결종료]: %s", "서버 문제로 인하여 종료되었다.");
			break;
		}
		buf[iRecvByte] = 0;
		printf("\n[받은메세지]: %s", buf);						
	}
	closesocket(sock);
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

	// 2 : FALSE(자동리셋 이벤트),TRUE(수동리셋 이벤트)
	// 3 : 초기값(FALSE: 논시그널, TRUE:시그널)
	g_hEvent = CreateEvent(NULL, FALSE, FALSE, L"ConnectEvent");
	//ResetEvent(g_hEvent);

	if (argc != 3)
	{
		printf("\n[사용방법] xxx.exe [port] [ip]");
		printf("\n[사용예시] xxx.exe 10000 192.168.0.100");
		//return 1;		
	}
	//unsigned short iPort = atoi(argv[1]);
	//char* ip = argv[2];

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
		
		//WaitForSingleObject(hConnectThread, INFINITE);
		WaitForSingleObject(g_hEvent, INFINITE);
		//while (g_bConnect == false);
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

