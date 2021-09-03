// overlapped_write.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <TCHAR.h>
#include <stdio.h>
#include <assert.h>


int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE			h					= INVALID_HANDLE_VALUE;
	char			buf[1024]			= {0,};
	char			buf2[1024]			= {0,};
	char			buf3[1024]			= {0,};
	int				i					= 0;
	LARGE_INTEGER	nLarge				= {0,};
	char			temppath[MAX_PATH+1]= {0,};
	OVERLAPPED		ov					= {0,};
	DWORD			dwValue				= 0;

	// abc...xy가 반복
	for (i=0; i<1024; i++)
	{
		buf[i] = 'a' + i % ('z' - 'a');
	}

	// ABC...XY가 반복
	for (i=0; i<1024; i++)
	{
		buf2[i] = 'A' + i % ('Z' - 'A');
	}

	// 임시 경로 생성
	//::GetTempPathA(MAX_PATH, temppath);
	//strncat_s(temppath, "\\_test_.txt", MAX_PATH);

	// %temp%_test_.txt 생성
	// FILE_FLAG_NO_BUFFERING이 OR되면 최적의 성능으로 된다고 한다.
	h = ::CreateFileA("test.txt", 
					  GENERIC_READ | GENERIC_WRITE, 
					  0, 
					  NULL, 
					  CREATE_ALWAYS, 
					  FILE_FLAG_OVERLAPPED, // | FILE_FLAG_NO_BUFFERING 가 OR되어 있으면, ReadFile에도 sector 크기배의 버퍼, File Pointer 위치의 제약이 따른다.
					  NULL);
	if (INVALID_HANDLE_VALUE == h)
	{
		assert(FALSE);
		exit(1);
	}

	// i/o completion 발생시 event가 signal 됨
	// auto-reset mode이기 때문에, WaitForSingleObject등을 한번 통과하면,
	// 자동으로 non-signaled 상태로 전환됨
	ov.hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	//////////////////////////////////////////////////////////////////////////
	// Overlapped i/o로 파일 쓰기

	// Overlapped i/o로 쓴다. 즉시 리턴된다.
	// FILE_FLAG_NO_BUFFERING 사용시 sector 크기의 배수 (512 x 2 = 1024)로 Write해야 한다.
	if (FALSE == ::WriteFile(h, buf, 1024, NULL, &ov))
	{
		dwValue = ::GetLastError();
		if (dwValue == ERROR_IO_PENDING)
		{
			// i/o가 완료되지 않았음
			// 정상
		}
		else
		{
			// 에러 상황
			assert(FALSE);
			exit(1);
		}
	}
	else
	{
		// 예측되지 않은 에러 상황
		assert(FALSE);
		exit(1);
	}

	// 이곳에서 CPU 사용되는 코드를 실행하면 된다.
	{

	}

	// I/O가 완료될 때 가지 기다린다.
	::WaitForSingleObject(ov.hEvent, INFINITE);

	// I/O의 결과를 보고받는다.
	if (FALSE == ::GetOverlappedResult(h, &ov, &dwValue, TRUE))
	{
		// 에러 상황
		assert(FALSE);
		exit(1);
	}

	// 다음 File Pointer를 계산한다.
	// LARGE_INTER의 QuadPart를 이용하면 편리하다.
	nLarge.QuadPart += 1024;

	// LARGE_INTEGER 값을 기반으로 File Pointer 위치를 전달한다.
	ov.Offset = nLarge.LowPart;
	ov.OffsetHigh = nLarge.HighPart;

	// Overlapped i/o로 쓴다. 즉시 리턴된다.
	// FILE_FLAG_NO_BUFFERING 사용시 sector 크기의 배수 (512 x 2 = 1024)로 Write해야 한다.
	if (FALSE == ::WriteFile(h, buf2, 1024, NULL, &ov))
	{
		dwValue = ::GetLastError();
		if (dwValue == ERROR_IO_PENDING)
		{
			// i/o가 완료되지 않았음
			// 정상
		}
		else
		{
			// 에러 상황
			assert(FALSE);
			exit(1);
		}
	}
	else
	{
		// 예측되지 않은 에러 상황
		assert(FALSE);
		exit(1);
	}

	// 이곳에서 CPU 사용되는 코드를 실행하면 된다.
	{

	}

	// I/O가 완료될 때 가지 기다린다.
	// 만일, MsgWaitForMultipleObjects(...)를 사용하면,
	// UI의 block을 막을 수 있어 유용한다.
	// 만일 취소를 원한다면, ::CancelIo(...)를 사용할 수 있다.
	::WaitForSingleObject(ov.hEvent, INFINITE);

	// I/O의 결과를 보고받는다.
	if (FALSE == ::GetOverlappedResult(h, &ov, &dwValue, TRUE))
	{
		// 에러 상황
		assert(FALSE);
		exit(1);
	}

	// 여기까지,
	// abc....xyabc...xy... 가 1024 byte
	// ABC....XYABC...XY... 가 1024 byte
	// 총 2048 byte가 저장되었다.
	printf("2048 bytes written.\r\n");

	//////////////////////////////////////////////////////////////////////////
	// Overlapped i/o로 파일 읽기

	// 1byte 위치 부터 읽어본다.
	// 읽기는 WriteFile 처럼, sector 배수의 조건이 필요 없다.
	// 여기서는 File Pointer는 선두 1byte, 읽을 크기는 10byte로 한다.
	// Overlapped i/o로 하기 때문에, 바로 리턴된다.
	// 만일, FILE_FLAG_NO_BUFFERING이 OR되었다면, 아래는 87번 오류(=ERROR_INVALID_PARAMETER)
	// 가 들어온다.
	nLarge.QuadPart = 1;
	ov.Offset = nLarge.LowPart;
	ov.OffsetHigh = nLarge.HighPart;
	if (FALSE == ::ReadFile(h, buf3, 10, NULL, &ov))
	{
		dwValue = ::GetLastError();
		if (dwValue == ERROR_IO_PENDING)
		{
			// i/o가 완료되지 않았음
			// 정상
		}
		else
		{
			// 에러 상황
			assert(FALSE);
			exit(1);
		}
	}
	else
	{
		// 예측되지 않은 에러 상황
		assert(FALSE);
		exit(1);
	}

	// 이곳에서 CPU 사용되는 코드를 실행하면 된다.
	{

	}

	// I/O가 완료될 때 까지 기다린다.
	::WaitForSingleObject(ov.hEvent, INFINITE);

	// I/O의 결과를 보고받는다.
	if (FALSE == ::GetOverlappedResult(h, &ov, &dwValue, TRUE))
	{
		// 에러 상황
		assert(FALSE);
		exit(1);
	}

	printf("Readed : %s\r\n", buf3);

	ZeroMemory(buf3, sizeof(buf3));

	// EOF 근처에서 한번 읽어 본다.
	// 즉시 리턴된다.
	nLarge.QuadPart = 2048 - 5;
	ov.Offset = nLarge.LowPart;
	ov.OffsetHigh = nLarge.HighPart;
	if (FALSE == ::ReadFile(h, buf3, 1024, NULL, &ov))
	{
		dwValue = ::GetLastError();
		if (dwValue == ERROR_IO_PENDING)
		{
			// i/o가 완료되지 않았음
			// 정상
		}
		else
		{
			// 에러 상황
			assert(FALSE);
			exit(1);
		}
	}
	else
	{
		// 예측되지 않은 에러 상황
		assert(FALSE);
		exit(1);
	}

	// I/O가 완료될 때 까지 기다린다.
	::WaitForSingleObject(ov.hEvent, INFINITE);

	// I/O의 결과를 보고받는다.
	if (FALSE == ::GetOverlappedResult(h, &ov, &dwValue, TRUE))
	{
		// 에러 상황
		assert(FALSE);
		exit(1);
	}

	printf("Readed : %s, Byte Transfered : %d\r\n", buf3, dwValue);

	// EOF 위치에서 읽어 본다.
	nLarge.QuadPart = 2048;
	ov.Offset = nLarge.LowPart;
	ov.OffsetHigh = nLarge.HighPart;
	if (FALSE == ::ReadFile(h, buf3, 1024, NULL, &ov))
	{
		dwValue = ::GetLastError();
		if (dwValue == ERROR_IO_PENDING)
		{
			// EOF 영역에서 읽어서, 실제로 읽을 것이 없겠지만,
			// I/O Pending이 들어온다.
			// 정상
		}
		else
		{
			// 에러 상황
			assert(FALSE);
			exit(1);
		}
	}
	else
	{
		// 예측되지 않은 에러 상황
		assert(FALSE);
		exit(1);
	}

	// I/O가 완료될 때 까지 기다린다.
	::WaitForSingleObject(ov.hEvent, INFINITE);

	// I/O의 결과를 보고받는다.
	if (FALSE == ::GetOverlappedResult(h, &ov, &dwValue, TRUE))
	{
		dwValue = ::GetLastError();
		if (dwValue == ERROR_HANDLE_EOF)
		{
			// EOF로 인해 실패함
			// 정상
		}
		else
		{
			// 예측되지 않은 에러 상황
			assert(FALSE);
			exit(1);
		}
	}
	else
	{
		// 성공할 수 없음
		// 예측되지 않은 에러 상황
		assert(FALSE);
		exit(1);
	}

	printf("Read Seek at %d, ERROR_HANDLE_EOF returned, Byte Transfered : %d\r\n", nLarge.QuadPart, dwValue);

	if (NULL != ov.hEvent)
	{
		::CloseHandle(ov.hEvent);
		ov.hEvent = NULL;
	}

	if (INVALID_HANDLE_VALUE != h)
	{
		::CloseHandle(h);
		h = INVALID_HANDLE_VALUE;
	}

	return 0;
}

