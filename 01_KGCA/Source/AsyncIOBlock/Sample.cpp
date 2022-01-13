/*10mb 읽고 10mb 출력, 다시 10mb 읽게해야함
그 작업이 다시 10mb 출력, 
그게 비동기라서 순서가 보장이 되지않는다.
읽는 버퍼가 10번이 돌아가되 offset 0 10 20 30
쓰는 버퍼가 알아서 10개 되면 읽어옴

읽기 여러명 쓰기 여러명 관리 한명

여러명이 구간 구간 나눠서 읽음, 
읽은 버퍼를 연속적으로 파일 출력

블록단위로 한다. 2048 512 배수
비동기 출력 Offset를 왜 사용하는지 아는게 중요
*/
#define BlockSizes 2048
#include <windows.h>
#include <iostream>

void main()
{
	setlocale(LC_ALL, "");
	WCHAR* pwbuffer = NULL;
	LARGE_INTEGER fileSize;
	OVERLAPPED ol_Read = { 0, };
	OVERLAPPED ol_Write = { 0, };

	//핸들 반환 FILE_FLAG_OVERLAPPED으로 비동기 파일 입출력 
	//기존에 있는 파일을 읽어옴
	HANDLE hFile = CreateFile(L"test.JPG", GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	//실패하면 초기값이 잡힘
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return;
	}
	// 핸들 반환 FILE_FLAG_OVERLAPPED으로 비동기 파일 입출력
	// 복사하는 핸들
	HANDLE hFileCopy = CreateFile(L"test_copy.JPG", GENERIC_WRITE, 0, 
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	//실패하면 초기값이 잡힘
	if (hFileCopy == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFileCopy);
		return;
	}

	//읽어온 파일 사이즈를 체크
	GetFileSizeEx(hFile, &fileSize);

	DWORD dwRead;
	DWORD dwWritten;
	DWORD dwTotalWrite = 0;

	//sector의 크기 배수 일반적으로 512
	pwbuffer = new WCHAR[BlockSizes];
	int i = 0;
	while(1)
	{
		//Temp 버퍼
		ZeroMemory(pwbuffer, BlockSizes);
		
		//비동기 로드
		//Overlapped 구조체 오프셋
		ol_Read.Offset = i * BlockSizes;
		ol_Read.OffsetHigh = 0;

		BOOL ret = ReadFile(hFile, pwbuffer, BlockSizes, &dwRead, &ol_Read);
		BOOL bPending = FALSE;
		if (ret == FALSE)
		{
			DWORD dwError = GetLastError();
			if (dwError == ERROR_IO_PENDING)
			{
				std::wcout << L"ReadFile";
			}
			else
			{

			}
		}
		BOOL bReturn = GetOverlappedResult(hFile, &ol_Read, &dwRead, FALSE);
		if (bReturn == TRUE)
		{
			std::wcout << L"로드 완료";
			bPending = FALSE;
		}
		else
		{
			DWORD dwError = GetLastError();
			if (dwError == ERROR_IO_INCOMPLETE)
			{
				std::wcout << L".";
			}
			else
			{
				std::wcout << L"False";
				bPending = FALSE;
			}
		}
		//비동기 출력
		//Overlapped 구조체 오프셋
		ol_Write.Offset = i * BlockSizes;
		ol_Write.OffsetHigh = 0;

		ret = WriteFile(hFileCopy, pwbuffer, dwRead, &dwWritten, &ol_Write);
		if (ret == FALSE)
		{
			DWORD dwError = GetLastError();
			if (dwError == ERROR_IO_PENDING)
			{
				std::wcout << L"출력 중";
			}
			else
			{

			}
		}
		bReturn = GetOverlappedResult(hFileCopy, &ol_Write, &dwWritten, FALSE);
		if (bReturn == TRUE)
		{
			std::wcout << L"로드 완료";
			bPending = FALSE;
		}
		else
		{
			DWORD dwError = GetLastError();
			if (dwError == ERROR_IO_INCOMPLETE)
			{
				std::wcout << L".";
			}
			else
			{
				std::wcout << L"False";
				bPending = FALSE;
			}
		}
		dwTotalWrite += dwWritten;

		if (fileSize.LowPart == dwTotalWrite)
		{
			break;
		}
		i++;
	}
	CloseHandle(hFile);
	CloseHandle(hFileCopy);
}
