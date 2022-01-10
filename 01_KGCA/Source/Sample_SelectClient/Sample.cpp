#define IP_KGCA "192.168.0.56"
#define IP_TKGCA "192.168.0.12"
#include "Sample.h"
LRESULT  Sample::ExternMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			//��ư�� �������� �޼����� buffer�� ������. send
		case 200:
		{
			char buffer[MAX_PATH] = { 0, };
			SendMessageA(m_hEditBox, WM_GETTEXT, MAX_PATH, (LPARAM)buffer);
			KPacket tPacket(PACKET_CHAT_MSG);
			tPacket << 123 << "������" << (short)12 << buffer;
			m_Net.SendMsg(m_Net.m_Sock, tPacket.m_uPacket);

			SendMessageA(m_hEditBox, WM_SETTEXT, 0, (LPARAM)"");
		}break;
		}
	}break;
	}
	return m_Net.Select_MsgProc(hWnd, msg, wParam, lParam);
}
bool Sample::Init()
{
    m_Camera.CreateViewMatrix(KVector3(0, 0, -20), KVector3(0, 0, 0));
    m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.45f, (float)g_rtClient.right / (float)g_rtClient.bottom);

    DWORD style = WS_CHILD | WS_VISIBLE | ES_MULTILINE;
	m_hEditBox = CreateWindow(L"edit", NULL, style,
		10, g_rtClient.bottom - 60, g_rtClient.right-120, 50,
		m_hWnd, (HMENU)100, m_hInstance, NULL);
	style = WS_CHILD | WS_VISIBLE;
	m_hButton = CreateWindow(L"button", L"����", style,
		g_rtClient.right - 90, g_rtClient.bottom - 60, 50, 50,
		m_hWnd, (HMENU)200, m_hInstance, NULL);
	m_hListBox = CreateWindow(L"listbox", NULL, style,
		10, 10, g_rtClient.right - 50, g_rtClient.bottom - 70,
		m_hWnd, (HMENU)300, m_hInstance, NULL);

	SendMessageA(m_hListBox, LB_ADDSTRING, 0, (LPARAM)" -> ä���� ǥ�� �˴ϴ�.");

	m_Net.InitNetwork();
	m_Net.Connect(g_hWnd, SOCK_STREAM, 10000, IP_TKGCA);
    return true;
}
bool Sample::Frame()
{
	// ������ �Լ�
	//��Ŷ Ǯ�� ������� ��ȭ ī��Ʈ�� �Ѵ�.
	int iChatCnt = m_Net.m_User.m_lPacketPool.size();
	if (iChatCnt > 0 && m_iChatCount != iChatCnt)
	{
		m_iChatCount = iChatCnt;
		//ī��Ʈ�� �þ�� �� �����.
		SendMessage(m_hListBox, LB_RESETCONTENT, 0, 0);

		std::list<KPacket>::iterator iter;
		//��ȭ���� ���ѷ��� ������ ������
		if (m_Net.m_User.m_lPacketPool.size() > 30)
		{
			m_Net.m_User.m_lPacketPool.pop_front();
		}
		for (iter = m_Net.m_User.m_lPacketPool.begin();
			iter != m_Net.m_User.m_lPacketPool.end();
			iter++)
		{
			KChatting recvdata;
			ZeroMemory(&recvdata, sizeof(recvdata));
			(*iter) >> recvdata.index >> recvdata.name
				>> recvdata.damage >> recvdata.message;
			SendMessageA(m_hListBox, LB_ADDSTRING, 0,
				(LPARAM)recvdata.name );
			SendMessageA(m_hListBox, LB_ADDSTRING, 0,
				(LPARAM)recvdata.message);
			(*iter).Reset();
		}
	}
    return true;
}
bool Sample::Render()
{
    DrawTestBox(g_fSecTimer * m_Speed,
        0.0f, 0.0f, 0.0f, nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);

    return true;
}
bool Sample::Release()
{
    m_Camera.Release();
    return true;
}

Sample::Sample()
{
}

Sample::~Sample()
{
}

WinMain_OPT(ä�� �׽�Ʈ, 500, 400);