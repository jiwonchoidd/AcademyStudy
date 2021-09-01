#include "Sample.h"
bool Sample::Init()
{	
	// 윈도우 컨트롤(edit)
	DWORD sytle = WS_CHILD | WS_VISIBLE | ES_MULTILINE ;
	m_hEdit = CreateWindow(L"edit", NULL,
		sytle, 420, 100, 200, 100,
		m_hWnd,
		(HMENU)100, // id
		m_hInstance, NULL);
	// 윈도우 컨트롤(button)
	sytle = WS_CHILD | WS_VISIBLE;
	m_hButton = CreateWindow(L"button", L"전송",
		sytle, 630, 100, 200, 100,
		m_hWnd,
		(HMENU)200, // id
		m_hInstance, NULL);

	sytle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL;
	m_hList = CreateWindow(L"listbox", NULL,
		sytle, 10, 10, 400, 500,
		m_hWnd,
		(HMENU)300, // id
		m_hInstance, NULL);

	SendMessage(m_hList, LB_ADDSTRING, 0, (LPARAM)L"채팅 시작");

	m_Network.Init();	
	if (!m_Network.Connect("192.168.3.22", 10000))
	{
		return false;
	}	
	return true;
}
LRESULT	Sample::MsgProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{
		case WM_COMMAND:
		{
			// id에 해당하는 컨트롤
			switch (LOWORD(wParam))
			{
				case 200:
				{
					// 버튼클릭
					TCHAR buffer[MAX_PATH] = { 0 };
					SendMessage(m_hEdit, WM_GETTEXT, MAX_PATH,(LPARAM)buffer);
					
					int iCnt = SendMessage(m_hList, LB_GETCOUNT, 0, 0);
					if (iCnt > 20)
					{
						SendMessage(m_hList, LB_RESETCONTENT, 0, 0);
					}
					//SendMessage(m_hList, LB_ADDSTRING, 0, (LPARAM)buffer);
					int iSendByte = m_Network.SendMsg(GetWtM(buffer), PACKET_CHAT_MSG);
					if (iSendByte == SOCKET_ERROR) break;
				}
			}
		}break;
	}
	return 0;
}
void Sample::MsgEvent(MSG msg)
{
	m_Network.MsgEvent(msg);
};
bool Sample::Frame()
{
	if (m_Network.m_RecvList.empty() == false)
	{
		for (std::list<UPACKET>::iterator itor = m_Network.m_RecvList.begin();
		itor != m_Network.m_RecvList.end();
			itor++)
		{
			UPACKET pPacket = *itor;
			switch (pPacket.ph.type)
			{
				case PACKET_CHAT_MSG:
				{
					SendMessage(m_hList, LB_ADDSTRING, 0, (LPARAM)GetMtW(pPacket.msg));
				}break;
			}			
		}
		m_Network.m_RecvList.clear();
	}
	return true;
}
bool Sample::Release()
{
	m_Network.Release();
	return true;
}

Sample::Sample()
{
}


Sample::~Sample()
{
}
TCORE_RUN(Sample, 1024, 768);