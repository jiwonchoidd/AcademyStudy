#include "TAsyncSelect.h"
// 사용자 정의 메세지( 네트워크 이벤트가 발생하면 ) 
#define NETWORK_MSG WM_USER+80
// 비동기셀렉트(AsyncSelect())
int TAsyncSelect::SendMsg()
{
	if (m_bSendEnable == false) return 0;
	if (m_SendList.size() == 0) return 0;
	std::list<UPACKET>::iterator itor = m_SendList.begin();
	UPACKET data = *itor;

	{
		DWORD dwSendSize;
		DWORD dwflag = 0;
		WSABUF wsabuf;
		wsabuf.len = data.ph.len;
		wsabuf.buf = (char*)&data;

		int iRet = WSASend(m_Sock, &wsabuf, 1, &dwSendSize, dwflag, NULL, NULL);
		if (iRet == SOCKET_ERROR)
		{
			if (WSAEWOULDBLOCK == WSAGetLastError())
			{
				m_bSendEnable = FALSE;
				return 0;
			}
			return -1;
		}
		else
		{
			// 발생 오류!
			if (wsabuf.len != dwSendSize)
			{
				MessageBox(g_hWnd, L"WSASend", L"ERROR", MB_OK);
				return -1;
			}
			m_SendList.pop_front();
		}
	}
	return 0;
}
int TAsyncSelect::SendMsg(SOCKET sock, char* msg, WORD type)
{
	if (m_bSendEnable == false) return 0;
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph.type = type;// PACKET_CHAT_MSG;
						   // 방법 1
	memcpy(sendmsg.msg, msg, strlen(msg));
	// 방법 2
	//sprintf_s(sendmsg.msg,"%s", "[서버]환영합니다.");
	sendmsg.ph.len = strlen(sendmsg.msg) + PACKET_HEADER_SIZE;

	DWORD dwSendSize;
	DWORD dwflag = 0;
	WSABUF wsabuf;
	wsabuf.len = sendmsg.ph.len;
	wsabuf.buf = (char*)&sendmsg;

	int iRet = WSASend(m_Sock, &wsabuf, 1, &dwSendSize, dwflag, NULL, NULL);
	if (iRet == SOCKET_ERROR)
	{
		if (WSAEWOULDBLOCK == WSAGetLastError())
		{
			m_bSendEnable = FALSE;
			m_SendList.push_back(sendmsg);
			return 0;
		}
		return -1;
	}
	else
	{
		// 발생 오류!
		if (wsabuf.len != dwSendSize)
		{
			MessageBox(g_hWnd, L"WSASend", L"ERROR", MB_OK);
			return -1;
		}
	}
	return iRet;
}
int TAsyncSelect::SendMsg(char* msg, WORD type)
{	
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph.type = type;// PACKET_CHAT_MSG;
						   // 방법 1
	memcpy(sendmsg.msg, msg, strlen(msg));
	// 방법 2
	//sprintf_s(sendmsg.msg,"%s", "[서버]환영합니다.");
	sendmsg.ph.len = strlen(sendmsg.msg) + PACKET_HEADER_SIZE;
	int iRet = 0;
	if(m_bSendEnable==true)
	{
		iRet = send(m_Sock, (char*)&sendmsg, sendmsg.ph.len, 0);
	}
	else
	{
		Push(sendmsg);
	}
	return		iRet;
}
bool TAsyncSelect::Connect(char* ip, int iPort)
{
	// (2) 비동기 셀렉트에 윈도우 메세지 적용
	int iResult = WSAAsyncSelect(m_Sock,
		g_hWnd,
		NETWORK_MSG,
		FD_CONNECT | // connect
		FD_READ |    // recv
		FD_WRITE |   // send
		FD_CLOSE);
	if (iResult == SOCKET_ERROR)   // close
	{
		return false;
	}

	SOCKADDR_IN server;
	ZeroMemory(&server, sizeof(SOCKADDR_IN));
	server.sin_family = AF_INET;
	server.sin_port = htons(iPort);
	server.sin_addr.s_addr = inet_addr(ip);

	DWORD dwRet = WSAConnect(m_Sock,
		(sockaddr*)&server,
		sizeof(SOCKADDR_IN),
		NULL, NULL, NULL, NULL);
	if (SOCKET_ERROR == dwRet)
	{
		// 비동기 정상 반환 = WSAEWOULDBLOCK
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	return true;
};
void TAsyncSelect::MsgEvent(MSG msg)
{
	switch (msg.message)
	{
	case NETWORK_MSG:
	{
		// 서버가 종료되어도 들어온다.
		if (WSAGETSELECTERROR(msg.lParam) != 0)
		{
			MessageBox(g_hWnd, L"접속 종료", L"종료", MB_OK);
			PostQuitMessage(0);
			return;
		}
		WORD dwSelect = WSAGETSELECTEVENT(msg.lParam);
		switch (dwSelect)
		{
		case FD_CONNECT:  // 정상 연결
		{
			m_bConnect = true;
			PostMessage(g_hWnd, NETWORK_MSG, m_Sock, FD_READ);
		}break;
		case FD_CLOSE:
		{
			m_bConnect = false;
		}break;
		case FD_READ:
		{
			int iRet = RecvMsg();
			// 비정상 오류가 있거나 정상 종료시
			if (iRet < 0)
			{
				closesocket(m_Sock);
				break;
			}
			// 정상적인 상태이면
			if (iRet != WSAEWOULDBLOCK) 
			{
				PostMessage(g_hWnd, NETWORK_MSG, m_Sock, FD_READ);
			}
			// WSAEWOULDBLOCK == iRet
		}
		// 중요 break를 생략한다.
		case FD_WRITE:
		{
			m_bSendEnable = true;
			int iRet = SendMsg();
			if (iRet < 0) closesocket(m_Sock);
		}break;
		}
	}
	}
};
int TAsyncSelect::RecvMsg()
{
	{
		int iLen = 0;
		DWORD dwRecvSize;
		DWORD dwFlag = 0;
		WSABUF wsabuf;
		wsabuf.len = 2048;
		wsabuf.buf = m_szData;

		DWORD dwResult = WSARecv(m_Sock,
			&wsabuf, 1, &dwRecvSize, &dwFlag, NULL, NULL);

		//dwRecvSize==0 처리가 불필요하다.
		if (dwResult == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				return -1;
			}
			return WSAEWOULDBLOCK;
		}
		else
		{
			m_StreamPacket.Put(m_szData, dwRecvSize);
		}
	}
	return 0;
}
TAsyncSelect::TAsyncSelect()
{
}


TAsyncSelect::~TAsyncSelect()
{
}
