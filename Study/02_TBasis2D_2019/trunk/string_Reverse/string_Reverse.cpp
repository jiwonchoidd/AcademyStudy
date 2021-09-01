// string_Reverse.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <windows.h>
#include <string>
using namespace std;
typedef basic_string<TCHAR> T_STR;
typedef basic_string<wchar_t> W_STG;
typedef basic_string<char>  C_STR;
// 멀티바이트 문자집합 사용경우
// ABC홍길동 = 9
// [A][B][C] [0]
// 유니코드바이트 문자집합 사용경우
// ABC홍길동 = 12
// [A][0][B][0][C][0] [][][][][][]
static std::wstring mtw( std::string str)
{
	std::wstring ret = std::wstring(str.begin(), str.end());
	return  ret;
}
static std::string wtm(std::wstring str)
{
	return  std::string(str.begin(), str.end());
}
static char* GetWtM( WCHAR* data)
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
	int iLength = MultiByteToWideChar(CP_ACP, 0,data, -1, 0, 0);
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
int main()
{
	//////////// stl //////////////// 
	//C_STG  name5 = "KGCA";
	//C_STG  name7;
	//W_STG  name6;
	//name6 = mtw(name5);
	//_tprintf(_T("%s"), name6.c_str());
	//name7 = wtm(name6);
	//printf("%s", name7.c_str());
	////////////// API ///////////////
	char name0[256] = { 0, };
	//char   name1[] = "game";
	TCHAR name2[256] = L"game";
	TCHAR name3[256] = { 0 };
	if (GetWtM(name2, name0))
	{
		printf("%s", name0);
	}
	if (GetMtW(name0, name3))
	{
		_tprintf(L"%s", name3);
	}
	/////////////////////////// C언어 /////////////
	////name2 = name1;
	//mbstowcs(name2, name1, strlen(name1));
	//wcstombs(name0, name2, _tcslen(name2));
	//W_STG  name6 = name2;
	//_tprintf(_T("%s"), name2 );
	//printf("\n%s", name0);
	//name0 = name2;

	PRINT("%s : %d : %f", "data", 10, 3.14f);
    return 0;
}

