#pragma once
#include "TCoreStd.h"

#ifdef _DEBUG
	#define g_WindowWidth = 800
#else
	#define g_WindowWidth = 600
#endif

class TWindow
{
public:
	int		m_iStartX;
	int		m_iStartY;
	int		m_iWidth;
	int		m_iHeight;
	int		m_iStyle;
	HINSTANCE m_hInstance;
	HWND	m_hWnd;
	RECT	m_rcWindowRect;   // 윈도우    영역
	RECT	m_rcClientRect;   // 클라이언트영역

public:
	virtual LRESULT MsgProc(HWND hWnd, 
						 UINT message, 
						 WPARAM wParam, 
						 LPARAM lParam);
	virtual void SetStyle( int iStyle = WS_OVERLAPPEDWINDOW );
	virtual void SetWinRect( int ix, int iy,
					 int iWidth, 
					 int iHeight);
	bool    SetRegisterClass(HINSTANCE hInstance);
	virtual bool  SetWin(HINSTANCE hInstance,
				 int nCmdShow,
				 TCHAR* szTitle = _T("kgca Sample")
				 );
	
public:
	TWindow(void);
	~TWindow(void);
};

