#pragma once
#include "TStd.h"


class TWindow
{
public:
	HWND		m_hWnd; // 윈도우 핸들
	HINSTANCE	m_hInstance; // 실행객체 핸들
	RECT		m_rtWindow;
	RECT		m_rtClient;
public:
	void	CenterWindow();
	bool	SetWindow(HINSTANCE hInstance,
		TCHAR* pTitleName = L"Sample",
		int iWidth=800, int iHeight=600 );
	bool	Run();
	bool	Release();
	virtual  bool	GameRun() { return true; };
	virtual  void   MsgEvent(MSG msg) {};
	virtual  bool	GameInit() { return true; };
	virtual  bool   GameRelease() { return true; };
public:
	TWindow();
	virtual ~TWindow();
};

