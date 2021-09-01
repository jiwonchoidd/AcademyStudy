#pragma once
#include "TBitmap.h"

class TGraphicAPI
{
public:
	HDC		m_hScreenDC;
	HDC		m_hOffScreenDC;
	HBITMAP m_hOffScreenBitmap;
	HBRUSH  m_hbrRed;
public:
	static IWICImagingFactory *m_pIWICFactory;
public:
	BOOL LocateImageFile(HWND hWnd, LPWSTR pszFileName, DWORD cbFileName);
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	PreRender();
	bool	PostRender();
	bool	Release();
};

