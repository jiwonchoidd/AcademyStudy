/*3번 청소끝났어요 그럼 3번 테이블 배치하고 다른 작업못함
파일을 소켓으로 이용하면 그대로 이용 가능하다.

완료되면 비동기 입출력 작업결과 저장 큐가 있음
입출력이 끝나면 데이터가 저장이 됨

스레드에서는 완료큐에 데이터가 있으면 작업한다.

완료 포트 관리 객체 생성
생성할떄는 첫번째에 INVALID_HANDLE_VALUE 초기화 값임.
비동기 결과 작업이 저장이된다. hiocp로 연결해서 
3문장으로 끝나버린것이다. Completionport 완료 포트
파일 포트가 완료가 되면 앞으로 1000번이라는 번호를 남겨줘
이걸 1000번을 완료 포트라고 한다. 1000번 작업 끝났어 리턴해준다.

GetQueuedCompletionStatus, iocp 핸들, 트랜스퍼, 키값, 오버랩, 1
키값이 1000번이면 비동기 작업 읽기가 완료되면 여기서 반환
Transfer는 얼만큼 읽었는디 얼마큼 썻는지
*/

/*Overlapped I/O와 IOCP의 차이점 중 하나는 "스레드 풀링"에 있습니다.

스레드 풀링은 재사용이 가능한 스레드를 유지할 수 있는 최적화 된 기능입니다.

IOCP의 강점 중 하나는 이러한 스레드 풀링을 활용할 수 있다는 점입니다.

WSASend / WSARecv와 같은 비동기 입출력 함수를 호출하면

스레드에서 내부적으로 APC(Asynchronous Procedure Call Queue) 큐가 생성되고,

여기에 입출력이 완료된 결과를 저장합니다.

APC 큐는 각 스레드마다 자동으로 생성되고 파괴된다는 특징이 있습니다.

모든 스레드는 자신만의 독립적인 APC 큐를 가지고 있으므로, APC 큐의 결과는

APC 큐를 소유하고 있는 스레드에서만 확인할 수 있습니다.

반면에 IOCP는 이러한 제약이 없습니다.

스레드 풀을 유지하기 때문에 사용자가 생성한 IOCP 커널 객체에 접근하는

여러 스레드들을 두고 입출력 결과를 처리할 수 있습니다.

스레드 풀의 크기는 보통 머신에 설치된 프로세서 * 2만큼 생성한다고 합니다.

IOCP를 쓰려면 IOCP객체를 생성해야합니다.

CreateCompletionPort 함수로 IOCP 객체를 생성할 수 있습니다.

블록 사이즈를 작게하니까 오류가 나는데 무슨 오류지.........
*/
#include <windows.h>
#include <iostream>
#define MAX_WORKER_THREAD 3
#define BlockSizes 104857600
WCHAR* g_buffer = NULL;
LARGE_INTEGER fileSize;
DWORD	   g_dwCurrentRead = 0;
DWORD	   g_dwCurrentWrite = 0;

//오버랩 구조체
OVERLAPPED g_ReadOVArray[100];
OVERLAPPED g_WriteOVArray[100];


