/*3�� û�ҳ������ �׷� 3�� ���̺� ��ġ�ϰ� �ٸ� �۾�����
������ �������� �̿��ϸ� �״�� �̿� �����ϴ�.

�Ϸ�Ǹ� �񵿱� ����� �۾���� ���� ť�� ����
������� ������ �����Ͱ� ������ ��

�����忡���� �Ϸ�ť�� �����Ͱ� ������ �۾��Ѵ�.

�Ϸ� ��Ʈ ���� ��ü ����
�����ҋ��� ù��°�� INVALID_HANDLE_VALUE �ʱ�ȭ ����.
�񵿱� ��� �۾��� �����̵ȴ�. hiocp�� �����ؼ� 
3�������� �����������̴�. Completionport �Ϸ� ��Ʈ
���� ��Ʈ�� �Ϸᰡ �Ǹ� ������ 1000���̶�� ��ȣ�� ������
�̰� 1000���� �Ϸ� ��Ʈ��� �Ѵ�. 1000�� �۾� ������ �������ش�.

GetQueuedCompletionStatus, iocp �ڵ�, Ʈ������, Ű��, ������, 1
Ű���� 1000���̸� �񵿱� �۾� �бⰡ �Ϸ�Ǹ� ���⼭ ��ȯ
Transfer�� ��ŭ �о��µ� ��ŭ ������
*/

/*Overlapped I/O�� IOCP�� ������ �� �ϳ��� "������ Ǯ��"�� �ֽ��ϴ�.

������ Ǯ���� ������ ������ �����带 ������ �� �ִ� ����ȭ �� ����Դϴ�.

IOCP�� ���� �� �ϳ��� �̷��� ������ Ǯ���� Ȱ���� �� �ִٴ� ���Դϴ�.

WSASend / WSARecv�� ���� �񵿱� ����� �Լ��� ȣ���ϸ�

�����忡�� ���������� APC(Asynchronous Procedure Call Queue) ť�� �����ǰ�,

���⿡ ������� �Ϸ�� ����� �����մϴ�.

APC ť�� �� �����帶�� �ڵ����� �����ǰ� �ı��ȴٴ� Ư¡�� �ֽ��ϴ�.

��� ������� �ڽŸ��� �������� APC ť�� ������ �����Ƿ�, APC ť�� �����

APC ť�� �����ϰ� �ִ� �����忡���� Ȯ���� �� �ֽ��ϴ�.

�ݸ鿡 IOCP�� �̷��� ������ �����ϴ�.

������ Ǯ�� �����ϱ� ������ ����ڰ� ������ IOCP Ŀ�� ��ü�� �����ϴ�

���� ��������� �ΰ� ����� ����� ó���� �� �ֽ��ϴ�.

������ Ǯ�� ũ��� ���� �ӽſ� ��ġ�� ���μ��� * 2��ŭ �����Ѵٰ� �մϴ�.

IOCP�� ������ IOCP��ü�� �����ؾ��մϴ�.

CreateCompletionPort �Լ��� IOCP ��ü�� ������ �� �ֽ��ϴ�.

��� ����� �۰��ϴϱ� ������ ���µ� ���� ������.........
*/
#include <windows.h>
#include <iostream>
#define MAX_WORKER_THREAD 3
#define BlockSizes 104857600
WCHAR* g_buffer = NULL;
LARGE_INTEGER fileSize;
DWORD	   g_dwCurrentRead = 0;
DWORD	   g_dwCurrentWrite = 0;

//������ ����ü
OVERLAPPED g_ReadOVArray[100];
OVERLAPPED g_WriteOVArray[100];


HANDLE			g_hFileRead;
HANDLE			g_hFileWrite;
HANDLE			g_hWorkThread[MAX_WORKER_THREAD];
/*�Ϸ���Ʈ ���� ��ü*/
HANDLE			g_hIOCP;
LARGE_INTEGER	g_LargeRead;
LARGE_INTEGER	g_LargeWrite;
//�̺�Ʈ �ڵ�
HANDLE g_hKillEvent;
bool DispatchRead(DWORD dwTransfer, OVERLAPPED* ov)
{
	//����ġ ���尡 ���ذ� ���� �ʾƿ�..
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
		//������ ų�̺�Ʈ�� ��ȣ�� �޾Ҵٸ� break
		if (WaitForSingleObject(g_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
		//�Ϸ�ť�� �����Ͱ� ������ �۾�����
		BOOL bRet = ::GetQueuedCompletionStatus(
		g_hIOCP,&dwTransfer, &KeyValue, &pOverlapped, 1);

		if (bRet == TRUE)
		{
			//�۾�
			if (KeyValue == 1000)
			{
				//�۾����� �ƴ϶�� ų�̺�Ʈ �ߵ�
				if (!DispatchRead(dwTransfer, pOverlapped))
				{
					::SetEvent(g_hKillEvent);
					break;
				}
			}
			if (KeyValue == 1001)
			{
				//�۾����� �ƴ϶�� ų�̺�Ʈ �ߵ�
				if (!DispatchWrite(dwTransfer, pOverlapped))
				{
					::SetEvent(g_hKillEvent);
					break;
				}
			}
		}
		else
		{
			//����
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

	//�̺�Ʈ ����
	g_hKillEvent = CreateEvent(0, TRUE, FALSE, NULL);

	//�б����� �ڵ�
	g_hFileRead = CreateFile(L"video.mp4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (g_hFileRead == INVALID_HANDLE_VALUE)
	{
		CloseHandle(g_hFileRead);
		return 0;
	}
	//�������� �ڵ�
	g_hFileWrite = CreateFile(L"video_copy.mp4", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS
		, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (g_hFileWrite == INVALID_HANDLE_VALUE)
	{
		CloseHandle(g_hFileWrite);
		return 0;
	}
	//�񵿱� ����� �۾���� ���� ť IOCP ��ü ����
	//1��
	g_hIOCP = CreateIoCompletionPort(
		INVALID_HANDLE_VALUE, 0, 0, 0);
	//��ȣ
	//����� �۾��� ������ �˷��ٲ��ϴ�. 1000�� 1001�� Ű���� ����
	//read write ����������� �������� �ٲ�� ����
	CreateIoCompletionPort(g_hFileRead, g_hIOCP, 1000, 0);
	CreateIoCompletionPort(g_hFileWrite, g_hIOCP, 1001, 0);

	GetFileSizeEx(g_hFileRead, &fileSize);

	DWORD dwRead;
	DWORD dwWritten;
	DWORD dwTotalWrite = 0;

	g_buffer = new WCHAR[BlockSizes];

	//������ ������ ����
	for (int i = 0; i < MAX_WORKER_THREAD; i++)
	{
		DWORD id;
		g_hWorkThread[i] = CreateThread(0, 0, WorkerThread, nullptr, 0, &id);
	}

	g_LargeRead.QuadPart = 0;
	g_LargeWrite.QuadPart = 0;
	// �񵿱� �ε�	
	BOOL ret = ReadFile(g_hFileRead, g_buffer,
		BlockSizes, &dwRead,
		&g_ReadOVArray[g_dwCurrentRead++]);

	WaitForMultipleObjects(MAX_WORKER_THREAD,
		g_hWorkThread, TRUE, INFINITE);


	//IOCP �� ������ �ڵ� ����..
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
