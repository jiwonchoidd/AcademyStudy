#pragma once
#include "TCore.h"
class Sample: public TCore
{
public:	
	bool   Init();
	bool   Release();
	bool	PreRender() { return true; }
	bool	PostRender() { return true; }
	
	LRESULT MsgProc(HWND hWnd, 
						 UINT message, 
						 WPARAM wParam, 
						 LPARAM lParam);
public:
	Sample(void);
	~Sample(void);
};