HANDLE			g_hFileRead;
HANDLE			g_hFileWrite;
HANDLE			g_hWorkThread[MAX_WORKER_THREAD];
/*완료포트 관리 객체*/
HANDLE			g_hIOCP;
LARGE_INTEGER	g_LargeRead;
LARGE_INTEGER	g_LargeWrite;
//이벤트 핸들
HANDLE g_hKillEvent;
bool DispatchRead(DWORD dwTransfer, OVERLAPPED* ov)
{
	//디스패치 리드가 이해가 되지 않아요..
	g_LargeRead.QuadPart += dwTransfer;

	++g_dwCurrentWrite;
	g_WriteOVArray[g_dwCurrentWrite].Offset
		= g_LargeWrite.LowPart;
	g_WriteOVArray[g_dwCurrentWrite].OffsetHigh
		= g_LargeWrite.HighPart;

	DWORD dwWritten;
	BOOL ret = WriteFile(g_hFileWrite,
		g_buffer,
		dwTransfer,
		&dwWritten,
		&g_WriteOVArray[g_dwCurrentWrite]);
	if (ret == FALSE)
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}
bool DispatchWrite(DWORD dwTransfer, OVERLAPPED* ov)
{
	g_LargeWrite.QuadPart += dwTransfer;

	if (fileSize.QuadPart == g_LargeWrite.QuadPart)
	{
		::SetEvent(g_hKillEvent);
		return true;
	}

	++g_dwCurrentRead;
	g_ReadOVArray[g_dwCurrentRead].Offset
		= g_LargeRead.LowPart;
	g_ReadOVArray[g_dwCurrentRead].OffsetHigh
		= g_LargeRead.HighPart;

	DWORD dwRead;
	BOOL ret = ReadFile(g_hFileRead,
		g_buffer,
		BlockSizes,
		&dwRead,
		&g_ReadOVArray[g_dwCurrentRead]);
	if (ret == FALSE)
	{
		if (GetLastError() != ERROR_IO_PENDING)
		{
			return false;
		}
	}
	return true;
}
DWORD WINAPI WorkerThread(LPVOID param)
{
	DWORD dwTransfer;
	ULONG_PTR KeyValue;
	OVERLAPPED* pOverlapped;
	while (1)
	{
		//지정된 킬이벤트가 신호를 받았다면 break
		if (WaitForSingleObject(g_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
		//완료큐에 데이터가 있으면 작업시작
		BOOL bRet = ::GetQueuedCompletionStatus(
		g_hIOCP,&dwTransfer, &KeyValue, &pOverlapped, 1);

		if (bRet == TRUE)
		{
			//작업
			if (KeyValue == 1000)
			{
				//작업중이 아니라면 킬이벤트 발동
				if (!DispatchRead(dwTransfer, pOverlapped))
				{
					::SetEvent(g_hKillEvent);
					break;
				}
			}
			if (KeyValue == 1001)
			{
				//작업중이 아니라면 킬이벤트 발동
				if (!DispatchWrite(dwTransfer, pOverlapped))
				{
					::SetEvent(g_hKillEvent);
					break;
				}
			}
		}
		else
		{
			//오류
			if (GetLastError() != WAIT_TIMEOUT)
			{
				::SetEvent(g_hKillEvent);
				break;
			}
		}
	}
	return TRUE;
}

int main()
{
	setlocale(LC_ALL, "KOREAN");

	//이벤트 생성
	g_hKillEvent = CreateEvent(0, TRUE, FALSE, NULL);

	//읽기파일 핸들
	g_hFileRead = CreateFile(L"video.mp4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (g_hFileRead == INVALID_HANDLE_VALUE)
	{
		CloseHandle(g_hFileRead);
		return 0;
	}
	//쓰기파일 핸들
	g_hFileWrite = CreateFile(L"video_copy.mp4", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS
		, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (g_hFileWrite == INVALID_HANDLE_VALUE)
	{
		CloseHandle(g_hFileWrite);
		return 0;
	}
	//비동기 입출력 작업결과 저장 큐 IOCP 객체 생성
	//1번
	g_hIOCP = CreateIoCompletionPort(
		INVALID_HANDLE_VALUE, 0, 0, 0);
	//번호
	//입출력 작업이 끝나면 알려줄껍니다. 1000번 1001번 키값을 가짐
	//read write 파일입출력이 소켓으로 바뀌면 끝남
	CreateIoCompletionPort(g_hFileRead, g_hIOCP, 1000, 0);
	CreateIoCompletionPort(g_hFileWrite, g_hIOCP, 1001, 0);

	GetFileSizeEx(g_hFileRead, &fileSize);

	DWORD dwRead;
	DWORD dwWritten;
	DWORD dwTotalWrite = 0;

	g_buffer = new WCHAR[BlockSizes];

	//쓰레드 여러개 생성
	for (int i = 0; i < MAX_WORKER_THREAD; i++)
	{
		DWORD id;
		g_hWorkThread[i] = CreateThread(0, 0, WorkerThread, nullptr, 0, &id);
	}

	g_LargeRead.QuadPart = 0;
	g_LargeWrite.QuadPart = 0;
	// 비동기 로드	
	BOOL ret = ReadFile(g_hFileRead, g_buffer,
		BlockSizes, &dwRead,
		&g_ReadOVArray[g_dwCurrentRead++]);

	WaitForMultipleObjects(MAX_WORKER_THREAD,
		g_hWorkThread, TRUE, INFINITE);


	//IOCP 및 쓰레드 핸들 해제..
	for (int iThread = 0;
		iThread < MAX_WORKER_THREAD;  iThread++)
	{
		CloseHandle(g_hWorkThread[iThread]);
	}
	CloseHandle(g_hKillEvent);
	CloseHandle(g_hIOCP);
	CloseHandle(g_hFileRead);
	CloseHandle(g_hFileWrite);
	return 0;

}
