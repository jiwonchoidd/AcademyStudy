#pragma once
#include "TCore.h"
#include "TSender.h"
#include "TRecver.h"
class Sample: public TCore
{
public:
	TSender				m_Sender;
	TRecver				m_Recver;
	
	
	
public:
	bool				Init();
	bool				PreRender();
	bool				Render();
	bool				PostRender();
	bool				Release();	
	LRESULT			MsgProc( HWND hWnd,
						UINT msg,
						WPARAM wParam,
						LPARAM lParam );
public:
	Sample(void);
	~Sample(void);
};
