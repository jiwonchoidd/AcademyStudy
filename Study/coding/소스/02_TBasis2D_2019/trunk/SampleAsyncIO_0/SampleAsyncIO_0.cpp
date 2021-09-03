// SampleFile_0.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <assert.h>
TCHAR* pFileBuffer = 0;

// 중요
OVERLAPPED readOV;
OVERLAPPED whiteOV;


DWORD Write(const TCHAR* filename)
{
	HANDLE hWriteFile = CreateFile(filename, GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
		NULL);
	DWORD dwWritten = 0;
	if (hWriteFile != NULL)
	{
		TCHAR buf[] = L"kkkkkk";

		DWORD dwLength = sizeof(buf);
		BOOL ret = WriteFile(hWriteFile, buf, dwLength, &dwWritten, NULL);
		if (ret == TRUE)
		{
			std::cout << "출력완료" << std::endl;
		}
	}
	CloseHandle(hWriteFile);
	return dwWritten;
}
DWORD Read(const TCHAR* filename)
{
	HANDLE hReadFile = CreateFile(L"test.txt",
		GENERIC_READ, 0,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD dwRead = 0;
	if (hReadFile != NULL)
	{
		TCHAR buf[MAX_PATH] = { 0, };

		DWORD dwLength = sizeof(buf);
		BOOL ret = ReadFile(hReadFile, buf, dwLength,
			&dwRead, NULL);
		if (ret == TRUE)
		{
			std::wcout << buf << std::endl;
		}
	}
	CloseHandle(hReadFile);
	return dwRead;
}

DWORD Load(const TCHAR* filename)
{
	HANDLE hReadFile = CreateFile(filename,
		GENERIC_READ, 0,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD dwRead = 0;
	LARGE_INTEGER FileSize;
	if (hReadFile != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hReadFile, &FileSize);
		pFileBuffer = new TCHAR[FileSize.LowPart];
		DWORD dwLength = FileSize.LowPart;
		BOOL ret = ReadFile(hReadFile, pFileBuffer, dwLength,
			&dwRead, NULL);
		if (ret == TRUE)
		{
			INT KKK = 0;
		}
	}
	CloseHandle(hReadFile);
	return dwRead;
}
DWORD Copy(const TCHAR* filename, DWORD dwLength)
{
	HANDLE hWriteFile = CreateFile(filename, GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
		NULL);
	DWORD dwWritten = 0;
	if (hWriteFile != NULL)
	{
		//DWORD dwLength = _msize(pFileBuffer);
		BOOL ret = WriteFile(hWriteFile, pFileBuffer,
			dwLength, &dwWritten, NULL);
		if (ret == TRUE)
		{
			std::cout << "출력완료" << std::endl;
		}
	}
	CloseHandle(hWriteFile);
	return dwWritten;
}

DWORD AsyncLoad(const TCHAR* filename)
{
	DWORD dwTrans = 0;
	HANDLE hReadFile = CreateFile(filename,
		GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	LARGE_INTEGER FileSize;
	ZeroMemory(&readOV, sizeof(OVERLAPPED));

	if (hReadFile != INVALID_HANDLE_VALUE)
	{
		GetFileSizeEx(hReadFile, &FileSize);
		pFileBuffer = new TCHAR[FileSize.LowPart];
		DWORD dwLength = FileSize.LowPart;
		BOOL bPending = FALSE;
		BOOL bResult = ReadFile(hReadFile,
			pFileBuffer, dwLength,
			&dwTrans, &readOV);

		if (bResult == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				std::cout << "읽기 중!" << std::endl;
				bPending = TRUE;
			}
		};
		if (bResult == TRUE)
		{
			std::cout << " 로딩완료" << std::endl;
		}

		while (bPending)
		{
			bResult = GetOverlappedResult(hReadFile, &readOV, &dwTrans, FALSE);
			if (bResult == TRUE)
			{
				assert(dwTrans >= dwLength);
				std::cout << " Loading Finish!" << std::endl;
				bPending = FALSE;
			}
			else
			{
				if (GetLastError() == ERROR_IO_INCOMPLETE)
				{
					std::cout << " Loading..." << "ERROR_IO_INCOMPLETE(996)" << std::endl;
				}
				else
				{
					std::cout << " Loading Error!" << GetLastError() << std::endl;
					break;
				}
			}
			Sleep(1000);
		}
	}
	CloseHandle(hReadFile);
	return dwTrans;
}
DWORD AsyncCopy(const TCHAR* filename, DWORD dwLength)
{
	BOOL bPending = FALSE;
	DWORD dwTrans = 0;

	HANDLE hWriteFile = CreateFile(filename,
		GENERIC_READ | GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS,
		//FILE_FLAG_WRITE_THROUGH 반드시 추가되어야 함.
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED | FILE_FLAG_WRITE_THROUGH,
		NULL);

	ZeroMemory(&whiteOV, sizeof(OVERLAPPED));
	//whiteOV.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (hWriteFile != NULL)
	{
		BOOL bResult = WriteFile(hWriteFile, pFileBuffer, dwLength, &dwTrans, &whiteOV);
		if (bResult == FALSE)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				std::cout << "쓰기 중!" << std::endl;
				bPending = TRUE;
			}
		}
		if (bResult == TRUE)
		{
			std::cout << " 쓰기완료" << std::endl;
		}
		while (bPending)
		{
			bResult = GetOverlappedResult(hWriteFile, &whiteOV, &dwTrans, FALSE);
			if (bResult == TRUE)
			{
				assert(dwTrans >= dwLength);
				std::cout << " Write Finish!" << std::endl;
				bPending = FALSE;
			}
			else
			{
				if (GetLastError() == ERROR_IO_INCOMPLETE)
				{
					std::cout << " Writting..." << "GetOverlappedResult I/O Incomplete(996)" << std::endl;
				}
				else
				{
					std::cout << " Writting Error!" << GetLastError() << std::endl;
					break;
				}
			}
			Sleep(1000);
		}
	}

	delete[] pFileBuffer;
	CloseHandle(hWriteFile);
	return dwTrans;
}
void main()
{
	//Write(L"test.txt");
	//Read(L"test.txt");	
	//DWORD dwFileSize = Load(L"DEMO.ISO");
	//Copy(L"Copy.ISO", dwFileSize);

	std::wstring readname = L"Demo";
	std::wstring writename = L"Copy";
	std::wstring ext = L".iso";
	readname += ext;
	writename += ext;
	DWORD dwFileSize = AsyncLoad(readname.c_str());
	AsyncCopy(writename.c_str(), dwFileSize);
}