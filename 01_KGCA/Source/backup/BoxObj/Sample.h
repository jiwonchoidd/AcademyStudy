#pragma once
#define IP_DD		"192.168.0.56"
#define IP_KGCA2	"192.168.0.12"
#define IP_1		"192.168.31.236"
#include "KCore.h"
#include "KAsyncSelect.h"
#include "KPlaneObj.h"
#include "KBoxObj.h"
class Sample : public KCore
{
public:
	KAsyncSelect		m_Net;
	bool				m_bConnect = false;
	int					m_iChatCount;
	char				m_chatItems[2048] = { 0, };
	char				m_buffer[MAX_PATH];
	char				ip_Address[30] = IP_DD;
	char				computer_name[24] = { 0, };
	std::string			str_isConnect = "DisConnected";
	KPlaneObj			plane;
	KBoxObj				box;
	bool		Init()override;
	bool		Frame()override;
	bool		Render()override;
	bool		Release()override;
	virtual LRESULT	ExternMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Sample();
	virtual ~Sample();
};

