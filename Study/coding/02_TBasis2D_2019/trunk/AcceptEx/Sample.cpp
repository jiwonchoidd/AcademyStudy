#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <mswsock.h>
#include <string>
#include <list>
#include <process.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")

using namespace std;

enum State { stateClosed, stateAccept, stateReading, stateWriting };

#define BUFFER_SIZE 1024



// Overlap 구조체 정의
struct stOvelap : public OVERLAPPED
{
	static const int addrlen;
	static const int initialReceiveSize;

	State state; //접속상태 체크
	SOCKET s; // 클라이언트 소켓
	DWORD n; // recv send 전송 바이트 
	char *end;
	bool ignore;
	sockaddr_in local, peer;

	string SendString;
	WSABUF sendBufferDescriptor;
	char buf[BUFFER_SIZE]; // receive buffer
	WSABUF recvBufferDescriptor;
	DWORD recvFlags;

	stOvelap() : state(stateClosed), ignore(false), s(SOCKET_ERROR), end(&buf[0])
	{
		Internal = InternalHigh = Offset = OffsetHigh = 0; hEvent = 0;
	}

	stOvelap(const stOvelap &o)
	{
		state = o.state;
		s = o.s;
		n = o.n;
		end = &buf[0] + (o.end - &o.buf[0]);
		ignore = o.ignore;
		local = o.local;
		peer = o.peer;
		memcpy(buf, o.buf, BUFFER_SIZE);
	}

	~stOvelap() { }
};



typedef list< stOvelap * > OL;
typedef OL::iterator OLI;
const int stOvelap::addrlen = sizeof sockaddr_in + 16;
const int stOvelap::initialReceiveSize = 0;

// 전체 overlapped 구조체 리스트
OL g_OverlappedList;

// Globals
HANDLE g_hIOCP;
const int MAXSOCKETS = 5000;
SOCKET g_listener = INVALID_SOCKET;
int concurrency = 0;

void DoIo(stOvelap &stOvelap);
void CloseSocket(stOvelap &stOvelap, bool force = false, bool listenAgain = true);
void InitOverlap(stOvelap &stOvelap);
void PostRead(stOvelap &stOvelap);
bool DoCommands(stOvelap &stOvelap);
void PostSend(stOvelap &stOvelap);


// I/O packet을 처리하는 WorkerThread
unsigned int __stdcall WorkerThread(void *arg)
{
	DWORD n;
	ULONG_PTR key;
	BOOL result;
	stOvelap *pOverlap;

	while (1)
	{
		result = GetQueuedCompletionStatus(g_hIOCP, &n, &key, (OVERLAPPED **)&pOverlap, INFINITE);

		if (result == FALSE || pOverlap == NULL) {
			if (pOverlap != NULL) {
				CloseSocket(*pOverlap, true, g_listener == INVALID_SOCKET ? false : true);
			}
		}
		else {
			pOverlap->n = n;
			DoIo(*pOverlap);
		}
	}
	return 0;
}


void CloseSocket(stOvelap &ov, bool force, bool listenAgain)
{
	struct linger li = { 0, 0 }; // default: SO_DONTLINGER

	if (force)
		li.l_onoff = 1; // SO_LINGER, timeout = 0

	setsockopt(ov.s, SOL_SOCKET, SO_LINGER, (char *)&li, sizeof li);
	closesocket(ov.s);

	if (listenAgain)
		InitOverlap(ov);
}


// Overlap 구조체를 초기화 합니다
void InitOverlap(stOvelap &ov)
{
	int zero = 0;

	ov.s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ov.s != INVALID_SOCKET)
	{
		ov.state = stateAccept;
		ov.ignore = false;
		ov.end = &ov.buf[0];

		setsockopt(ov.s, SOL_SOCKET, SO_SNDBUF, (char *)&zero, sizeof zero);
		setsockopt(ov.s, SOL_SOCKET, SO_RCVBUF, (char *)&zero, sizeof zero);

		memset(ov.buf, '\0', sizeof ov.buf);

		AcceptEx(g_listener, ov.s, &ov.buf[0], stOvelap::initialReceiveSize, ov.addrlen, ov.addrlen, &ov.n, &ov);
	}
	else {
		// Error condition
	}
}



// I/O 처리
void DoIo(stOvelap &ov)
{
	int locallen, remotelen;
	string tt;
	sockaddr_in *plocal = 0, *premote = 0;

reevaluate:
	switch (ov.state)
	{
	case stateAccept: // Accept 되었다
		// 소켓, 소켓주소를 받아오는 방식이 다릅니다
		GetAcceptExSockaddrs(&ov.buf[0], stOvelap::initialReceiveSize,
			ov.addrlen, ov.addrlen, (sockaddr **)&plocal, &locallen,
			(sockaddr **)&premote, &remotelen);

		memcpy(&ov.local, plocal, sizeof sockaddr_in);
		memcpy(&ov.peer, premote, sizeof sockaddr_in);

		setsockopt(ov.s, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
			(char *)&g_listener, sizeof g_listener);

		CreateIoCompletionPort((HANDLE)ov.s, g_hIOCP, 0, concurrency);
		tt = inet_ntoa(ov.peer.sin_addr);

		if (ov.n != 0) { // 접속과 동시에 무언가 받은게 있다면 처리한다
			ov.state = stateReading; // fake a completed read

			goto reevaluate;
		}

		PostRead(ov);
		break;

	case stateReading:
		if (ov.n == SOCKET_ERROR) {
			CloseSocket(ov, true);
		}
		else if (ov.n == 0) {
			CloseSocket(ov, false);
		}
		else {
			ov.end += ov.n;
			if (!DoCommands(ov)) {
				PostRead(ov);
			}
			else {

			}
		}
		break;

	case stateWriting:
		if (ov.n == SOCKET_ERROR) {
			CloseSocket(ov, true);
		}
		else if (ov.n == 0) {
			CloseSocket(ov, false);
		}
		else {
			PostRead(ov);
		}
		break;
	}
}


