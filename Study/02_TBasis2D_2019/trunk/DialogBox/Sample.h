#pragma once
#include "TCore.h"
#include "resource.h"
class Sample  : public TCore
{
public:
	HWND m_hModelessDlg;
	

public:
	Sample(void);
	~Sample(void);
	LRESULT MsgProc( HWND hWnd, 
						UINT msg,
						WPARAM wParam,
						LPARAM lParam );

};
