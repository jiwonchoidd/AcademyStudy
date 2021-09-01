// AsyncSelectSvr.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "TAsyncSelect.h"

#define DEFAULT_BUFFER_SIZE     4096    // default buffer size

// Window message for the socket events
#define WM_SOCKET               (WM_USER + 10)

int gAddressFamily = AF_UNSPEC,         // default to unspecified
gSocketType = SOCK_STREAM,       // default to TCP socket type
gProtocol = IPPROTO_TCP,       // default to TCP protocol
gBufferSize = DEFAULT_BUFFER_SIZE;

char *gBindAddr = NULL,              // local interface to bind to
*gBindPort = "10000";            // local port to bind to

HWND  gWorkerWindow = NULL;       // Window to post socket events to

								  //
								  // Allocated for each receiver posted
								  //
typedef struct _BUFFER_OBJ
{
	char        *buf;           // Data buffer for data
	int          buflen;        // Length of buffer or number of bytes contained in buffer

	SOCKADDR_STORAGE addr;      // Address data was received from (UDP)
	int              addrlen;   // Length of address

	struct _BUFFER_OBJ *next;   // Links buffer objects together
} BUFFER_OBJ;

//
// Allocated for each socket handle
//
typedef struct _SOCKET_OBJ
{
	SOCKET      s;              // Socket handle
	int         closing;        // Indicates whether the connection is closing

	SOCKADDR_STORAGE addr;      // Used for client's remote address
	int              addrlen;   // Length of the address

	BUFFER_OBJ *pending,        // List of pending buffers to be sent
		*pendingtail;    // Last entry in buffer list

	struct _SOCKET_OBJ *next,   // Used to lick socket objects together
		*prev;
	CRITICAL_SECTION    SockCritSec;    // Synchronize access to socket object
} SOCKET_OBJ;

SOCKET_OBJ *gSocketList = NULL,           // Linked list of all sockets allocated
*gSocketListEnd = NULL;        // End of socket list
int         gSocketCount = 0;             // Nubmer of socket objects in list
CRITICAL_SECTION gSocketCritSec;        // Syncrhonize access to socket list

										//
										// Statistics counters
										//
volatile LONG gBytesRead = 0,
gBytesSent = 0,
gStartTime = 0,
gBytesReadLast = 0,
gBytesSentLast = 0,
gStartTimeLast = 0,
gCurrentConnections = 0;

void usage(char *progname)
{
	fprintf(stderr, "usage: %s [-a 4|6] [-e port] [-l local-addr] [-p udp|tcp]\n",
		progname);
	fprintf(stderr, "  -a 4|6     Address family, 4 = IPv4, 6 = IPv6 [default = IPv4]\n"
		"  -b size    Size of send/recv buffer in bytes [default = %d]\n"
		"  -e port    Port number [default = %s]\n"
		"  -l addr    Local address to bind to [default INADDR_ANY for IPv4 or INADDR6_ANY for IPv6]\n"
		"  -p tcp|udp Which protocol to use [default = TCP]\n",
		gBufferSize,
		gBindPort
		);
	ExitProcess(-1);
}

BUFFER_OBJ *GetBufferObj(int buflen)
{
	BUFFER_OBJ *newobj = NULL;

	// 프로세스의 전역 힙 영역에서 할당 받는다.(Global Heap) Allocate the object
	newobj = (BUFFER_OBJ *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BUFFER_OBJ));
	if (newobj == NULL)
	{
		fprintf(stderr, "GetBufferObj: HeapAlloc failed: %d\n", GetLastError());
		ExitProcess(-1);
	}
	// Allocate the buffer
	newobj->buf = (char *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(BYTE) *buflen);
	if (newobj->buf == NULL)
	{
		fprintf(stderr, "GetBufferObj: HeapAlloc failed: %d\n", GetLastError());
		ExitProcess(-1);
	}
	newobj->buflen = buflen;

	newobj->addrlen = sizeof(newobj->addr);

	return newobj;
}

