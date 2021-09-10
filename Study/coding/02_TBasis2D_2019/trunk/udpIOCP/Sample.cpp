//asynchronous udp + iocp
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

#define BUFSIZE     1024 
#define MAXADDRSTR  16

#define DEFAULT_GROUP_ADDRESS	"224.0.0.255"
#define DEFAULT_PORT			10000 

HANDLE g_hCompletionPort;

DWORD WINAPI WorkerThread(LPVOID WorkContext);

BOOL	HandleIncomingData(UCHAR* pBuf);
BOOL	CreateNetConnections(VOID);
BOOL	CreateWorkers(UINT);
void	InitWinsock2();
void	UnInitWinsock2();

HANDLE			g_hReadEvent;
SOCKET			g_hSocket;
UCHAR			achInBuf[BUFSIZE];
char			achMCAddr[MAXADDRSTR] = DEFAULT_GROUP_ADDRESS;
u_short			nPort = DEFAULT_PORT;


OVERLAPPED Overlapped;


//-----------------------------------------------------------------------------
void InitWinsock2()
{
	WSADATA data;
	WORD version;
	int ret = 0;

	version = (MAKEWORD(2, 2));
	ret = WSAStartup(version, &data);
	if (ret != 0)
	{
		ret = WSAGetLastError();
		if (ret == WSANOTINITIALISED)
		{
			printf("not initialised");
		}
	}
}
//-----------------------------------------------------------------------------
void UnInitWinsock2()
{
	WSACleanup();
}

//-----------------------------------------------------------------------------
BOOL   CreateNetConnections(void)
{
	DWORD nbytes;
	BOOL b;
	BOOL fFlag = TRUE;
	int nRet = 0;
	SOCKADDR_IN stLclAddr;
	struct ip_mreq stMreq;         
								   
	g_hSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (g_hSocket == INVALID_SOCKET)
	{
		printf("socket() failed, Err: %d\n", WSAGetLastError());
		return FALSE;
	}

	nRet = setsockopt(g_hSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&fFlag, sizeof(fFlag));
	if (nRet == SOCKET_ERROR)
	{
		printf("setsockopt() SO_REUSEADDR failed, Err: %d\n", WSAGetLastError());
	}

	stLclAddr.sin_family = AF_INET;
	stLclAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	stLclAddr.sin_port = htons(nPort);
	nRet = bind(g_hSocket, (struct sockaddr*) &stLclAddr, sizeof(stLclAddr));
	if (nRet == SOCKET_ERROR)
	{
		printf("bind() port: %d failed, Err: %d\n", nPort, WSAGetLastError());
	}
	stMreq.imr_multiaddr.s_addr = inet_addr(achMCAddr);
	stMreq.imr_interface.s_addr = inet_addr("192.168.0.101");
	nRet = setsockopt(g_hSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&stMreq, sizeof(stMreq));
	if (nRet == SOCKET_ERROR)
	{
		printf("IP_ADD_MEMBERSHIP address %s failed, Err: %d\n", achMCAddr, WSAGetLastError());
	}


	g_hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 10);
	if (!g_hCompletionPort)
	{
		fprintf(stdout, "g_hCompletionPort Create Failed\n");
		return FALSE;
	}

	CreateIoCompletionPort((HANDLE)g_hSocket, g_hCompletionPort, (DWORD)g_hSocket, 10);


	Overlapped.hEvent = g_hReadEvent;
	Overlapped.Internal = 0;
	Overlapped.InternalHigh = 0;
	Overlapped.Offset = 0;
	Overlapped.OffsetHigh = 0;

	b = ReadFile((HANDLE)g_hSocket, &achInBuf, sizeof(achInBuf), &nbytes, &Overlapped);

	if (!b && GetLastError() != ERROR_IO_PENDING)
	{
		fprintf(stdout, "ReadFile Failed\n");
		return FALSE;
	}

	return TRUE;
}

