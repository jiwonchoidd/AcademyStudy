#include "Sample.h"
#include "TStdSelect.h"
#include "TEventSelect.h"
#include "TAsyncSelect.h"

#define XXS_DOUBLEBUFFER         (0x0001)
HFONT	g_hFont = NULL;
WNDPROC g_OldEditProc;
HWND	g_hEdit;
HWND	g_hButton;
RECT	g_rtEdit;
RECT	g_rtButton;
TNetwork	Sample::m_Network;

char username[] = "홍길동";
static void CustomPaint(HWND hwnd, HDC hDC, RECT* rcDirty, BOOL bErase)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;
	GetClientRect(hwnd, &rect);
	hdc = BeginPaint(hwnd, &ps);
	SetTextColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);
//	DrawText(hdc, _T("Hello World!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	EndPaint(hwnd, &ps);
}
static void CustomDoubleBuffer(HWND hwnd, PAINTSTRUCT* pPaintStruct)
{
	int cx = pPaintStruct->rcPaint.right - pPaintStruct->rcPaint.left;
	int cy = pPaintStruct->rcPaint.bottom - pPaintStruct->rcPaint.top;
	HDC hMemDC;
	HBITMAP hBmp;
	HBITMAP hOldBmp;
	POINT ptOldOrigin;

	// Create new bitmap-back device context, large as the dirty rectangle.
	hMemDC = CreateCompatibleDC(pPaintStruct->hdc);
	hBmp = CreateCompatibleBitmap(pPaintStruct->hdc, cx, cy);
	hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);

	// Do the painting into the memory bitmap.
	OffsetViewportOrgEx(hMemDC, -(pPaintStruct->rcPaint.left),
		-(pPaintStruct->rcPaint.top), &ptOldOrigin);
	CustomPaint(hwnd, hMemDC, &pPaintStruct->rcPaint, TRUE);
	SetViewportOrgEx(hMemDC, ptOldOrigin.x, ptOldOrigin.y, NULL);

	// Blit the bitmap into the screen. This is really fast operation and although
	// the CustomPaint() can be complex and slow there will be no flicker any more.
	BitBlt(pPaintStruct->hdc, pPaintStruct->rcPaint.left, pPaintStruct->rcPaint.top,
		cx, cy, hMemDC, 0, 0, SRCCOPY);

	// Clean up.
	SelectObject(hMemDC, hOldBmp);
	DeleteObject(hBmp);
	DeleteDC(hMemDC);
}
LRESULT CALLBACK SuperProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (iMessage == WM_LBUTTONDOWN)
	{
		char buffer[MAX_PATH] = { 0, };
		SendMessageA(g_hEdit,
			WM_GETTEXT,
			MAX_PATH,
			(LPARAM)buffer);
		Sample::m_Network.SendMsg(PACKET_CHAR_MSG, buffer, sizeof(buffer));
		return TRUE;
	}
	return  CallWindowProc(g_OldEditProc, hWnd, iMessage, wParam, lParam);
}
bool Sample::PacketProcess()
{
	for (UPACKET& packet : TNetwork::m_RecvPool)
	{
		switch (packet.ph.type)
		{
		case PACKET_DRUP_USERS_SC:
		{
			USER_BASE userinfo;
			ZeroMemory(&userinfo, sizeof(USER_BASE));
			memcpy(&userinfo, packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
			printf("%s:님이 퇴장하였습니다.", userinfo.szName);
		}break;
		case PACKET_DRUP_SC_ACK:
		{
			printf("\n채팅방 퇴장.");
		}break;
		case PACKET_JOIN_SIGNIN_SC:
		{
			printf("\n채팅방 입장.\n");
		}break;
		case  PACKET_CHAR_MSG:
		{
			USER_CHAT_MSG message;
			ZeroMemory(&message, sizeof(USER_CHAT_MSG));
			memcpy(&message, packet.msg, packet.ph.len - PACKET_HEADER_SIZE);
			//printf("\n[%s]%s", message.szName, message.msg);
			C_STR msg = "[";
			msg += message.szName;
			msg += "]";
			msg += message.msg;
			OutputDebugStringA(msg.c_str());
			TMsg tMsg;
			tMsg.rt = g_rtClient;
			tMsg.rt.top = 50;
			tMsg.msg = to_mw(msg);
			g_Write.push(tMsg);
		}break;
		case  PACKET_CHAR_NAME_SC_REQ:
		{
			USER_BASE data;
			ZeroMemory(&data, sizeof(USER_BASE));
			data.iIndex = 8;
			memcpy(data.szName, username, strlen(username));
			m_Network.m_pModel->SendMsg(m_Network.m_Sock, PACKET_CHAR_NAME_CS_ACK, (char*)&data, sizeof(USER_BASE));
		}break;
		}
	}
	TNetwork::m_RecvPool.clear();
	return true;
}
bool Sample::Init()
{
	g_rtEdit = { 0, g_rtClient.bottom - 30, g_rtClient.right - 100, 30};
	g_rtButton = { g_rtEdit.right + 10, g_rtEdit.top, 50, 30 };

	WNDCLASS WndClass;
	GetClassInfo(NULL, TEXT("edit"), &WndClass); // "edit"클래스에 대한 정보 조사 
	WndClass.hInstance = g_hInstance;
	WndClass.style = CS_OWNDC;
	WndClass.lpszClassName = TEXT("SuperEdit"); // 새로운 이름 등록 
	WndClass.hCursor = LoadCursor(NULL, IDC_IBEAM); // 커서 변경 	
	RegisterClass(&WndClass); // 수정한 윈도우 등록 
	//  엔터로 줄 바꾸기(ES_WANTRETURN)
	DWORD style = WS_CHILD | WS_VISIBLE;// | ES_WANTRETURN;// | ES_MULTILINE;//| ES_AUTOVSCROLL | WS_VSCROLL;
	g_hEdit = CreateWindow(L"SuperEdit", NULL, style,
	//g_hEdit = CreateWindow(L"edit", NULL, style,
		g_rtEdit.left, g_rtEdit.top, g_rtEdit.right, g_rtEdit.bottom, m_hWnd, (HMENU)100,
		m_hInstance, NULL);		
	SetFocus(g_hEdit);
	g_hFont = CreateFont(20, 0, 0, FW_BOLD, 0, 0, 0, 0,DEFAULT_CHARSET, 0, 0, 0, 0,
							L"고딕");// L"MS Sans Serif");
	SendMessage(g_hEdit, WM_SETFONT, (WPARAM)g_hFont, 0);

	// button
	GetClassInfo(NULL, TEXT("button"), &WndClass); // "edit"클래스에 대한 정보 조사 
	WndClass.hInstance = g_hInstance;
	WndClass.style = CS_OWNDC;
	WndClass.lpszClassName = TEXT("SuperButton"); // 새로운 이름 등록 
	WndClass.hCursor = LoadCursor(NULL, IDC_WAIT); // 커서 변경 	
	RegisterClass(&WndClass); // 수정한 윈도우 등록 
	g_hButton = CreateWindow(L"SuperButton", L"Send", style,
		g_rtButton.left, g_rtButton.top, g_rtButton.right, g_rtButton.bottom, m_hWnd, (HMENU)200,	m_hInstance, NULL);
	//서브클래싱
	g_OldEditProc = (WNDPROC)SetWindowLongPtr(g_hButton, GWLP_WNDPROC, (LONG_PTR)SuperProc);
	
	m_ImageBK.Load(L"../../data/kgcabk.bmp");
	m_ImageEdit.Load(L"../../data/EditBoxBack.bmp");
	m_ImageButton.Load(L"../../data/main_fr_list.bmp");

	// 1)초기화
	if (!m_Network.Init())
	{
		return false;
	}
	// 2)접속
	if (!m_Network.Connect("192.168.0.151", 10000, m_hWnd))
	{
		return false;
	}
	// 3)모델 선택
	m_Network.SetModel(make_shared<TStdSelect>(m_Network.m_Sock));
	return true;
}
bool Sample::PreFrame()
{
	m_Network.Frame();
	PacketProcess();
	return true;
}
bool Sample::Frame()
{
	/* 초당 1번씩 전송한다.
	static float fTime = 0.0f;
	fTime += g_fSecPerFrame;
	if (fTime > 1.0f)
	{
		m_Network.SendMsg(PACKET_CHAR_MSG, username, sizeof(username));
		fTime -= 1.0f;
	}*/
	return true;
}
bool Sample::Render()
{
	RECT rt = { 0,0, m_ImageBK.m_bmpInfo.bmWidth, m_ImageBK.m_bmpInfo.bmHeight};	
	m_ImageBK.DrawColorKey(0,0,rt, RGB(255, 0, 0));
	
	RECT rtSrc1 = { 0, 0, 
				   m_ImageEdit.m_bmpInfo.bmWidth, m_ImageEdit.m_bmpInfo.bmHeight };
	RECT rtDesk1 = { g_rtEdit.left, g_rtEdit.top,  g_rtEdit.right, g_rtEdit.bottom };
	m_ImageEdit.Draw(rtDesk1, rtSrc1, SRCCOPY);

	RECT rtSrc2 = { 0, 0,
				   m_ImageButton.m_bmpInfo.bmWidth, m_ImageButton.m_bmpInfo.bmHeight };
	RECT rtDesk2 = { g_rtButton.left, g_rtButton.top,  g_rtButton.right, g_rtButton.bottom };
	m_ImageButton.Draw(rtDesk2, rtSrc2, SRCCOPY);
	return true;
}
bool Sample::PostRender()
{
	TCHAR buffer[MAX_PATH] = { 0, };
	SendMessage(g_hEdit,WM_GETTEXT,	MAX_PATH,(LPARAM)buffer);
	g_Write.Draw({ g_rtEdit.left, g_rtEdit.top }, buffer);

	TCore::PostRender();
	m_Network.SendPacketPool();
	return true;
}
bool Sample::Release()
{
	DeleteObject(g_hFont);
	m_Network.Release();
	SetWindowLongPtr(g_hButton, GWLP_WNDPROC, (LONG)g_OldEditProc);
	return true;
}
LRESULT		Sample::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		/*case WM_COMMAND:
		{
			switch (LOWORD(wParam))
			{
			case 200:
			{
				char buffer[MAX_PATH] = { 0, };
				SendMessageA(g_hEdit,
					WM_GETTEXT,
					MAX_PATH,
					(LPARAM)buffer);
				m_Network.SendMsg(PACKET_CHAR_MSG, buffer, sizeof(buffer));
			}
			break;
			}
		}break;*/	
	}
	m_Network.MsgProc(hWnd, msg, wParam, lParam);
	return 0;
}
Sample::Sample()
{
}


Sample::~Sample()
{
	
}

TCORE_RUN(TNetworkClient, 800, 600);