//
// Function: FreeBufferObj
// 
// Description:
//    Free the buffer object.
//
void FreeBufferObj(BUFFER_OBJ *obj)
{
	HeapFree(GetProcessHeap(), 0, obj->buf);
	HeapFree(GetProcessHeap(), 0, obj);
}

SOCKET_OBJ *GetSocketObj(SOCKET s)
{
	SOCKET_OBJ  *sockobj = NULL;

	sockobj = (SOCKET_OBJ *)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(SOCKET_OBJ));
	if (sockobj == NULL)
	{
		fprintf(stderr, "GetSocketObj: HeapAlloc failed: %d\n", GetLastError());
		ExitProcess(-1);
	}

	// Initialize the members
	sockobj->s = s;
	sockobj->addrlen = sizeof(sockobj->addr);

	InitializeCriticalSection(&sockobj->SockCritSec);

	return sockobj;
}

void FreeSocketObj(SOCKET_OBJ *obj)
{
	BUFFER_OBJ  *ptr = NULL,
		*tmp = NULL;

	ptr = obj->pending;
	while (ptr)
	{
		tmp = ptr;
		ptr = ptr->next;

		FreeBufferObj(tmp);
	}

	DeleteCriticalSection(&obj->SockCritSec);

	HeapFree(GetProcessHeap(), 0, obj);
}

void InsertSocketObj(SOCKET_OBJ *sock)
{
	EnterCriticalSection(&gSocketCritSec);

	sock->next = sock->prev = NULL;
	if (gSocketList == NULL)
	{
		// List is empty
		gSocketList = gSocketListEnd = sock;
	}
	else
	{
		// Non empty; insert at end
		sock->prev = gSocketListEnd;
		gSocketListEnd->next = sock;
		gSocketListEnd = sock;

	}
	gSocketCount++;

	LeaveCriticalSection(&gSocketCritSec);
}

void RemoveSocketObj(SOCKET_OBJ *sock)
{
	EnterCriticalSection(&gSocketCritSec);

	if (sock->prev)
	{
		sock->prev->next = sock->next;
	}
	if (sock->next)
	{
		sock->next->prev = sock->prev;
	}

	if (gSocketList == sock)
		gSocketList = sock->next;
	if (gSocketListEnd == sock)
		gSocketListEnd = sock->prev;

	gSocketCount--;

	LeaveCriticalSection(&gSocketCritSec);
}

SOCKET_OBJ *FindSocketObj(SOCKET s)
{
	SOCKET_OBJ *ptr = NULL;

	EnterCriticalSection(&gSocketCritSec);

	ptr = gSocketList;
	while (ptr)
	{
		if (ptr->s == s)
			break;
		ptr = ptr->next;
	}

	LeaveCriticalSection(&gSocketCritSec);

	return ptr;
}

void RemoveSocketObjByHandle(SOCKET s)
{
	SOCKET_OBJ *obj;

	obj = FindSocketObj(s);
	if (obj)
	{
		RemoveSocketObj(obj);
	}
	return;
}

//
// Function: EnqueueBufferObj
//
// Description:
//   Queue up a receive buffer for this connection.
//
void EnqueueBufferObj(SOCKET_OBJ *sock, BUFFER_OBJ *obj, BOOL AtHead)
{
	EnterCriticalSection(&sock->SockCritSec);
	if (sock->pending == NULL)
	{
		// Queue is empty
		sock->pending = sock->pendingtail = obj;
	}
	else if (AtHead == FALSE)
	{
		// Put new object at the end 
		sock->pendingtail->next = obj;
		sock->pendingtail = obj;
	}
	else
	{
		// Put new object at the head
		obj->next = sock->pending;
		sock->pending = obj;
	}
	LeaveCriticalSection(&sock->SockCritSec);
}

