#pragma once
#include "TCore.h"
#include "TAsyncSelect.h"

class Sample : public TCore
{
public:
	TAsyncSelect	m_Network;
	HWND m_hEdit;
	HWND m_hButton;
	HWND m_hList;
public:
	bool	GameRender() { return true;	}
	bool			Init();
	bool			PreFrame();
	bool			Frame();
	bool			Release();
	LRESULT			MsgProc(HWND hWnd,
		UINT msg,
		WPARAM wParam,
		LPARAM lParam);
	void			MsgEvent(MSG msg);
public:
	Sample();
	virtual ~Sample();
};

