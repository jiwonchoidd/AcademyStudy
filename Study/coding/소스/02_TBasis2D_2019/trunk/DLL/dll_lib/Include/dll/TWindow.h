#pragma once
#include "TCoreStd.h"

class TCOREDLL_API TWindow
{
public:
	HINSTANCE	m_hInstance;
	HWND		m_hWnd;
public:
	const int	m_iBackgroundColor;	
	RECT		m_rtClient;
	RECT		m_rtWindow;
	int			m_iWindowX;
	int			m_iWindowY;
	int			m_iWindowWidth;
	int			m_iWindowHeight;	
public:
	virtual LRESULT MsgProc(	HWND hWnd,
							UINT msg,
							WPARAM wParam,
							LPARAM lParam);
	void	SetPositionSize( int iX, 
							int iY, 
							int iWidth, 
							int iHeight);
	bool   SetRegister();
	bool   CreateWin( WCHAR* strTitleName);
	bool   SetWindow(HINSTANCE hInst,
				   	  int nCmdShow,
					  WCHAR* strTitleName);
public:
	TWindow(void);
	virtual ~TWindow(void);
};