BUFFER_OBJ *DequeueBufferObj(SOCKET_OBJ *sock)
{
	BUFFER_OBJ *ret = NULL;

	EnterCriticalSection(&sock->SockCritSec);
	if (sock->pendingtail != NULL)
	{
		// Queue is non empty
		ret = sock->pending;

		sock->pending = sock->pending->next;
		if (sock->pendingtail == ret)
		{
			// Item is the only item in the queue
			sock->pendingtail = NULL;
		}
	}
	LeaveCriticalSection(&sock->SockCritSec);

	return ret;
}

void ValidateArgs(int argc, char **argv)
{
	int     i;

	for (i = 1; i < argc; i++)
	{
		if (((argv[i][0] != '/') && (argv[i][0] != '-')) || (strlen(argv[i]) < 2))
			usage(argv[0]);
		else
		{
			switch (tolower(argv[i][1]))
			{
			case 'a':               // address family - IPv4 or IPv6
				if (i + 1 >= argc)
					usage(argv[0]);
				if (argv[i + 1][0] != '0')
				{
					if (argv[i + 1][0] == '4')
						gAddressFamily = AF_INET;
					else if (argv[i + 1][0] == '6')
						gAddressFamily = AF_INET6;
					else
						usage(argv[0]);
				}
				i++;
				break;
			case 'b':               // buffer size for send/recv
				if (i + 1 >= argc)
					usage(argv[0]);
				gBufferSize = atol(argv[++i]);
				break;
			case 'e':               // endpoint - port number
				if (i + 1 >= argc)
					usage(argv[0]);
				gBindPort = argv[++i];
				break;
			case 'l':               // local address for binding
				if (i + 1 >= argc)
					usage(argv[0]);

				gBindAddr = 0;
				if (argv[++i][0] != '0')
				{
					gBindAddr = argv[i];
				}
				break;
			case 'p':               // protocol - TCP or UDP
				if (i + 1 >= argc)
					usage(argv[0]);
				if (_strnicmp(argv[i + 1], "tcp", 3) == 0)
				{
					gProtocol = IPPROTO_TCP;
					gSocketType = SOCK_STREAM;
				}
				else if (_strnicmp(argv[i + 1], "udp", 3) == 0)
				{
					gProtocol = IPPROTO_UDP;
					gSocketType = SOCK_DGRAM;
				}
				else
					usage(argv[0]);
				i++;
				break;
			default:
				usage(argv[0]);
				break;
			}
		}
	}
}

int ReceivePendingData(SOCKET_OBJ *sockobj)
{
	BUFFER_OBJ *buffobj = NULL;
	int         rc,
		ret;

	// Get a buffer to receive the data
	buffobj = GetBufferObj(gBufferSize);

	ret = 0;

	if (gProtocol == IPPROTO_TCP)
	{
		rc = recv(
			sockobj->s,
			buffobj->buf,
			buffobj->buflen,
			0
			);
	}
	else
	{
		rc = recvfrom(
			sockobj->s,
			buffobj->buf,
			buffobj->buflen,
			0,
			(SOCKADDR *)&buffobj->addr,
			&buffobj->addrlen
			);
	}
	if (rc == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			// Socket connection has failed, close the socket
			fprintf(stderr, "recv(from) failed: %d\n", WSAGetLastError());

			closesocket(sockobj->s);

			ret = -1;
		}
		else
		{
			ret = WSAEWOULDBLOCK;
		}
		FreeBufferObj(buffobj);
	}
	else if (rc == 0)
	{
		// Graceful close
		if (gProtocol == IPPROTO_TCP)
		{
			FreeBufferObj(buffobj);
		}
		else
		{
			buffobj->buflen = 0;
			EnqueueBufferObj(sockobj, buffobj, FALSE);
		}

		printf("Closing\n");
		sockobj->closing = TRUE;

		if (sockobj->pending == NULL)
		{
			// If no sends are pending, close the socket for good
			closesocket(sockobj->s);

			ret = -1;
		}
		else
		{
			ret = WSAEWOULDBLOCK;
		}
	}
	else
	{
		// Read data, updated the counters and enqueue the buffer for sending
		InterlockedExchangeAdd(&gBytesRead, rc);
		InterlockedExchangeAdd(&gBytesReadLast, rc);

		buffobj->buflen = rc;
		EnqueueBufferObj(sockobj, buffobj, FALSE);

	}
	return ret;
}

