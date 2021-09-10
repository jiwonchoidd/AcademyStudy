// https://docs.microsoft.com/ko-kr/windows/desktop/FileIO/testing-for-the-end-of-a-file
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <assert.h>
// 중요
OVERLAPPED readOV;
OVERLAPPED writeOV;
char*   g_pOffsetData = nullptr;
char*   g_pDataBuffer = nullptr;
LARGE_INTEGER FileSize;
LARGE_INTEGER g_LargeRead;
const DWORD   g_dwMaxReadSize = 4096 * 1024;
DWORD FileRead(HANDLE hHandle, DWORD offset)
{
	g_LargeRead.QuadPart += offset;
	readOV.Offset = g_LargeRead.LowPart;
	readOV.OffsetHigh = g_LargeRead.HighPart;
	g_pOffsetData = &g_pDataBuffer[g_LargeRead.QuadPart];

	DWORD dwReadByte = 0;
	BOOL bResult = ReadFile(hHandle,
		g_pOffsetData, g_dwMaxReadSize,
		&dwReadByte, &readOV);

	if (bResult == FALSE)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			//std::cout << "읽기 중!" << std::endl;
			//bPending = TRUE;
		}
	};
	if (bResult == TRUE)
	{
		std::cout << " 로딩완료" << std::endl;
	}
	return 1;
}
DWORD FileWrite(HANDLE hHandle, DWORD offset)
{
	g_LargeRead.QuadPart += offset;
	writeOV.Offset = g_LargeRead.LowPart;
	writeOV.OffsetHigh = g_LargeRead.HighPart;
	g_pOffsetData = &g_pDataBuffer[g_LargeRead.QuadPart];

	DWORD dwOffset = 0;
	if ((FileSize.QuadPart - g_LargeRead.QuadPart) < g_dwMaxReadSize)
	{
		dwOffset = g_dwMaxReadSize - (FileSize.LowPart - g_LargeRead.LowPart);
	}

	DWORD dwWriteByte = 0;
	BOOL bResult = WriteFile(hHandle,
		g_pOffsetData, g_dwMaxReadSize - dwOffset,
		&dwWriteByte, &writeOV);
	return bResult;
}
DWORD AsyncLoad(const TCHAR* filename)
{
	DWORD dwTrans = 0;
	HANDLE hFile = CreateFile(filename,
		GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);


	ZeroMemory(&readOV, sizeof(OVERLAPPED));

	if (hFile != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hFile, &FileSize);
		g_pDataBuffer = new char[FileSize.LowPart];
		DWORD dwCnt = FileSize.LowPart / 4096;
		FileRead(hFile, 0);

		BOOL bPending = TRUE;
		BOOL bResult = FALSE;

		while (bPending)
		{
			bResult = WaitForSingleObject(hFile, 0);
			if (bResult == WAIT_OBJECT_0)
			{
				bResult = GetOverlappedResult(hFile, &readOV, &dwTrans, TRUE);
				if (bResult == TRUE)
				{
					if (g_LargeRead.QuadPart + dwTrans < FileSize.LowPart)
					{
						std::cout << ".";
						FileRead(hFile, dwTrans);
					}
					else
					{
						std::cout << " Loading Finish!" << std::endl;
						bPending = FALSE;
					}
				}
			}
			else if (bResult != WAIT_TIMEOUT)
			{
				assert(0);
			}
			//std::cout << "_";
		}
	}
	CloseHandle(hFile);
	return FileSize.LowPart;
}
DWORD AsyncWrite(const TCHAR* filename)
{
	g_LargeRead.QuadPart = 0;
	DWORD dwTrans = 0;
	HANDLE hFile = CreateFile(filename,
		GENERIC_READ | GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);
	ZeroMemory(&writeOV, sizeof(OVERLAPPED));

	if (hFile != INVALID_HANDLE_VALUE)
	{
		FileWrite(hFile, 0);

		BOOL bPending = TRUE;
		BOOL bResult = FALSE;

		while (bPending)
		{
			bResult = WaitForSingleObject(hFile, 0);
			if (bResult == WAIT_OBJECT_0)
			{
				bResult = GetOverlappedResult(hFile, &writeOV, &dwTrans, TRUE);
				if (bResult == TRUE)
				{
					if (g_LargeRead.QuadPart + dwTrans < FileSize.LowPart)
					{
						std::cout << ".";
						FileWrite(hFile, dwTrans);
					}
					else
					{
						std::cout << "Write Finish!" << std::endl;
						bPending = FALSE;
					}
				}
			}
			else if (bResult != WAIT_TIMEOUT)
			{
				assert(0);
			}
			//std::cout << ".";
		}
	}
	CloseHandle(hFile);
	return FileSize.LowPart;
}
void main()
{
	std::wstring readname = L"Demo";
	std::wstring writename = L"Copy";
	std::wstring ext = L".iso";
	readname += ext;
	writename += ext;
	DWORD dwFileSize = AsyncLoad(readname.c_str());
	AsyncWrite(writename.c_str());
}
