#pragma once
#include "eStd.h"
//��Ӱ���� ���� virtual
class Window
{
public:
	RECT m_rtWindow;
	RECT m_rtClient;

	HINSTANCE m_hInstance;
	HWND		m_hWnd;
public:
	bool	WindowRegister(HINSTANCE hInstance, int nCmdShow, const WCHAR* strTitle, int iWidth, int iHeight, DWORD dStyle);
	bool	WindowRun();
public:
	Window();
};

