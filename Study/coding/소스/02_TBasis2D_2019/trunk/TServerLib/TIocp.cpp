#include "TIocp.h"
DWORD WINAPI WorkerThread(LPVOID param)
{
	TIocp* pIOCP = (TIocp*)param;
	HANDLE hIOCP = pIOCP->m_hIOCP;
	BOOL bReturn;
	DWORD dwTransfer;
	ULONG_PTR uKeyValue;
	OVERLAPPED2* ov;
	while (1)
	{
		bReturn = ::GetQueuedCompletionStatus(hIOCP,
			&dwTransfer,
			&uKeyValue,
			(LPOVERLAPPED*)&ov, INFINITE);
		
		TUser* pUser = (TUser*)uKeyValue;
		if (ov!=nullptr && ov->iFlag == PACK_END)
		{
			I_UserMgr.DelUser(pUser);
			continue;
		}
		if (bReturn == TRUE)
		{				
			if (dwTransfer == 0)
			{
				ov->iFlag = PACK_END;
				PostQueuedCompletionStatus(hIOCP, 0, uKeyValue, (LPOVERLAPPED)ov);
			}
			else
			{
				if (pUser->Dispatch(dwTransfer, ov)==false)
				{
					ov->iFlag = PACK_END;
					PostQueuedCompletionStatus(hIOCP, 0, uKeyValue, (LPOVERLAPPED)ov);
				}
			}
		}
		else
		{
			ov->iFlag = PACK_END;
			PostQueuedCompletionStatus(hIOCP,0, uKeyValue, (LPOVERLAPPED)ov);
		}
	}
	return 1;
}
bool TIocp::Run()
{
	WaitForMultipleObjects(MAX_WORKER_THREAD,m_hWorkThread, TRUE, INFINITE);	

	for (int iThread = 0;
		iThread < MAX_WORKER_THREAD;
		iThread++)
	{
		CloseHandle(m_hWorkThread[iThread]);
	}
	return true;
}
void TIocp::SetSocketBind(SOCKET sock, ULONG_PTR key)
{
	::CreateIoCompletionPort((HANDLE)sock,
		m_hIOCP, 
		key,
		0);
}
bool TIocp::Init()
{
	m_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	
	// iocp queue -> worker thread
	DWORD threadID;
	for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
	{
		m_hWorkThread[iThread] =
			::CreateThread(0, 0, WorkerThread,
				this,
				0, &threadID);
	}	
	return true;
}

TIocp::TIocp()
{
}


TIocp::~TIocp()
{
	CloseHandle(m_hIOCP);
}
