#include "OverlappedPool.h"
#define MAX_WORKER_THREAD 5
#define g_dwMaxReadSize (4096*4096)
HANDLE		g_hIOCP;
HANDLE		g_hReadFile;
HANDLE		g_hWriteFile;
HANDLE		g_hWorkThread[MAX_WORKER_THREAD];
HANDLE	    g_hKillEvent;
HANDLE	    g_hEventEOF;

char*   g_pOffsetData = nullptr;
char    g_pDataBuffer[g_dwMaxReadSize];
LARGE_INTEGER g_LargeWrite;
LARGE_INTEGER g_LargeRead;
CRITICAL_SECTION RecvCS;
CRITICAL_SECTION WriteCS;

struct T_OV : public ObjectPool<T_OV>
{
	OVERLAPPED ov;
};

bool  DispatchRead(DWORD dwTrans, T_OV* ov)
{	
	std::cout << "R" <<ov->g_iIndex << "[" << GetCurrentThreadId() << "]" << std::endl;
	delete ov;

	//EnterCriticalSection(&RecvCS);
		g_LargeRead.QuadPart += dwTrans;
	//LeaveCriticalSection(&RecvCS);

	T_OV* pov = new T_OV;
	ZeroMemory(pov, sizeof(T_OV));
	pov->ov.Offset = g_LargeWrite.LowPart;
	pov->ov.OffsetHigh = g_LargeWrite.HighPart;
	
	DWORD dwWriteByte = 0;
	BOOL bResult = WriteFile(g_hWriteFile,g_pDataBuffer, dwTrans,&dwWriteByte, &pov->ov);
	if (bResult == FALSE)
	{
		DWORD error = GetLastError();
		if (error != ERROR_IO_PENDING)
		{
			return false;
		}
	}	
	return true;
}
bool  DispatchWrite(DWORD dwTrans, T_OV* ov)
{	
	std::cout << "W" << ov->g_iIndex << "[" << GetCurrentThreadId() << "]" << std::endl;
	delete ov;
	
	//EnterCriticalSection(&WriteCS);
		g_LargeWrite.QuadPart += dwTrans;
	//LeaveCriticalSection(&WriteCS);

	if (WaitForSingleObject(g_hEventEOF, 1) == WAIT_OBJECT_0)
	{
		if (g_LargeRead.QuadPart <= g_LargeWrite.QuadPart)
		{
			::SetEvent(g_hKillEvent);
			std::cout << "DispatchWrite";
			return true;
		}
	}

	T_OV* pov = new T_OV;
	ZeroMemory(pov, sizeof(T_OV));
	pov->ov.Offset = g_LargeRead.LowPart;
	pov->ov.OffsetHigh = g_LargeRead.HighPart;
	DWORD dwReadByte = 0;
	BOOL bResult = ReadFile(g_hReadFile,g_pDataBuffer, g_dwMaxReadSize,	&dwReadByte, &pov->ov);
	if (bResult == FALSE)
	{
		DWORD dwError = GetLastError();
		if (dwError != ERROR_IO_PENDING)
		{
			return false;
		}	
	}
	return true;
}
DWORD WINAPI WorkerThread(LPVOID param)
{
	BOOL bReturn;
	DWORD dwTransfer;
	ULONG_PTR KeyValue;
	OVERLAPPED*   pOV;
	while (1)
	{
		if (WaitForSingleObject(g_hKillEvent, 1) ==	WAIT_OBJECT_0)
		{
			std::cout << std::endl;
			std::cout << GetCurrentThreadId() << std::endl;
			break;
		}
		// 모든 출력 이후에 ERROR_HANDLE_EOF가 발생하면  DispatchWrite함수가 호출될 수 없다.
		// 이때에 g_hKillEvent 이벤트를 호출한다.
		if (WaitForSingleObject(g_hEventEOF, 1) ==	WAIT_OBJECT_0)
		{
			if (g_LargeRead.QuadPart <= g_LargeWrite.QuadPart)
			{
				::SetEvent(g_hKillEvent);
				continue;
			}
		}

		bReturn = ::GetQueuedCompletionStatus(g_hIOCP,&dwTransfer,&KeyValue,(LPOVERLAPPED*)&pOV, 1);

		if (bReturn == TRUE)
		{
			T_OV* tOV = (T_OV*)pOV;
			// dispatch
			if (KeyValue == 1000)
			{				
				if (DispatchRead(dwTransfer, tOV) == false)
				{
					::SetEvent(g_hKillEvent);
				}
			}
			if (KeyValue == 2000)
			{
				if (DispatchWrite(dwTransfer, tOV) == false)
				{
					::SetEvent(g_hKillEvent);
				}
			}
		}
		else
		{
			DWORD dwError = GetLastError();
			if (dwError == WAIT_TIMEOUT)
			{
				continue;
			}
			if (pOV != NULL)
			{
				if (ERROR_HANDLE_EOF == dwError)
				{
					::SetEvent(g_hEventEOF);
					std::wcout << "ERROR_HANDLE_EOF" << std::endl;
				}
			}
			else
			{
				std::wcout << dwError << std::endl;
			}
		}
	}
	return 1;
}
void main()
{
	InitializeCriticalSection(&RecvCS);
	InitializeCriticalSection(&WriteCS);
	g_hKillEvent = ::CreateEvent(0, TRUE, FALSE, 0);
	g_hEventEOF = ::CreateEvent(0, TRUE, FALSE, 0);

	std::wstring readname = L"Demo";
	std::wstring writename = L"Copy";
	std::wstring ext = L".7z";
	readname += ext;
	writename += ext;
	g_hReadFile = CreateFile(readname.c_str(),
		GENERIC_READ | GENERIC_WRITE, 0,
		NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);
	g_hWriteFile = CreateFile(writename.c_str(),
		GENERIC_READ | GENERIC_WRITE, 0,
		NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);
	//  ipcp 객체 초기화
	g_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	::CreateIoCompletionPort(g_hReadFile, g_hIOCP, 1000, 0);
	::CreateIoCompletionPort(g_hWriteFile, g_hIOCP, 2000, 0);

	// iocp queue -> worker thread
	DWORD threadID;
	for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
	{
		g_hWorkThread[iThread] =
			::CreateThread(0, 0, WorkerThread,
				nullptr,
				0, &threadID);
	}
	g_LargeRead.QuadPart = 0;
	g_LargeWrite.QuadPart = 0;
	T_OV* pov = new T_OV;
	ZeroMemory(pov, sizeof(T_OV));
	DWORD dwReadByte = 0;
	BOOL bResult = ReadFile(g_hReadFile,
		g_pDataBuffer, g_dwMaxReadSize,
		&dwReadByte, &pov->ov);

	WaitForMultipleObjects(MAX_WORKER_THREAD,
		g_hWorkThread, TRUE, INFINITE);

	CloseHandle(g_hKillEvent);
	CloseHandle(g_hReadFile);
	CloseHandle(g_hWriteFile);
	for (int iThread = 0;
		iThread < MAX_WORKER_THREAD;
		iThread++)
	{
		CloseHandle(g_hWorkThread[iThread]);
	}

	DeleteCriticalSection(&RecvCS);
	DeleteCriticalSection(&WriteCS);
}