//-----------------------------------------------------------------------------
BOOL   CreateWorkers(UINT dwNumberOfWorkers)
{
	DWORD ThreadId;
	HANDLE ThreadHandle;
	DWORD i;

	for (i = 0; i < dwNumberOfWorkers; i++)
	{
		ThreadHandle = CreateThread(NULL, 0, WorkerThread, NULL, 0, &ThreadId);
		if (!ThreadHandle)
		{
			fprintf(stdout, "Create Worker Thread Failed\n");
			return FALSE;
		}

		CloseHandle(ThreadHandle);
	}
	return TRUE;
}

//-----------------------------------------------------------------------------
DWORD   WINAPI WorkerThread(LPVOID WorkContext)
{

	ULONG_PTR	nSocket;
	BOOL ret;
	OVERLAPPED ovl;
	LPOVERLAPPED lpo = &ovl;
	DWORD nBytesRead = 0;
	DWORD nBytesToBeRead;
	UCHAR ReadBuffer[BUFSIZE];
	LPVOID lpMsgBuf;

	memset(&ReadBuffer, 0, BUFSIZE);


	for (;;)
	{
		ret = GetQueuedCompletionStatus(g_hCompletionPort, &nBytesToBeRead, &nSocket, &lpo, INFINITE);

		if (ret || lpo)
		{
			if (ret)
			{
				OVERLAPPED ol;
				ol.hEvent = g_hReadEvent;
				ol.Offset = 0;
				ol.OffsetHigh = 0;

				ret = ReadFile((HANDLE)nSocket, &ReadBuffer, nBytesToBeRead, &nBytesRead, &ol);
				if (!ret)
				{
					DWORD dwErrCode = GetLastError();
					if (dwErrCode != ERROR_IO_PENDING)
					{
						printf("Error code - %d\n", dwErrCode);

						FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
							FORMAT_MESSAGE_FROM_SYSTEM |
							FORMAT_MESSAGE_IGNORE_INSERTS,
							NULL,
							dwErrCode,
							MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
							(LPTSTR)&lpMsgBuf,
							0,
							NULL
						);

						OutputDebugString((LPCTSTR)lpMsgBuf);

						// Free the buffer.
						LocalFree(lpMsgBuf);
					}
					else
						if (dwErrCode == ERROR_IO_PENDING)
						{
							WaitForSingleObject(ol.hEvent, INFINITE);

							HandleIncomingData(ReadBuffer);
						}
				}
				else
				{
					HandleIncomingData(ReadBuffer);
				}

				continue;
			}
			else
			{
				fprintf(stdout, "WorkThread Wait Failed\n");
				//exit (1);
			}
		}
		return 1;
	}
}

////////////////////////////////////////////////////////////////
//SYSTEMTIME st, lt;
//GetSystemTime(&st);
//GetLocalTime(&lt);
//printf("The system time is: %02d:%02d\n", st.wHour, st.wMinute);
//printf(" The local time is: %02d:%02d\n", lt.wHour, lt.wMinute);
//-----------------------------------------------------------------------------
BOOL HandleIncomingData(UCHAR* pBuf)
{
	SYSTEMTIME *lpstSysTime;

	lpstSysTime = (SYSTEMTIME *)(pBuf);

	printf("UTC Time %02d:%02d:%02d:%03d on %02d-%02d-%d \n",
		lpstSysTime->wHour, lpstSysTime->wMinute,
		lpstSysTime->wSecond, lpstSysTime->wMilliseconds,
		lpstSysTime->wMonth, lpstSysTime->wDay, lpstSysTime->wYear);

	memset(&pBuf, 0, BUFSIZE); 

	return TRUE;
}


int  main()
{
	printf("\n***************************************\n");
	printf("Group IP address: %s\n", achMCAddr);
	printf("Port number		: %d\n", nPort);
	printf("\n***************************************\n");

	InitWinsock2();

	HANDLE hWait2Exit = CreateEvent(NULL, FALSE, TRUE, L"MCLIENT");
	ResetEvent(hWait2Exit);

	g_hReadEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);

	if (!CreateNetConnections())
	{
		printf("Error\n");
		return 1;
	}

	if (!CreateWorkers(5))
	{
		printf("Error\n");
		return 1;
	}

	WaitForSingleObject(hWait2Exit, INFINITE);

	UnInitWinsock2();

	return 1;
}