// 해당 소켓을 다시 stateWriting으로 복귀시킵니다
void PostSend(stOvelap &ov)
{
	BOOL result;
	DWORD err;

	ov.state = stateWriting;
	ov.n = ov.SendString.size();
	ov.sendBufferDescriptor.len = ov.n;
	ov.sendBufferDescriptor.buf = const_cast<char *>(ov.SendString.c_str());
	result = WSASend(ov.s, &ov.sendBufferDescriptor, /* number of WSABUFs */ 1,
		&ov.n, /* flags */ 0, &ov, /* completion routine */ 0);

	result = (result != SOCKET_ERROR); // WSASend() uses inverted logic wrt/WriteFile()
	err = WSAGetLastError();

	if (!result) {
		if (err != ERROR_IO_PENDING) {
			CloseSocket(ov, true);
		}
		else {

		}
	}
	else {

	}
}


// 해당 소켓을 다시 read상태로 복귀시킵니다
void PostRead(stOvelap &ov)
{
	BOOL result;

	if (ov.end == &ov.buf[BUFFER_SIZE]) {
		ov.end = &ov.buf[0];
		ov.ignore = true;
	}

	ov.state = stateReading; // 읽기 상태임을 명시한다
	ov.n = &ov.buf[BUFFER_SIZE] - ov.end;
	ov.recvBufferDescriptor.len = ov.n;
	ov.recvBufferDescriptor.buf = ov.end;
	ov.recvFlags = 0;
	result = WSARecv(ov.s, &ov.recvBufferDescriptor, 1,
		&ov.n, &ov.recvFlags, &ov, 0);

	result = (result != SOCKET_ERROR);
	if (result) {
		return;
	}
	else {
		if (GetLastError() != ERROR_IO_PENDING) {
			CloseSocket(ov, true);
		}
		else {

		}
	}
	return;
}


// 받은 데이터를 처리합니다 - 데이터는 ov안에 들어있습니다
bool DoCommands(stOvelap &ov)
{
	// 받은 데이터를 콘솔화면에 뿌려준다
	char buff[1024];
	memcpy(buff, ov.buf, ov.n);
	buff[ov.n] = '\0';
	printf(ov.buf);
	memset(ov.buf, '\0', sizeof ov.buf);
	ov.n = 0;
	ov.end = ov.buf;

	// 받은 데이터를 모두에게 재전송한다    

	OLI ovi; // iterator into that list
	for (ovi = g_OverlappedList.begin(); ovi != g_OverlappedList.end(); ++ovi)
	{
		if ((*ovi)->state != stateAccept) {
			(*ovi)->SendString = buff;
			PostSend(*(*ovi));
		}
	}
	return true;
}


int main(int argc, char *argv[])
{

	int maxthreads = MAXIMUM_WAIT_OBJECTS, port = 10000, maxsockets = 300;
	int zero = 0;
	OL ovl; // linked list of stOvelapERLAPPED derivates
	OLI ovi; // iterator into that list
	HANDLE h; // temporary thread handle
	int i;
	unsigned int dummy;

	// IOCP port 생성
	g_hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, concurrency);

	// threads 생성
	for (i = 0; i < maxthreads; ++i) {
		h = (HANDLE)_beginthreadex(0, 0, WorkerThread, g_hIOCP, 0, &dummy);
	}

	// winsock 초기화
	WSADATA wd = { 0 };
	WSAStartup(MAKEWORD(2, 0), &wd);

	// listen socket 생성
	SOCKADDR_IN addr;
	g_listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons((short)port);
	bind(g_listener, (SOCKADDR *)&addr, sizeof addr);
	listen(g_listener, 200);

	// create accept sockets and stOvelap structs
	g_OverlappedList.clear();

	for (i = 0; i < maxsockets; ++i)
	{
		stOvelap *pOverlap = new stOvelap;
		pOverlap->state = stateAccept;
		InitOverlap(*pOverlap);
		g_OverlappedList.push_back(pOverlap);
	}

	// connect g_listener socket to IOCP
	CreateIoCompletionPort((HANDLE)g_listener, g_hIOCP, 0, concurrency);

	while (TRUE) {};

	// 소켓 닫기
	for (ovi = g_OverlappedList.begin(); ovi != g_OverlappedList.end(); ++ovi)
	{
		closesocket((*ovi)->s);
		delete (*ovi);
	}

	CloseHandle(g_hIOCP);
	// shut down winsock
	WSACleanup();

	return 0;
}
