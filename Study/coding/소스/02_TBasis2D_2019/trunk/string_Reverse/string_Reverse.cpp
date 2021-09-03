// string_Reverse.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include <windows.h>
#include <string>
using namespace std;
typedef basic_string<TCHAR> T_STR;
typedef basic_string<wchar_t> W_STG;
typedef basic_string<char>  C_STR;
// ��Ƽ����Ʈ �������� �����
// ABCȫ�浿 = 9
// [A][B][C] [0]
// �����ڵ����Ʈ �������� �����
// ABCȫ�浿 = 12
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
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = WideCharToMultiByte(CP_ACP, 0,
		data, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0,
		data, -1,  //  �ҽ�
		retData, iLength, // ���
		NULL, NULL);
	return retData;
}
static bool GetWtM(WCHAR* src, char* pDest)
{
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = WideCharToMultiByte(CP_ACP, 0,
		src, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0,
		src, -1,  //  �ҽ�
		pDest, iLength, // ���
		NULL, NULL);
	if (iRet == 0) return false;
	return true;
}
static WCHAR* GetMtW(char* data)
{
	WCHAR retData[4096] = { 0 };
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = MultiByteToWideChar(CP_ACP, 0,data, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0,
		data, -1,  //  �ҽ�
		retData, iLength); // ���
	return retData;
}
static bool GetMtW(char* pSrc, WCHAR* pDest)
{
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = MultiByteToWideChar(CP_ACP, 0,
		pSrc, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0,
		pSrc, -1,  //  �ҽ�
		pDest, iLength); // ���		
	if (iRet == 0) return false;
	return true;
}
static void PRINT(char* fmt, ...) // ����������
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
	/////////////////////////// C��� /////////////
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

