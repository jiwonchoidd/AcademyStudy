#include "TAsyncSelect.h"
#include "TNetwork.h"
#define WM_ASYNC_SOCKET   (WM_USER+1)
const char username[] = "È«±æµ¿";
bool		TAsyncSelect::Init()
{
	//-------------------ASYNC-SELECT---------------------------
	int iRet = WSAAsyncSelect(m_Sock, m_hWnd,
		WM_ASYNC_SOCKET, 
		FD_CONNECT | 
		FD_CLOSE |
		FD_READ |
		FD_WRITE);
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}
	return true;
}

LRESULT		TAsyncSelect::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int iEvent = WSAGETSELECTEVENT(lParam);
	int iSock = WSAGETSELECTEVENT(wParam);
	if (WSAGETSELECTERROR(lParam))
	{		
		return 1;
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
	{
	}break;
	case FD_READ:
	{	
		char recvbuf[256] = { 0, };
		UPACKET packet;
		ZeroMemory(&packet, sizeof(packet));

		int iRecvByte = recv(m_Sock, (char*)&packet.ph, PACKET_HEADER_SIZE, 0);
		if (iRecvByte == 0)
		{
			break;
		}
		if (iRecvByte == SOCKET_ERROR)
		{
			printf("\n¼­¹ö Á¾·áµÊ");
			break;
		}
		if (iRecvByte == PACKET_HEADER_SIZE)
		{
			int iNumDataByte = packet.ph.len - PACKET_HEADER_SIZE;
			if (false == RecvMsg(m_Sock, packet.msg, iNumDataByte))
			{
				break;
			}
			iRecvByte = 0;

			TNetwork::m_RecvPool.push_back(packet);
		}
	};
	case FD_WRITE:
	{
		
	}break;
	case FD_CLOSE:
	{
		PostMessage(m_hWnd, WM_ASYNC_SOCKET,
			wParam, FD_READ);
	}break;
	}	
	return 1;
}

TAsyncSelect::TAsyncSelect(SOCKET sock)
{
	m_Sock = sock;
}


TAsyncSelect::~TAsyncSelect()
{
}