int SendPendingData(SOCKET_OBJ *sock)
{
	BUFFER_OBJ *bufobj = NULL;
	BOOL        breakouter;
	int         nleft,
		idx,
		ret,
		rc;

	// Attemp to dequeue all the buffer objects on the socket
	ret = 0;
	while (bufobj = DequeueBufferObj(sock))
	{
		if (gProtocol == IPPROTO_TCP)
		{
			breakouter = FALSE;

			nleft = bufobj->buflen;
			idx = 0;

			// In the event not all the data was sent we need to increment
			// through the buffer. This only needs to be done for stream
			// sockets since UDP is datagram and its all or nothing for that.
			while (nleft)
			{
				rc = send(
					sock->s,
					&bufobj->buf[idx],
					nleft,
					0
					);
				if (rc == SOCKET_ERROR)
				{
					if (WSAGetLastError() == WSAEWOULDBLOCK)
					{
						BUFFER_OBJ *newbuf = NULL;


						// Copy the unsent portion of the buffer and put it back
						// at the head of the send queue
						newbuf = GetBufferObj(nleft);
						memcpy(newbuf->buf, &bufobj->buf[idx], nleft);

						EnqueueBufferObj(sock, newbuf, TRUE);

						ret = WSAEWOULDBLOCK;
					}
					else
					{
						// The connection was broken, indicate failure
						ret = -1;
					}
					breakouter = TRUE;

					break;
				}
				else
				{
					InterlockedExchangeAdd(&gBytesSent, rc);
					InterlockedExchangeAdd(&gBytesSentLast, rc);

					nleft -= rc;
					idx += 0;
				}
			}
			FreeBufferObj(bufobj);

			if (breakouter)
				break;
		}
		else
		{
			rc = sendto(
				sock->s,
				bufobj->buf,
				bufobj->buflen,
				0,
				(SOCKADDR *)&bufobj->addr,
				bufobj->addrlen
				);
			if (rc == SOCKET_ERROR)
			{
				if (WSAGetLastError() == WSAEWOULDBLOCK)
				{
					// If the send couldn't be made, put the buffer
					// back at the head of the queue
					EnqueueBufferObj(sock, bufobj, TRUE);

					ret = WSAEWOULDBLOCK;
				}
				else
				{
					// Free all pending sends
					ret = -1;
				}
				break;
			}
			else
			{
				FreeBufferObj(bufobj);

				InterlockedExchangeAdd(&gBytesSent, rc);
				InterlockedExchangeAdd(&gBytesSentLast, rc);
			}
		}
	}
	// If no more sends are pending and the socket was marked as closing (the
	// receiver got zero bytes) then close the socket and indicate to the caller
	// to remove the socket structure.
	if ((sock->pending == NULL) && (sock->closing))
	{
		closesocket(sock->s);
		ret = -1;

		printf("Closing connection\n");
	}
	return ret;
}


