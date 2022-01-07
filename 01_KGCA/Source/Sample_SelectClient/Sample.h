#pragma once
#include "KCore.h"
#include "KAsyncSelect.h"
class Sample : public KCore
{
public:
	KAsyncSelect		m_Net;
	HWND				m_hEditBox;
	HWND				m_hButton;
	HWND				m_hListBox;
	int					m_iChatCount;
	bool		Init()override;
	bool		Frame()override;
	bool		Render()override;
	bool		Release()override;
	virtual LRESULT	ExternMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	Sample();
	virtual ~Sample();
};

