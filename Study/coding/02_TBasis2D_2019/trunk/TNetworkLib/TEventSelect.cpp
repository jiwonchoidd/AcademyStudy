#include "TEventSelect.h"
#include "TNetwork.h"

bool		TEventSelect::Init()
{
	//-------------------ASYNC-SELECT---------------------------
	m_EventArray[0] = WSACreateEvent();
	int iRet = WSAEventSelect(m_Sock, 
		m_EventArray[0], FD_CONNECT |
							FD_CLOSE |
							FD_READ |
							FD_WRITE);
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

bool		TEventSelect::Frame()
{
	INT iIndex = WSAWaitForMultipleEvents(
		1,
		m_EventArray,
		TRUE,
		0,
		FALSE);
	if (iIndex == WSA_WAIT_FAILED) return true;
	if (iIndex == WSA_WAIT_TIMEOUT) return true;
	iIndex -= WSA_WAIT_EVENT_0;

	WSANETWORKEVENTS networkevent;
	// 비신호상태로 전환한다.
	int iRet = WSAEnumNetworkEvents(
		m_Sock,
		m_EventArray[iIndex],
		&networkevent);

	if (iRet == SOCKET_ERROR)
	{
		return false;
	}
	if (networkevent.lNetworkEvents & FD_READ)
	{
		if (networkevent.iErrorCode[FD_READ_BIT] != 0)
		{
			return false;
		}
		if (RecvData() == false)
		{
			return false;
		}
	}
	if (networkevent.lNetworkEvents & FD_WRITE)
	{
		if (networkevent.iErrorCode[FD_WRITE_BIT] != 0)
		{
			return false;
		}
		//printf(".");
	}
	if (networkevent.lNetworkEvents & FD_CLOSE)
	{
		if (networkevent.iErrorCode[FD_CLOSE_BIT] != 0)
		{
			return false;
		}
		TNetwork::m_bConnect = false;
	}

	return true;
}
TEventSelect::TEventSelect(SOCKET sock)
{
	m_Sock = sock;
}


TEventSelect::~TEventSelect()
{
}
