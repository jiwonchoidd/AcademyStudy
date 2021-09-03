#pragma once
#include "TBitmap.h"
class xObject
{
public:
	TBitmap		m_Bitmap;
	TBitmap		m_BitmapMask;
	bool		m_bMask;
	tPOINT		m_ObjPos;
	RECT		m_rtObj;
	HDC			m_hOffScreenDC;
	HBRUSH		m_hBrush;
	HBRUSH		m_hOldBrush;
	DWORD		m_dwReverse;
	bool		m_bHero;
	int			m_iDirection[2];
	float		m_fSpeed;
	float		m_fAlpha;
	bool		m_bDebugRect;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	// clrMask 픽셀을 투명처리한다.
	void		TransBitBlt(HDC hdc, int x, int y, int width, int height,
							HDC MemDC, int mx, int my, COLORREF clrMask);
	bool AlphaBlend(HDC dcDest, int x, int y, int cx, int cy,
		HDC dcSrc,
		HDC dcMaskSrc, int sx, int sy, int scx, int scy, int alpha, DWORD opMode, COLORREF rgbMask);
public:
	void		SetRect(RECT rt, 
						tPOINT pos, 
						bool bHero=false, 
						float fSpeed= 10.0f);
	bool		Load(	HDC hScreenDC, 
						HDC hOffScrenDC, 
						TCHAR* pszMaskLoadFileName,
						TCHAR* pszLoadFileName);
	void		Left();
	void		Right();
	void		Up();
	void		Down();
public:
	xObject();
	virtual ~xObject();
};

