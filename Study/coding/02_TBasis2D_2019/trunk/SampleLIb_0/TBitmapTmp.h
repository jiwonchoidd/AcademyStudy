#pragma once
#include "resource.h"
#include "TStd.h"
class TBitmapTmp
{
public:
	HBITMAP		m_hBitmap;
	HDC			m_hMemDC;
	HBITMAP		m_hOldBitmap;
	BITMAP		m_BmpInfo;
public:
	bool		Set(HDC hdc, DWORD bitmap);
	bool		Draw(HDC hDC, RECT rt);
	bool		Init() {return true;};
	bool		Frame() {return true;};
	bool		Render() {	return true;};
	bool		Release();
public:
	TBitmapTmp();
	virtual ~TBitmapTmp();
};

