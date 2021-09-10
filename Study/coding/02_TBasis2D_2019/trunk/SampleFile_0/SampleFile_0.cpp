// SampleFile_0.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <windows.h>

char* GetWtM(WCHAR* data)
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

int main()
{
	HANDLE hWriteFile = CreateFile(L"test.txt", GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hWriteFile != NULL)
	{
		TCHAR buf[] = L"KGCA";
		DWORD dwWritten;
		DWORD dwLength = sizeof(buf);// wcslen(buf) * 2;
		BOOL ret = WriteFile(hWriteFile, buf, dwLength,	&dwWritten, NULL);
		if (ret == TRUE)
		{
			printf("출력 완료!");
		}
	}
	CloseHandle(hWriteFile);

	HANDLE hReadFile = CreateFile(L"test.txt", GENERIC_READ, 0, NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hReadFile != NULL)
	{
		TCHAR buf[1024];
		DWORD dwRead;
		DWORD dwLength = sizeof(buf);// wcslen(buf) * 2;
		BOOL ret = ReadFile(hReadFile, buf, dwLength, &dwRead, NULL);
		if (ret == TRUE)
		{
			_tprintf(L"\n읽기 완료! : %s", buf);
			_tcprintf(L"\n읽기 완료! : %s", buf);
			printf("\n--->%s", GetWtM(buf));
		}
	}
	CloseHandle(hReadFile);
    return 0;
}

