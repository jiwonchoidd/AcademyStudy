#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <string>
#include <list>
#include <vector>
#include <memory>
using namespace std;

#pragma comment(lib, "ws2_32.lib")
static void E_MSG(const char* pMsg)
{
	void* lpMsgBuf;
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(char*)&lpMsgBuf, 0, NULL);
	MessageBoxA(NULL, (char*)lpMsgBuf, pMsg, MB_OK);
	LocalFree(lpMsgBuf);
	return;
}

namespace T_SERVER
{
	typedef basic_string<TCHAR> T_STR;
	typedef basic_string<wchar_t> W_STG;
	typedef basic_string<char>  C_STR;
	typedef vector<basic_string<TCHAR>>		TCHAR_STRING_VECTOR;
	typedef vector<DWORD>					DWORD_VECTOR;
};