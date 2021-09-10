#pragma once
#include "TStd.h"
struct TMsg
{
	RECT	rt;
	wstring msg;
};
class TWrite : public TSingleton<TWrite>
{
public:
	HFONT	m_hFont;
	std::vector<TMsg> m_TextList;
public:
	void   push(TMsg msg);
	void   Draw(POINT pos, wstring msg, DWORD color = RGB(255, 0, 0));
	void   Draw(int x, int y, wstring msg, DWORD color = RGB(255, 0, 0));
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	TWrite();
	virtual ~TWrite();
};
#define g_Write TWrite::GetInstance()
