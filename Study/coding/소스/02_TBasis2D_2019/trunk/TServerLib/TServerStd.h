#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

#pragma comment(lib, "ws2_32.lib")


#define PACK_RECV 1000
#define PACK_SEND 2000
#define PACK_END  3000
#define MAX_STREAM_SIZE 4096*5

template <class T> class TSingleton
{
public:
	static T& GetInstance()
	{
		static T theSingle;
		return theSingle;
	}
};

static void E_MSG(const char* pMsg)
{
	void* lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	if (lpMsgBuf != nullptr)
	{
		std::string msg = (char*)lpMsgBuf;
		msg += std::to_string(WSAGetLastError());
		MessageBoxA(NULL, msg.c_str(), pMsg, MB_OK);
		LocalFree(lpMsgBuf);
	}
	return;
}