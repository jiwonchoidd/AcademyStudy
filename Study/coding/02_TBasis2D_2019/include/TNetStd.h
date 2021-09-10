#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h> // windows.h 보다 우선 연결한다.
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <string>
#include <vector>
#include <list>
#include <atlconv.h> // A2W
#include "TProtocol.h"

#pragma comment (lib, "ws2_32.lib")
#if NDEBUG
#pragma comment (lib, "TNetLib_R.lib")
#else
#pragma comment (lib, "TNetLib_D.lib")
#endif

namespace TBASIS_CLIENT_LIB
{
	typedef std::basic_string<TCHAR>		T_STR;
	typedef std::basic_string<CHAR>			C_STR;
	typedef std::vector<T_STR>				T_STR_VECTOR;
	static std::wstring to_mw(const std::string& _src)
	{
		USES_CONVERSION;
		return std::wstring(A2W(_src.c_str()));
	};

	static std::string to_wm(const std::wstring& _src)
	{
		USES_CONVERSION;
		return std::string(W2A(_src.c_str()));
	};
	static char* GetWtM(WCHAR* data)
	{
		char retData[4096] = { 0 };
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = WideCharToMultiByte(CP_ACP, 0,
			data, -1, 0, 0, NULL, NULL);
		int iRet = WideCharToMultiByte(CP_ACP, 0,
			data, -1,  //  소스
			retData, iLength, // 대상
			NULL, NULL);
		return retData;
	}
	static bool GetWtM(WCHAR* src, char* pDest)
	{
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = WideCharToMultiByte(CP_ACP, 0,
			src, -1, 0, 0, NULL, NULL);
		int iRet = WideCharToMultiByte(CP_ACP, 0,
			src, -1,  //  소스
			pDest, iLength, // 대상
			NULL, NULL);
		if (iRet == 0) return false;
		return true;
	}
	static WCHAR* GetMtW(char* data)
	{
		WCHAR retData[4096] = { 0 };
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
		int iRet = MultiByteToWideChar(CP_ACP, 0,
			data, -1,  //  소스
			retData, iLength); // 대상
		return retData;
	}
	static bool GetMtW(char* pSrc, WCHAR* pDest)
	{
		// 변형되는 문자열의 크기가 반환된다.
		int iLength = MultiByteToWideChar(CP_ACP, 0,
			pSrc, -1, 0, 0);
		int iRet = MultiByteToWideChar(CP_ACP, 0,
			pSrc, -1,  //  소스
			pDest, iLength); // 대상		
		if (iRet == 0) return false;
		return true;
	}
	static void PRINT(char* fmt, ...) // 나열연산자
	{
		va_list arg;
		va_start(arg, fmt);
		char buf[256] = { 0, };
		vsprintf_s(buf, fmt, arg);
		printf("\n=====> %s", buf);
		va_end(arg);
	}
	static void Error(const CHAR* msg = 0, const char* lpData = 0)
	{
		LPVOID* lpMsg = 0;
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(CHAR*)&lpMsg, 0, NULL);

		std::string szBuffer = (lpData != nullptr) ? lpData : "";
		szBuffer += "\n";
		szBuffer += (CHAR*)lpMsg;

		MessageBoxA(NULL, szBuffer.c_str(), msg, MB_ICONERROR);
		LocalFree(lpMsg);
	}
	static void Check(int iRet, int line)
	{
		if (iRet == SOCKET_ERROR)
		{
			CHAR	szBuffer[256] = { 0, };
			sprintf_s(szBuffer,
				"%s[%d]",
				__FILE__,
				line);
			Error("ERROR", szBuffer);
			exit(1);
		}
	}
}