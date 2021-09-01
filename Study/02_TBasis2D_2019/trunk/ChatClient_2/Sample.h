#pragma once
#include "TCore.h"
#include "TClient.h"
#include "TUdpSocket.h"

class Sample: public TCore
{
public:
	TUdpSocket			m_Udp;
	TClient				m_Client;	
	HWND				m_hEdit;
	HWND				m_hChatEdit;
	TCHAR				m_strBuffer[128];
public:
	bool		Init();
	bool		Frame();
	bool		Release();
	bool		PreRender(){ return true; }
	bool		PostRender() { return true; }
	LRESULT			MsgProc(HWND hWnd, 
					UINT msg,
					WPARAM wParam,
					LPARAM lParam);
public:
	Sample(void);
	~Sample(void);
};
