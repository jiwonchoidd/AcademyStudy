#include "TAsyncSelect.h"
// 사용자 정의 메세지( 네트워크 이벤트가 발생하면 ) 
#define NETWORK_MSG WM_USER+80
// 비동기셀렉트(AsyncSelect())

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
				}break;
				case FD_CLOSE:
				{
					m_bConnect = false;
				}break;
				case FD_READ:
				{
					RecvMsg();
					/*char buf[256] = { 0, };
					ZeroMemory(buf, sizeof(char) * 256);
					int iLen = 0;
					int m_iRecvByte = recv(m_Sock, buf, 256, 0);
					if (m_iRecvByte == 0 || m_iRecvByte == SOCKET_ERROR)
					{
						break;
					}
					buf[m_iRecvByte] = 0;
					MessageBox(g_hWnd, GetMtW(buf), L"msg", MB_OK);*/
				}break;
				case FD_WRITE:
				{
					int kkkk = 0;
					//MessageBox(g_hWnd, L"msg", L"msg", MB_OK);
				}break;
			}
		}
	}
};
TAsyncSelect::TAsyncSelect()
{
}


TAsyncSelect::~TAsyncSelect()
{
}
