#include "TIocp.h"
#include "TServer.h"
DWORD WINAPI WorkerThread(LPVOID param)
{
	TIocp* iocp = (TIocp*)param;
	BOOL bReturn;
	DWORD dwTransfor;
	ULONG_PTR keyValue;
	OVERLAPPED2* pOV;
	while (1)
	{
		if (WaitForSingleObject(TServer::g_hKillEvent, 1) == WAIT_OBJECT_0)
		{
			break;
		}
		// 비동기 작업 결과를 iocp큐에서 확인
		bReturn = ::GetQueuedCompletionStatus(iocp->m_hIOCP,
			&dwTransfor,
			&keyValue,
			(LPOVERLAPPED*)&pOV, 1);// INFINITE);

		TNetUser* pUser = (TNetUser*)keyValue;
		if (pOV != nullptr && pOV->iType == OVERLAPPED2::MODE_EXIT)
		{
			iocp->m_pServer->SendLogoutUser(pUser);
			continue;
		}
		// 스레드 깨운다
		if (bReturn == TRUE)
		{
			// 정상종료
			if (dwTransfor == 0)
			{
				pOV->iType = OVERLAPPED2::MODE_EXIT;
				PostQueuedCompletionStatus(iocp->m_hIOCP, 0,
					keyValue, (LPOVERLAPPED)pOV);
			}		
			
			if (pOV->iType == OVERLAPPED2::MODE_RECV)
			{				
				//// 로드완료
				if (pUser->DispatchRead(dwTransfor, pOV) == false)
				{
					pOV->iType = OVERLAPPED2::MODE_EXIT;
					PostQueuedCompletionStatus(iocp->m_hIOCP, 0,
						keyValue, (LPOVERLAPPED)pOV);
				}
			}
			if (pOV->iType == OVERLAPPED2::MODE_SEND)
			{
				if (pUser->DispatchWrite(dwTransfor, pOV) == false)
				{
					pOV->iType = OVERLAPPED2::MODE_EXIT;
					PostQueuedCompletionStatus(iocp->m_hIOCP, 0,
						keyValue, (LPOVERLAPPED)pOV);
				}
			}
		}
		else
		{
			DWORD dwError = WSAGetLastError();
			if (dwError == WAIT_TIMEOUT) { continue; }	
			if (pOV != nullptr && pUser != nullptr)
			{
				closesocket(pUser->m_Sock);
			}
		}
	}
	return 0;
}
bool TIocp::Init()
{
	m_hIOCP = ::CreateIoCompletionPort(
			INVALID_HANDLE_VALUE, 0,0,0);
	for (int iThread = 0; iThread < MAX_WORKER_THREAD; iThread++)
	{
		m_hWorkThread[iThread] =
			::CreateThread(0, 0, WorkerThread, this, 0, &m_dwThreadID[iThread]);
	}
	return false;
}

bool TIocp::Run()
{
	return false;
}

bool TIocp::Release()
{
	CloseHandle(m_hIOCP);	
	for (int iThread = 0;
		iThread < MAX_WORKER_THREAD;
		iThread++)
	{
		CloseHandle(m_hWorkThread[iThread]);
	}
	return false;
}

void TIocp::SetBind(SOCKET sock, ULONG_PTR key)
{
	::CreateIoCompletionPort((HANDLE)sock, m_hIOCP, key, 0);
}

TIocp::TIocp(TServer* pServer) : m_pServer(pServer)
{
}

TIocp::~TIocp()
{
}
