#pragma once
#include "TSelectModel.h"
class TAsyncSelect : public TSelectModel
{
public:
	LRESULT		MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool		Init();
public:
	TAsyncSelect(SOCKET sock);
	virtual ~TAsyncSelect();
};