void PrintStatistics()
{
	ULONG       bps, tick, elapsed;

	tick = GetTickCount();

	elapsed = (tick - gStartTime) / 1000;

	if (elapsed == 0)
		return;

	printf("\n");

	bps = gBytesSent / elapsed;
	printf("Average BPS sent: %lu [%lu]\n", bps, gBytesSent);

	bps = gBytesRead / elapsed;
	printf("Average BPS read: %lu [%lu]\n", bps, gBytesRead);

	elapsed = (tick - gStartTimeLast) / 1000;

	if (elapsed == 0)
		return;

	bps = gBytesSentLast / elapsed;
	printf("Current BPS sent: %lu\n", bps);

	bps = gBytesReadLast / elapsed;
	printf("Current BPS read: %lu\n", bps);

	printf("Current Connections: %lu\n", gCurrentConnections);

	InterlockedExchange(&gBytesSentLast, 0);
	InterlockedExchange(&gBytesReadLast, 0);

	gStartTimeLast = tick;
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SOCKET_OBJ  *sockobj = NULL,
		*newsock = NULL;
	int          rc;

	if (uMsg == WM_SOCKET)
	{
		// Check for an error on the socket
		if (WSAGETSELECTERROR(lParam))
		{
			// An error occured on the socket, close it down
			fprintf(stderr, "Socket failed with error %d\n", WSAGETSELECTERROR(lParam));
			closesocket(wParam);
			RemoveSocketObjByHandle(wParam);
		}
		else
		{
			// Find the socket object for this event
			sockobj = FindSocketObj(wParam);
			if (sockobj == NULL)
				return 0;

			switch (WSAGETSELECTEVENT(lParam))
			{
			case FD_ACCEPT:

				// Get a new object for the client socket
				newsock = GetSocketObj(INVALID_SOCKET);

				newsock->s = accept(
					wParam,
					(SOCKADDR *)&newsock->addr,
					&newsock->addrlen
					);
				if (newsock->s == INVALID_SOCKET)
				{
					fprintf(stderr, "accept failed: %d\n", WSAGetLastError());
					break;
				}

				InterlockedIncrement(&gCurrentConnections);

				// Create a socket information structure to associate with the
				// socket for processing I/O.
				InsertSocketObj(newsock);

				
				printf("Accepted connection from: ");
				PrintAddress((SOCKADDR *)&newsock->addr, newsock->addrlen);
				printf("\n");
				

				rc = WSAAsyncSelect(
					newsock->s,
					hwnd,
					WM_SOCKET,
					FD_READ | FD_WRITE | FD_CLOSE
					);
				if (rc == SOCKET_ERROR)
				{
					fprintf(stderr, "WSAAsyncSelect failed: %d\n", WSAGetLastError());
					return -1;
				}

				break;
			case FD_READ:
				rc = ReceivePendingData(sockobj);
				if (rc == -1)
				{
					RemoveSocketObj(sockobj);
					break;
				}
				else if (rc != WSAEWOULDBLOCK)
				{
					PostMessage(hwnd, WM_SOCKET, wParam, FD_READ);
				}
				//
				// Don't break fall through and attempt to send data
				//
			case FD_WRITE:
				//
				// Send routine automatically tries to send all queued buffers.
				//
				rc = SendPendingData(sockobj);
				if (rc == -1)
				{
					RemoveSocketObj(sockobj);
				}
				break;
			case FD_CLOSE:
				sockobj->closing = TRUE;
				//
				// Post an FD_READ message to force another receive
				//    This is to ensure we recv() until 0 is returned.
				//
				PostMessage(hwnd, WM_SOCKET, wParam, FD_READ);
				break;
			default:
				printf("Unknown message received: %d\n", WSAGETSELECTEVENT(lParam));
				break;
			}
		}
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


HWND MakeWorkerWindow(void)
{
	WNDCLASS wndclass;
	TCHAR *ProviderClass = L"AsyncSelect";
	HWND Window;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = (WNDPROC)WindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = NULL;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = ProviderClass;

	if (RegisterClass(&wndclass) == 0)
	{
		fprintf(stderr, "RegisterClass() failed with error %d\n", GetLastError());
		return NULL;
	}

	// Create a window.

	if ((Window = CreateWindow(
		ProviderClass,
		L"",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		NULL,
		NULL)) == NULL)
	{
		fprintf(stderr, "CreateWindow() failed with error %d\n", GetLastError());
		return NULL;
	}

	return Window;
}


DWORD WINAPI StatisticsThread(LPVOID lpParam)
{
	while (1)
	{
		SleepEx(5000, TRUE);

		PrintStatistics();
	}
	ExitThread(0);
	return 0;
}


int __cdecl main(int argc, char **argv)
{
	WSADATA          wsd;
	SOCKET_OBJ      *sockobj = NULL,
		*sptr = NULL,
		*tmp = NULL;
	HANDLE           hThread;
	ULONG            lastprint = 0;
	MSG              msg;
	int              rc;
	struct addrinfo *res = NULL,
		*ptr = NULL;

	ValidateArgs(argc, argv);

	InitializeCriticalSection(&gSocketCritSec);

	// Load winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		fprintf(stderr, "unable to load Winsock!\n");
		return -1;
	}

	gWorkerWindow = MakeWorkerWindow();

	printf("Local address: %s; Port: %s; Family: %d\n",
		gBindAddr, gBindPort, gAddressFamily);
	gBindAddr = 0;
	res = ResolveAddress(gBindAddr, gBindPort, gAddressFamily, gSocketType, gProtocol);
	if (res == NULL)
	{
		fprintf(stderr, "ResolveAddress failed to return any addresses!\n");
		return -1;
	}

	// For each local address returned, create a listening/receiving socket
	ptr = res;
	while (ptr)
	{
		PrintAddress(ptr->ai_addr, ptr->ai_addrlen); printf("\n");

		sockobj = GetSocketObj(INVALID_SOCKET);

		// create the socket
		sockobj->s = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (sockobj->s == INVALID_SOCKET)
		{
			fprintf(stderr, "socket failed: %d\n", WSAGetLastError());
			return -1;
		}


		InsertSocketObj(sockobj);

		// bind the socket to a local address and port
		rc = bind(sockobj->s, ptr->ai_addr, ptr->ai_addrlen);
		if (rc == SOCKET_ERROR)
		{
			fprintf(stderr, "bind failed: %d\n", WSAGetLastError());
			return -1;
		}

		if (gProtocol == IPPROTO_TCP)
		{
			rc = listen(sockobj->s, 200);
			if (rc == SOCKET_ERROR)
			{
				fprintf(stderr, "listen failed: %d\n", WSAGetLastError());
				return -1;
			}

			// Register for notification
			rc = WSAAsyncSelect(
				sockobj->s,
				gWorkerWindow,
				WM_SOCKET,
				FD_ACCEPT | FD_CLOSE
				);
			if (rc == SOCKET_ERROR)
			{
				fprintf(stderr, "WSAAsyncSelect failed: %d\n", WSAGetLastError());
				return -1;
			}
		}
		else
		{
			// Register for notification
			rc = WSAAsyncSelect(
				sockobj->s,
				gWorkerWindow,
				WM_SOCKET,
				FD_READ | FD_WRITE | FD_CLOSE
				);
			if (rc == SOCKET_ERROR)
			{
				fprintf(stderr, "WSAAsyncSelect failed: %d\n", WSAGetLastError());
				return -1;
			}
		}

		ptr = ptr->ai_next;
	}
	// free the addrinfo structure for the 'bind' address
	freeaddrinfo(res);

	gStartTime = gStartTimeLast = lastprint = GetTickCount();

	// Start a thread to print statistics
	hThread = CreateThread(NULL, 0, StatisticsThread, NULL, 0, NULL);
	if (hThread == NULL)
	{
		fprintf(stderr, "CreateThread failed: %d\n", WSAGetLastError());
		return -1;
	}
	CloseHandle(hThread);

	while (rc = GetMessage(&msg, NULL, 0, 0))
	{
		if (rc == -1)
		{
			fprintf(stderr, "GetMessage() failed with error %d\n", GetLastError());
			return -1;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	WSACleanup();

	DeleteCriticalSection(&gSocketCritSec);

	return 0;
}
