//// https://docs.microsoft.com/ko-kr/windows/desktop/FileIO/testing-for-the-end-of-a-file
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define BUF_SIZE (61)

LPCTSTR ErrorMessage(DWORD error)
{

	LPVOID lpMsgBuf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		error,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	return((LPCTSTR)lpMsgBuf);
}

void GoDoSomethingElse(void)
{
	printf("Inside GoDoSomethingElse()\n");
}

DWORD AsyncTestForEnd(HANDLE hEvent, HANDLE hFile)
{
	char inBuffer[BUF_SIZE];
	DWORD nBytesToRead = BUF_SIZE;
	DWORD dwBytesRead = 0;
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	OVERLAPPED stOverlapped = { 0 };

	DWORD dwError = 0;
	LPCTSTR errMsg = NULL;

	BOOL bResult = FALSE;
	BOOL bContinue = TRUE;

	stOverlapped.hEvent = hEvent;

	while (bContinue)
	{
		bContinue = FALSE;

		bResult = ReadFile(hFile,
			inBuffer,
			nBytesToRead,
			&dwBytesRead,
			&stOverlapped);

		dwError = GetLastError();

		if (!bResult)
		{
			switch (dwError)
			{

			case ERROR_HANDLE_EOF:
			{
				printf("\nReadFile returned FALSE and EOF condition, async EOF not triggered.\n");
				break;
			}
			case ERROR_IO_PENDING:
			{
				BOOL bPending = TRUE;

				// I/O�� �Ϸ� �ɶ����� �ݺ��ϸ� �Ϸ�Ǹ� overlapped.event�� signal �ȴ�.
				while (bPending)
				{
					bPending = FALSE;

					printf("\nReadFile operation is pending\n");

					GoDoSomethingElse();
					//////////////  ERROR_HANDLE_EOF //////////////////
					// �񵿱� Read�ÿ��� GetOverlappedResult=FALSE�̸� ERROR_HANDLE_EOF�� 
					// ��ȯ�Ѵ�.
					// ����� ���� Read=TRUE�� ��ȯ�ϸ�  pNumberOfBytesRead=0�� �ȴ�.
					bResult = GetOverlappedResult(hFile,
						&stOverlapped,
						&dwBytesRead,
						FALSE);

					if (!bResult)
					{
						switch (dwError = GetLastError())
						{
						case ERROR_HANDLE_EOF:
						{
							printf("GetOverlappedResult found EOF\n");
							break;
						}

						case ERROR_IO_INCOMPLETE:
						{
							printf("GetOverlappedResult I/O Incomplete\n");
							bPending = TRUE;
							bContinue = TRUE;
							break;
						}

						default:
						{
							errMsg = ErrorMessage(dwError);
							_tprintf(TEXT("GetOverlappedResult failed (%d): %s\n"),
								dwError, errMsg);
							LocalFree((LPVOID)errMsg);
						}
						}
					}
					else
					{
						printf("ReadFile operation completed %d->%d\n", stOverlapped.Offset, 
							dwBytesRead);
						ResetEvent(stOverlapped.hEvent);
					}
				}
				break;
			}

			default:
			{
				errMsg = ErrorMessage(dwError);
				printf("ReadFile GLE unhandled (%ld): %Ls\n", dwError, errMsg);
				LocalFree((LPVOID)errMsg);
				break;
			}
			}
		}
		else
		{
			printf("ReadFile completed synchronously\n");
		}
		stOverlapped.Offset += dwBytesRead;
		if (stOverlapped.Offset < dwFileSize)
			bContinue = TRUE;
	}

	return stOverlapped.Offset;
}

void __cdecl _tmain(int argc, TCHAR *argv[])
{
	HANDLE hEvent;
	HANDLE hFile;
	DWORD dwReturnValue;

	hFile = CreateFile(L"bigdata.7z",GENERIC_READ,FILE_SHARE_READ, 
						NULL,OPEN_EXISTING,	FILE_FLAG_OVERLAPPED,NULL);                  

	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = GetLastError();
		LPCTSTR errMsg = ErrorMessage(dwError);
		printf("Could not open file (%ld): %Ls\n", dwError, errMsg);
		LocalFree((LPVOID)errMsg);
		return;
	}

	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (hEvent == NULL)
	{
		DWORD dwError = GetLastError();
		LPCTSTR errMsg = ErrorMessage(dwError);
		printf("Could not CreateEvent: %ld %Ls\n", dwError, errMsg);
		LocalFree((LPVOID)errMsg);
		return;
	}

	dwReturnValue = AsyncTestForEnd(hEvent, hFile);

	printf("\nRead complete. Bytes read: %ld\n", dwReturnValue);

	CloseHandle(hFile);
	CloseHandle(hEvent);
}